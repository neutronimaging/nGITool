#!/bin/sh

mkdir /tmp/QtnGITool
ln -s /Applications /tmp/QtnGITool/Applications
cp -r ~/Applications/QtnGITool.app /tmp/QtnGITool

hdiutil create -volname QtnGITool -srcfolder /tmp/QtnGITool -ov -format UDZO ~/QtnGITool_`date +%Y%m%d`.dmg

