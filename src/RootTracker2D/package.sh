#!/bin/sh

fname=~/roottracker2d_`uname -s`_`uname -m`_`date +%Y%m%d`_rev`svn info |grep Revision | sed -e 's/Revision: //'`.tar.bz2

echo $fname
tar -jcvhf $fname "../../../roottracker2d"

