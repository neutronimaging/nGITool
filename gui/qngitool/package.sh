#!/bin/sh

fname=~/QTnGITool_`uname -s`_`uname -m`_`date +%Y%m%d`_rev`svn info |grep Revision | sed -e 's/Revision: //'`.tar.bz2

echo $fname
pushd .
cd ~/repos/ngi
tar -jcvf $fname qngitool
popd
 
