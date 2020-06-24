﻿@echo ----------------- Deploy copy --------------
set REPOS=%WORKSPACE%
set DEST=%WORKSPACE%\deployed\nGITool


if not exist %DEST% mkdir %DEST%

pushd .
cd %DEST%
mkdir resources

copy %REPOS%\lib\ImagingAlgorithms.dll .
copy %REPOS%\lib\ImagingQAAlgorithms.dll .
copy %REPOS%\lib\ModuleConfig.dll .
copy %REPOS%\lib\QtAddons.dll .
copy %REPOS%\lib\QtImaging.dll .
copy %REPOS%\lib\QtModuleConfigure.dll .
copy %REPOS%\lib\kipl.dll .
copy %REPOS%\lib\ReaderConfig.dll .
copy %REPOS%\lib\ReaderGUI.dll .

copy %REPOS%\lib\nGIEstimators.dll .
copy %REPOS%\lib\nGIFramework.dll .
copy %REPOS%\lib\nGIPreprocessing.dll .

copy %REPOS%\imagingsuite\external\lib64\libtiff.dll .
copy %REPOS%\imagingsuite\external\lib64\libjpeg-62.dll .
copy %REPOS%\imagingsuite\external\lib64\zlib1.dll .
copy %REPOS%\imagingsuite\external\lib64\libfftw3-3.dll .
copy %REPOS%\imagingsuite\external\lib64\libfftw3f-3.dll .
copy %REPOS%\imagingsuite\external\lib64\libxml2-2.dll .
copy %REPOS%\imagingsuite\external\lib64\libiconv.dll .
copy %REPOS%\imagingsuite\external\lib64\cfitsio.dll .
copy %REPOS%\imagingsuite\external\lib64\nexus\NeXus.dll .
copy %REPOS%\imagingsuite\external\lib64\nexus\NexusCPP.dll .
copy %REPOS%\imagingsuite\external\lib64\hdf5\hdf5.dll .
copy %REPOS%\imagingsuite\external\lib64\hdf5\hdf5_cpp.dll .
copy %REPOS%\imagingsuite\external\lib64\hdf5\zlib.dll .
copy %REPOS%\imagingsuite\external\lib64\hdf5\szip.dll .
copy %REPOS%\imagingsuite\external\lib64\libgcc_s_seh_64-1.dll .
copy %REPOS%\imagingsuite\external\lib64\libgcc_s_seh-1.dll .
copy %REPOS%\imagingsuite\external\lib64\libgfortran_64-3.dll .
copy %REPOS%\imagingsuite\external\lib64\libquadmath-0.dll .
copy %REPOS%\imagingsuite\external\lib64\libwinpthread-1.dll .
copy %REPOS%\imagingsuite\external\lib64\libblas.dll .
copy %REPOS%\imagingsuite\external\lib64\liblapack.dll .

copy %REPOS%\Applications\nGITool.exe .

cd %QTBINPATH%

windeployqt %DEST%\nGITool.exe

copy Qt5PrintSupport.dll %DEST%
copy Qt5Charts.dll %DEST%

popd

