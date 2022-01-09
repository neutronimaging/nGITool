call set_vars.bat
pushd .

set REPOSPATH=%WORKSPACE%\nGITool
set DEST=%WORKSPACE%\builds
set SPECSTR=-spec win32-msvc

mkdir %DEST%\build-nGIFramework
cd %DEST%\build-nGIFramework
%QTBINPATH%\qmake.exe -makefile -r %SPECSTR% %REPOSPATH%\frameworks\ngi\nGIFramework\qt\nGIFramework.pro -o Makefile

%QTBINPATH%\..\..\..\Tools\QtCreator\bin\jom\jom.exe -f Makefile.release clean
%QTBINPATH%\..\..\..\Tools\QtCreator\bin\jom\jom.exe -f Makefile.release mocables all
%QTBINPATH%\..\..\..\Tools\QtCreator\bin\jom\jom.exe -f Makefile.release

echo "Build estimators"

mkdir %DEST%\build-nGIEstimators
cd %DEST%\build-nGIEstimators

%QTBINPATH%\qmake.exe -makefile -r %SPECSTR% %REPOSPATH%\frameworks\ngi\nGIEstimators\qt\nGIEstimators.pro -o Makefile
%QTBINPATH%\..\..\..\Tools\QtCreator\bin\jom\jom.exe -f Makefile.release clean
%QTBINPATH%\..\..\..\Tools\QtCreator\bin\jom\jom.exe -f Makefile.release mocables all
%QTBINPATH%\..\..\..\Tools\QtCreator\bin\jom\jom.exe -f Makefile.release

echo "Build preprocessing"
mkdir %DEST%\build-nGIPreprocessing
cd %DEST%\build-nGIPreprocessing

%QTBINPATH%\qmake.exe -makefile -r %SPECSTR% %REPOSPATH%\frameworks\ngi\nGIPreprocessing\qt\nGIPreprocessing.pro -o Makefile
%QTBINPATH%\..\..\..\Tools\QtCreator\bin\jom\jom.exe -f Makefile.release clean
%QTBINPATH%\..\..\..\Tools\QtCreator\bin\jom\jom.exe -f Makefile.release mocables all
%QTBINPATH%\..\..\..\Tools\QtCreator\bin\jom\jom.exe -f Makefile.release

popd 