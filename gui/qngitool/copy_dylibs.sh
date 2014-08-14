#!/bin/sh
#CPCMD="ln -s "
CPCMD="cp "
REPOS="/Users/kaestner/repos"
DEST="QtnGITool-build-desktop-Desktop_Qt_4_8_1_for_GCC__Qt_SDK__Release/QtnGITool.app/Contents/MacOS"
pushd .
cd $DEST
rm -f *.1.0.0.dylib
rm -f *.dylib
rm -f *.1.dylib
rm -f *.1.0.dylib

`$CPCMD $REPOS/qni/trunk/src/ImagingAlgorithms/qt/ImagingAlgorithms-build-Qt_4_8_1_for_GCC__Qt_SDK__Release/libImagingAlgorithms.1.0.0.dylib .`
`$CPCMD $REPOS/modules/trunk/ModuleConfig/ModuleConfig-build-Qt_4_8_1_Release/libModuleConfig.1.0.0.dylib .`
`$CPCMD $REPOS/gui/trunk/qt/QtAddons-build_Qt_4_8_1_for_GCC__Qt_SDK__Release/libQtAddons.1.0.0.dylib .`
`$CPCMD $REPOS/gui/trunk/qt/QtModuleConfigure-build-Qt_4_8_1_for_GCC__Qt_SDK__Release/libQtModuleConfigure.1.0.0.dylib .`
`$CPCMD $REPOS/kipl/trunk/kipl/kipl-build-Qt_4_8_1_Release/libkipl.1.0.0.dylib .`
`$CPCMD $REPOS/ngi/trunk/core/nGIEstimators/nGIEstimators-build-desktop-Desktop_Qt_4_8_1_for_GCC__Qt_SDK__Release/libnGIEstimators.1.0.0.dylib .`
`$CPCMD $REPOS/ngi/trunk/core/nGIFramework/nGIFramework-build-Qt_4_8_1_Release/libnGIFramework.1.0.0.dylib .`
`$CPCMD $REPOS/ngi/trunk/core/nGIPreprocessing/nGIPreprocessing-build-desktop-Desktop_Qt_4_8_1_for_GCC__Qt_SDK__Release/libnGIPreprocessing.1.0.0.dylib .`

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