DIRECTORY="/Users/kaestner/Applications"
REPOSPATH="/Users/kaestner/repos"
LIBPATH=$REPOSPATH/lib
#QTPATH="/Applications/Qt541/5.4/clang_64/"
QTPATH="/Applications/Qt56/5.6/clang_64/"
if [ ! -d "$DIRECTORY" ]; then
  mkdir $DIRECTORY
fi

cp -r $REPOSPATH/Applications/QtnGITool.app $DIRECTORY

pushd .
CPCMD="cp"
DEST="$DIRECTORY/QtnGITool.app"
cd $DEST/Contents
pwd

if [ ! -d "./Frameworks" ]; then
 mkdir ./Frameworks
fi

`$CPCMD $LIBPATH/libImagingAlgorithms.1.0.0.dylib $DEST/Contents/Frameworks`
`$CPCMD $LIBPATH/libModuleConfig.1.0.0.dylib $DEST/Contents/Frameworks`
`$CPCMD $LIBPATH/libQtAddons.1.0.0.dylib $DEST/Contents/Frameworks`
`$CPCMD $LIBPATH/libQtModuleConfigure.1.0.0.dylib $DEST/Contents/Frameworks`
`$CPCMD $LIBPATH/libkipl.1.0.0.dylib $DEST/Contents/Frameworks`
`$CPCMD $LIBPATH/libnGIFramework.1.0.0.dylib $DEST/Contents/Frameworks`
`$CPCMD $LIBPATH/libnGIEstimators.1.0.0.dylib $DEST/Contents/Frameworks`
`$CPCMD $LIBPATH/libnGIPreprocessing.1.0.0.dylib $DEST/Contents/Frameworks`

#rm -f ./MacOS/*.dylib

#for f in `ls ./Frameworks/*.1.0.0.dylib`; do
#	ln -s ../$f "./MacOS/`basename $f .1.0.0.dylib`.1.dylib"
#done

cd Frameworks
rm -f *.1.0.dylib
rm -f *.1.dylib

for f in `ls *.1.0.0.dylib`; do
	ln -s $f "`basename $f .1.0.0.dylib`.1.0.dylib"
	ln -s $f "`basename $f .1.0.0.dylib`.1.dylib"
done
cd ..
pwd


if [ ! -d "./Resources" ]; then
	mkdir ./Resources	
fi
# cp $REPOSPATH/ography/trunk/src/muhrec3/resources/* ./Resources

popd
sed -i.bak s+com.yourcompany+ch.imagingscience+g $DEST/Contents/Info.plist

if [ ! -d "./PlugIns" ]; then
 mkdir ./PlugIns
fi

if [ ! -d "./PlugIns/platforms" ]; then
 mkdir ./PlugIns/platforms
fi
cp $QTPATH/plugins/platforms/libqcocoa.dylib $DEST/Contents/PlugIns/platforms/

if [ ! -d "./PlugIns/printsupport" ]; then
 mkdir ./PlugIns/printsupport
fi
cp $QTPATH/plugins/printsupport/libcocoaprintersupport.dylib $DEST/Contents/PlugIns/printsupport/

if [ ! -d "./PlugIns/accessible" ]; then
 mkdir ./PlugIns/accessible
fi
cp $QTPATH/plugins/accessible/libqtaccessiblewidgets.dylib $DEST/Contents/PlugIns/accessible/

pushd .
cd $QTPATH/bin/
echo "Do deploy..."
./macdeployqt $DEST #-dmg

cd $DEST/Contents/MacOS
# QtnGITool
install_name_tool -change libkipl.1.dylib @executable_path/../Frameworks/libkipl.1.dylib QtnGITool
install_name_tool -change libModuleConfig.1.dylib @executable_path/../Frameworks/libModuleConfig.1.dylib QtnGITool
install_name_tool -change libQtAddons.1.dylib @executable_path/../Frameworks/libQtAddons.1.dylib QtnGITool
install_name_tool -change libQtModuleConfigure.1.dylib @executable_path/../Frameworks/libQtModuleConfigure.1.dylib QtnGITool
install_name_tool -change libnGIFramework.1.dylib @executable_path/../Frameworks/libnGIFramework.1.dylib QtnGITool

cd ../Frameworks

# ModuleConfig
install_name_tool -change libkipl.1.dylib @executable_path/../Frameworks/libkipl.1.dylib libModuleConfig.1.0.0.dylib

# QtAddons
install_name_tool -change libkipl.1.dylib @executable_path/../Frameworks/libkipl.1.dylib libQtAddons.1.0.0.dylib

# QtModuleConfigure
install_name_tool -change libkipl.1.dylib @executable_path/../Frameworks/libkipl.1.dylib libQtModuleConfigure.1.0.0.dylib
install_name_tool -change libModuleConfig.1.dylib @executable_path/../Frameworks/libModuleConfig.1.dylib libQtModuleConfigure.1.0.0.dylib

# nGIFramework
install_name_tool -change libkipl.1.dylib @executable_path/../Frameworks/libkipl.1.dylib libnGIFramework.1.0.0.dylib
install_name_tool -change libModuleConfig.1.dylib @executable_path/../Frameworks/libModuleConfig.1.dylib libnGIFramework.1.0.0.dylib

# ImagingAlgorithms
install_name_tool -change libkipl.1.dylib @executable_path/../Frameworks/libkipl.1.dylib libImagingAlgorithms.1.0.0.dylib

# nGIPreprocessing
install_name_tool -change libkipl.1.dylib @executable_path/../Frameworks/libkipl.1.dylib libnGIPreprocessing.1.0.0.dylib
install_name_tool -change libModuleConfig.1.dylib @executable_path/../Frameworks/libModuleConfig.1.dylib libnGIPreprocessing.1.0.0.dylib
install_name_tool -change libImagingAlgorithms.1.dylib @executable_path/../Frameworks/libImagingAlgorithms.1.dylib libnGIPreprocessing.1.0.0.dylib
install_name_tool -change libnGIFramework.1.dylib @executable_path/../Frameworks/libnGIFramework.1.dylib libnGIPreprocessing.1.0.0.dylib

# nGIEstimators
install_name_tool -change libkipl.1.dylib @executable_path/../Frameworks/libkipl.1.dylib libnGIEstimators.1.0.0.dylib
install_name_tool -change libModuleConfig.1.dylib @executable_path/../Frameworks/libModuleConfig.1.dylib libnGIEstimators.1.0.0.dylib
install_name_tool -change libImagingAlgorithms.1.dylib @executable_path/../Frameworks/libImagingAlgorithms.1.dylib libnGIEstimators.1.0.0.dylib
install_name_tool -change libnGIFramework.1.dylib @executable_path/../Frameworks/libnGIFramework.1.dylib libnGIEstimators.1.0.0.dylib

popd .