#!/bin/sh
#CPCMD="ln -s "
CPCMD="cp "
REPOS="/Users/kaestner/repos"
DEST="build-QtnGITool-Qt5-Release/QtnGITool.app/Contents/MacOS"
pushd .
cd $DEST
rm -f *.1.0.0.dylib
rm -f *.dylib
rm -f *.1.dylib
rm -f *.1.0.dylib

`$CPCMD $REPOS/qni/trunk/src/ImagingAlgorithms/build-ImagingAlgorithms-Qt5-Release/libImagingAlgorithms.1.0.0.dylib .`
`$CPCMD $REPOS/modules/trunk/ModuleConfig/build-ModuleConfig-Qt5-Release/libModuleConfig.1.0.0.dylib .`
`$CPCMD $REPOS/gui/trunk/qt/build-QtAddons-Qt5-Release/libQtAddons.1.0.0.dylib .`
`$CPCMD $REPOS/gui/trunk/qt/build-QtModuleConfigure-Qt5-Release/libQtModuleConfigure.1.0.0.dylib .`
`$CPCMD $REPOS/kipl/trunk/kipl/build-kipl-Qt5-Release/libkipl.1.0.0.dylib .`
`$CPCMD $REPOS/ngi/trunk/core/nGIEstimators/build-nGIEstimators-Qt5-Release/libnGIEstimators.1.0.0.dylib .`
`$CPCMD $REPOS/ngi/trunk/core/nGIFramework/build-nGIFramework-Qt5-Release/libnGIFramework.1.0.0.dylib .`
`$CPCMD $REPOS/ngi/trunk/core/nGIPreprocessing/build-nGIPreprocessing-Qt5-Release/libnGIPreprocessing.1.0.0.dylib .`

# `$CPCMD /Users/kaestner/QtSDK/Madde/sysroots/harmattan_sysroot_10.2011.34-1_slim/usr/lib/libgomp.so.1.0.0 .`


for f in `ls *.1.0.0.dylib`; do
	bn=`basename $f .1.0.0.dylib`
	echo $bn
	ln -s $f $bn.1.0.dylib
    ln -s $bn.1.0.dylib $bn.1.dylib
    ln -s $bn.1.dylib $bn.dylib
done

#`$CPCMD /Users/kaestner/repos/tomography/trunk/src/muhrec3/resources/* $DEST/../Resources`

popd
