#!/bin/bash
repos_path=~/repos
base_path=/opt/ngi
app_path=$repos_path/src/apps/dpc/trunk
lib_path=$repos_path/src/libs

if  [ ! -d $base_path ]; then
	mkdir $base_path
	cd $base_path
	ln -s $app_path/dpc/resources .
	ln -s $app_path/dpc/Release/dpc .
	ln -s $app_path/nGIFramework/Release/libnGIFramework.so .
	ln -s $app_path/nGIEstimators/Release/libnGIEstimators.so .
	ln -s $app_path/nGIPreprocessing/Release/libnGIPreprocessing.so .
	ln -s $app_path/nGIPreprocessingGUI/Release/libnGIPreprocessingGUI.so .

	ln -s $lib_path/modules/trunk/ModuleConfig/Release/libModuleConfig.so .
	ln -s $lib_path/modules/trunk/ModuleConfigurator/Release/libModuleConfigurator.so .
	ln -s $lib_path/gtk_addons/trunk/gtkmm_addons/Release/libgtkmm_addons.so .
	ln -s $repos_path/../qni/trunk/src/ImagingAlgorithms/Release/libImagingAlgorithms.so .

	ln -s $app_path/dpc/install .
fi



