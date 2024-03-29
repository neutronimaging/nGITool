call set_vars.bat
pushd .

set REPOSPATH=%WORKSPACE%\nGITool
set DEST=%WORKSPACE%\builds
set SPECSTR=-spec win32-msvc

mkdir %DEST%\build-nGITool
cd %DEST%\build-nGITool

%QTBINPATH%\qmake.exe -makefile -r %SPECSTR% %REPOSPATH%\applications\ngitool\nGITool\nGITool.pro -o Makefile
%QTBINPATH%\..\..\..\Tools\QtCreator\bin\jom\jom.exe -f Makefile.release clean
%QTBINPATH%\..\..\..\Tools\QtCreator\bin\jom\jom.exe -f Makefile.release mocables all
%QTBINPATH%\..\..\..\Tools\QtCreator\bin\jom\jom.exe -f Makefile.release

popd