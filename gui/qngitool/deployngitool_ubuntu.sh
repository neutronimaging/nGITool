#CPCMD="ln -s "
CPCMD="cp "
REPOS="/home/`whoami`/repos"

DEST="$REPOS/ngi/qngitool"
mkdir --parent $DEST
mkdir --parent $DEST/bin
mkdir --parent $DEST/Frameworks
mkdir --parent $DEST/resources

pushd .
cd $DEST/Frameworks
rm -f *.so.1.0.0
rm -f *.so
rm -f *.so.1
rm -f *.so.1.0

`$CPCMD $REPOS/qni/trunk/src/ImagingAlgorithms/build-ImagingAlgorithms-Qt5-Release/libImagingAlgorithms.so.1.0.0 .`
`$CPCMD $REPOS/modules/trunk/ModuleConfig/build-ModuleConfig-Qt5-Release/libModuleConfig.so.1.0.0 .`
`$CPCMD $REPOS/gui/trunk/qt/build-QtAddons-Qt5-Release/libQtAddons.so.1.0.0 .`
`$CPCMD $REPOS/gui/trunk/qt/build-QtModuleConfigure-Qt5-Release/libQtModuleConfigure.so.1.0.0 .`
`$CPCMD $REPOS/kipl/trunk/kipl/build-kipl-Qt5-Release/libkipl.so.1.0.0 .`
`$CPCMD $REPOS/ngi/trunk/core/nGIEstimators/build-nGIEstimators-Qt5-Release/libnGIEstimators.so.1.0.0 .`
`$CPCMD $REPOS/ngi/trunk/core/nGIFramework/build-nGIFramework-Qt5-Release/libnGIFramework.so.1.0.0 .`
`$CPCMD $REPOS/ngi/trunk/core/nGIPreprocessing/build-nGIPreprocessing-Qt5-Release/libnGIPreprocessing.so.1.0.0 .`

for f in `ls *.so.1.0.0`; do
	bn=`basename $f .so.1.0.0`
	echo $bn
	ln -s $f $bn.so.1.0
    ln -s $bn.so.1.0 $bn.so.1
    ln -s $bn.so.1 $bn.so
done


rm -f *.so
rm -f *.so.1
rm -f *.so.1.0

for f in `ls *.so.1.0.0`; do
	bn=`basename $f .so.1.0.0`
	echo $bn
	ln -s $f $bn.so.1.0
        ln -s $bn.so.1.0 $bn.so.1
	ln -s $bn.so.1 $bn.so
done

cd $DEST/bin
`$CPCMD $REPOS/ngi/trunk/gui/qngitool/build-QtnGITool-Qt5-Release/QtnGITool .`
cd ..
`$CPCMD $REPOS/ngi/trunk/gui/qngitool/scripts/qtngitool .`
chmod +x qtngitool

cp $REPOS/tomography/trunk/src/muhrec3/resources/*.xml resources
popd


