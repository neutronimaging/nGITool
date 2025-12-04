from conan import ConanFile
from conan.tools.cmake import CMakeToolchain, CMake, cmake_layout, CMakeDeps
from conan.tools.env import VirtualRunEnv
from conan.tools.files import copy
import os
import shutil
from io import StringIO

class nGIToolRecipe(ConanFile):
    settings = "os", "compiler", "build_type", "arch"
    default_options = {"*:shared": True}

    def requirements(self):
        self.requires("zlib/[1.3.1]")
        self.requires("openblas/[0.3.25]") # Can't update to 0.3.27 because of armadillo
        self.requires("pybind11/3.0.1")
        self.requires("libxml2/[2.15.0]")
        self.requires("armadillo/[12.6.4]")
        self.requires("libtiff/[4.7.1]")
        self.requires("fftw/[3.3.10]")
        self.requires("cfitsio/[4.6.3]")
        self.requires("hdf5/[1.14.6]")
        self.requires("nlohmann_json/[3.12.0]")


        if self.settings.os == "Windows":
            self.requires("dirent/1.24") # Header files only
        #self.requires("qt/[6.6.1]") Does work but QtCharts is not included


    def build_requirements(self): # Only used if conanbuild.bat environment is used, such as by "conan build ."
        self.tool_requires("ninja/[1.13.1]")
        self.tool_requires("cmake/[4.1.2]") 

    def layout(self):
        cmake_layout(
            self,
            build_folder="../build-nGITool",
            )

    def generate(self):
        deps = CMakeDeps(self)
        deps.generate()
        tc = CMakeToolchain(self, generator="Ninja") #default is None
        tc.generate()
        ms = VirtualRunEnv(self)
        ms.generate()
        bin_folder = os.path.abspath(os.path.join(self.build_folder, "bin", self.cpp.build.bindir))
        print("bindir is: ", self.cpp.build.bindir)
        self.lib_folder = os.path.abspath(os.path.join(self.build_folder, "lib", self.cpp.build.bindir))
        self.framework_folder_nGITool = os.path.abspath(os.path.join(self.build_folder, self.cpp.build.bindir, 'nGITool.app', 'Contents', 'Frameworks'))
        # Copy dynamic libraries from conan
        for dep in self.dependencies.values():
            if len(dep.cpp_info.bindirs)>0: # Avoid errors when using header-only files such as dirent. Can probably be done neater
                copy(self, "*.dll", dep.cpp_info.bindirs[0], bin_folder)
            if len(dep.cpp_info.libdirs)>0:
                copy(self, "*.so*", dep.cpp_info.libdirs[0], self.lib_folder)
                copy(self, "*.dylib", dep.cpp_info.libdirs[0], self.lib_folder)
            
        # Copy dynamic libraries from qt
        qtpath = os.environ["QTPATH"]
        Qt_dynamic_library_list = ["Qt6PrintSupport", "Qt6Charts", "Qt6OpenGLWidgets", "Qt6OpenGl", "Qt6Test"]
        Qt_linux_library_list = ["Qt6Core","Qt6Gui","Qt6Widgets","Qt6DBus","Qt6XcbQpa","icui18n","icudata","icuuc"]
        for library in Qt_dynamic_library_list:
            copy(self, library+".dll", os.path.join(qtpath, "bin"), bin_folder)
            #copy(self, library+".dylib", os.path.join(qtpath, "bin"), bin_folder)
            copy(self, "lib"+library+".so*", os.path.join(qtpath, "lib"), self.lib_folder)
        if self.settings.os == "Linux":
            for library in Qt_linux_library_list:
                copy(self, "lib"+library+".so*", os.path.join(qtpath, "lib"), self.lib_folder)
            copy(self, "libqxcb.so", os.path.join(qtpath, "plugins", "platforms"), os.path.join(bin_folder, "platforms"))
        
         # Allow override via env var IMGSUITE_LIB_DIR, default to ../build-imagingsuite/Release/lib
        default_imgsuite = os.path.abspath(os.path.join(self.source_folder, "..", "build-imagingsuite", "Release", "lib"))
        imgsuite_lib_dir = os.environ.get("IMGSUITE_LIB_DIR", default_imgsuite)

        # List of imagingsuite libs used by nGITool
        imgsuite_libs = [
            "kipl",
            "ModuleConfig",
            "ImagingAlgorithms",
            "ReaderConfig",
            "QtAddons",
            "ReaderGUI",
            "QtModuleConfigure",
            "QtImaging",
        ]

        if self.settings.os == "Windows":
            for name in imgsuite_libs:
                copy(self, f"{name}.dll", imgsuite_lib_dir, bin_folder)
        elif self.settings.os == "Linux":
            for name in imgsuite_libs:
                copy(self, f"lib{name}.so*", imgsuite_lib_dir, self.lib_folder)
        else:  # Macos
            for name in imgsuite_libs:
                copy(self, f"lib{name}.dylib", imgsuite_lib_dir, self.lib_folder)
        
        if self.settings.os == "Windows":
            dst = os.path.join(bin_folder,"resources")
        elif self.settings.os == "Linux":
            dst = os.path.join(bin_folder,"..","resources")
        else:
            dst = os.path.join(self.framework_folder_nGITool,"..",'Resources')
            if self.settings.arch == "armv8":
                sse2neon_dir = StringIO()
                self.run("brew --prefix sse2neon", stdout=sse2neon_dir)
                sse2neon = sse2neon_dir.getvalue().strip()
                copy(self, 'sse2neon/sse2neon.h', os.path.join(sse2neon, "include"), self.lib_folder)
        shutil.copytree(
            os.path.join(self.source_folder,"applications","nGITool","Resources"), 
            dst,
            dirs_exist_ok=True,
            )
    

    def build(self):
        cmake = CMake(self)
        cmake.configure()
        cmake.build()
        if self.settings.os == "Macos":
            copy(self, "*.dylib", self.lib_folder, self.framework_folder_nGITool, excludes='*cpython*')
            