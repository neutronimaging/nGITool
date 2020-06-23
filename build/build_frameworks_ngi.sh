#!/bin/bash
if [ `uname` == 'Linux' ]; then
    SPECSTR="-spec linux-g++"
else
    SPECSTR="-spec macx-clang CONFIG+=x86_64"
fi

REPOSPATH=$WORKSPACE/nGITool

DEST=$WORKSPACE/builds

echo Build nGIFramework
mkdir -p $DEST/build-nGIFramework
cd $DEST/build-nGIFramework

$QTBINPATH/qmake -makefile -r $SPECSTR -o Makefile ../../nGITool/frameworks/ngi/nGIFramework/qt/nGIFramework.pro
make -f Makefile clean
make -f Makefile mocables all
make -f Makefile

echo Build nGIEstimators
mkdir -p $DEST/build-nGIEstimators
cd $DEST/build-nGIEstimators

$QTBINPATH/qmake -makefile -r $SPECSTR -o Makefile ../../nGITool/frameworks/ngi/nGIEstimators/qt/nGIEstimators.pro
make -f Makefile clean
make -f Makefile mocables all
make -f Makefile

echo Build nGIPreprocessing
mkdir -p $DEST/build-nGIPreprocessing
cd $DEST/build-nGIPreprocessing

$QTBINPATH/qmake -makefile -r $SPECSTR -o Makefile ../../nGITool/frameworks/ngi/nGIPreprocessing/qt/nGIPreprocessing.pro
make -f Makefile clean
make -f Makefile mocables all
make -f Makefile


if [ -e "$REPOSPATH/frameworks/ngi/UnitTests" ]
then
    echo "Build tests"

    for f in `ls $REPOSPATH/frameworks/ngi/UnitTests`
    do
        echo "$REPOSPATHGUI/frameworks/ngi/UnitTests/$f/$f.pro"
        if [ -e "$REPOSPATH/frameworks/ngi/UnitTests/$f/$f.pro" ]
        then
            mkdir -p $DEST/build-$f
            cd $DEST/build-$f

            $QTBINPATH/qmake -makefile -r $SPECSTR -o Makefile ../../imagingsuite/frameworks/imageprocessing/UnitTests/$f/$f.pro
            make -f Makefile clean
            make -f Makefile mocables all
            make -f Makefile
        fi

    done

    echo "Tests built"
fi