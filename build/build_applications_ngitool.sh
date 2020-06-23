#!/bin/bash
if [ `uname` == 'Linux' ]; then
    SPECSTR="-spec linux-g++"
else
    SPECSTR="-spec macx-clang CONFIG+=x86_64"
fi

REPOSPATH=$WORKSPACE/nGITool

DEST=$WORKSPACE/builds

mkdir -p $DEST/build-nGITool
cd $DEST/build-nGITool

$QTBINPATH/qmake -makefile -r $SPECSTR -o Makefile ../../nGITool/applications/ngitool/nGITool/nGITool.pro
make -f Makefile clean
make -f Makefile mocables all
make -f Makefile



if [ -e "$REPOSPATH/applications/ngitool/UnitTests" ]
echo "Build tests"
then
    for f in `ls $REPOSPATH/applications/ngitool/UnitTests`
    do
        echo "$REPOSPATH/applications/ngitool/UnitTests/$f/$f.pro"
        if [ -e "$REPOSPATH/applications/ngitool/UnitTests/$f/$f.pro" ]
        then
            mkdir -p $DEST/build-$f
            cd $DEST/build-$f

            $QTBINPATH/qmake -makefile -r $SPECSTR -o Makefile ../../nGITool/applications/ngitool/UnitTests/$f/$f.pro
            make -f Makefile clean
            make -f Makefile mocables all
            make -f Makefile
        fi
    done

echo "Tests built"
fi