#!/bin/sh

fname=~/qtngitool_`uname -s`_`uname -m`_`date +%Y%m%d`_rev`svn info |grep Revision | sed -e 's/Revision: //'`.tar.bz2

echo $fname
pushd .
cd ~/repos/ngi
#tar -jcvhf $fname muhrec3
tar -jcvf $fname qtngitool
popd
 
