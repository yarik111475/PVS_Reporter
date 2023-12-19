@echo off

::create current dir variable
SET CURRENT_DIR=%CD%
echo %CURRENT_DIR%

::set dirs variables
SET BUILD_DIR=%CURRENT_DIR%\build_x32
SET INSTALL_DIR=%CURRENT_DIR%\install_x32
SET DEPLOY_DIR=%CURRENT_DIR%\deploy_x32

::remove old dirs
rmdir /Q /S %BUILD_DIR%
rmdir /Q /S %INSTALL_DIR%
rmdir /Q /S %DEPLOY_DIR%

::create new dirs
mkdir %BUILD_DIR%
mkdir %INSTALL_DIR%
mkdir %DEPLOY_DIR%

::create qt/cmake/make variables
SET Qt5_DIR="C:/Qt/Qt5.14.2/5.14.2/mingw73_32/lib"
SET CMAKE_PREFIX_PATH="C:/Qt/Qt5.14.2/5.14.2/mingw73_32"
SET CMAKE_PATH="C:/Program Files/CMake/bin/cmake.exe"
SET MAKE_PATH="C:/Qt/Ninja/ninja.exe"

::vs options
SET CMAKE_C_COMPILER="C:/Qt/Qt5.14.2/Tools/mingw730_32/bin/gcc.exe"
SET CMAKE_CXX_COMPILER="C:/Qt/Qt5.14.2/Tools/mingw730_32/bin/g++.exe"
SET QTENV2_PATH="C:/Qt/Qt5.14.2/5.14.2/mingw73_32/bin/qtenv2.bat"

::call vs script
call %QTENV2_PATH%

::change dir to 'build'
cd %BUILD_DIR%

::run cmake
%CMAKE_PATH% ../../../ -GNinja^
 -DCMAKE_BUILD_TYPE:String=Release^
 -DCMAKE_INSTALL_PREFIX=%INSTALL_DIR%^
 -DQt5_DIR=%Qt5_DIR%^
 -DCMAKE_MAKE_PROGRAM=%MAKE_PATH%^
 -DCMAKE_PREFIX_PATH:STRING=%CMAKE_PREFIX_PATH%^
 -DCMAKE_C_COMPILER:STRING=%CMAKE_C_COMPILER%^
 -DCMAKE_CXX_COMPILER:STRING=%CMAKE_CXX_COMPILER%

%CMAKE_PATH% --build . --parallel --target all
%CMAKE_PATH% --install .

::deployer options
SET CQTDEPLOYER_DIR="C:/Qt/CQtDeployer/1.5"
SET QMAKE_PATH="C:/Qt/Qt5.14.2/5.14.2/mingw73_32/bin/qmake.exe"

::deploy
call %CQTDEPLOYER_DIR%\cqtdeployer.bat -qmake %QMAKE_PATH% -bin %INSTALL_DIR%\bin\PVS_Reporter.exe -targetDir %DEPLOY_DIR%\bin noTranslations
call %CQTDEPLOYER_DIR%\cqtdeployer.bat -qmake %QMAKE_PATH% -bin %INSTALL_DIR%\bin\libqtcsv.dll -targetDir %DEPLOY_DIR%\bin noTranslations

::cd to -up- dir
cd ..

::remove 'build' and 'install' dirs
rmdir /Q /S %BUILD_DIR%
rmdir /Q /S %INSTALL_DIR%

pause
::exit
