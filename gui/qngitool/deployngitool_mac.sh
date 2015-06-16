DIRECTORY="/Users/kaestner/Applications"
REPOSPATH="/Users/kaestner/repos"
QTPATH="/Applications/Qt54/5.4/clang_64/"
if [ ! -d "$DIRECTORY" ]; then
  mkdir $DIRECTORY
fi

cp -r $REPOSPATH/ngi/trunk/gui/qngitool/build-QtnGITool-Qt5-Release/QtnGITool.app $DIRECTORY

pushd .
CPCMD="cp"
DEST="$DIRECTORY/QtnGITool.app"
cd $DEST/Contents
pwd
if [ ! -d "./Frameworks" ]; then
 mkdir ./Frameworks
fi

`$CPCMD $REPOSPATH/qni/trunk/src/ImagingAlgorithms/build-ImagingAlgorithms-Qt5-Release/libImagingAlgorithms.1.0.0.dylib $DEST/Contents/Frameworks`
`$CPCMD $REPOSPATH/modules/trunk/ModuleConfig/build-ModuleConfig-Qt5-Release/libModuleConfig.1.0.0.dylib $DEST/Contents/Frameworks`
`$CPCMD $REPOSPATH/gui/trunk/qt/build-QtAddons-Qt5-Release/libQtAddons.1.0.0.dylib $DEST/Contents/Frameworks`
`$CPCMD $REPOSPATH/gui/trunk/qt/build-QtModuleConfigure-Qt5-Release/libQtModuleConfigure.1.0.0.dylib $DEST/Contents/Frameworks`
`$CPCMD $REPOSPATH/kipl/trunk/kipl/build-kipl-Qt5-Release/libkipl.1.0.0.dylib $DEST/Contents/Frameworks`
`$CPCMD $REPOSPATH/ngi/trunk/core/nGIFramework/build-nGIFramework-Qt5-Release/libnGIFramework.1.0.0.dylib $DEST/Contents/Frameworks`
`$CPCMD $REPOSPATH/ngi/trunk/core/nGIEstimators/build-nGIEstimators-Qt5-Release/libnGIEstimators.1.0.0.dylib $DEST/Contents/Frameworks`
`$CPCMD $REPOSPATH/ngi/trunk/core/nGIPreprocessing/build-nGIPreprocessing-Qt5-Release/libnGIPreprocessing.1.0.0.dylib $DEST/Contents/Frameworks`

rm -f ./MacOS/*.dylib

for f in `ls ./Frameworks/*.1.0.0.dylib`; do
	ln -s ../$f "./MacOS/`basename $f .1.0.0.dylib`.1.dylib"
done

cd Frameworks
rm -f *.1.0.dylib
rm -f *.1.dylib

for f in `ls *.1.0.0.dylib`; do
	ln -s $f "`basename $f .1.0.0.dylib`.1.0.dylib"
	ln -s $f "`basename $f .1.0.0.dylib`.1.dylib"
done
cd ..

if [ ! -d "./Resources" ]; then
	mkdir ./Resources	
fi
# cp $REPOSPATH/ography/trunk/src/muhrec3/resources/* ./Resources

popd
sed -i.bak s+com.yourcompany+ch.imagingscience+g $DEST/Contents/Info.plist

cd $QTPATH/bin/
echo "Do deploy..."
./macdeployqt $DEST #-dmg
popd 