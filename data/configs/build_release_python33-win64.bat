rem set up the msvc 64bit compiler release mode
call "c:\Program Files\Microsoft SDKs\Windows\v7.1\Bin\SetEnv.Cmd" /Release /x64 /win7

rem Python 2.7
set PATH=%PATH%;"C:\Program Files\Python\Python 2.7\x86_64"
set INCLUDE=%INCLUDE%;"C:\Pink\Build\boost_1_52_0_python\boost_1_52_0_python_2.7_x86_64"
set LIB=%LIB%;"C:\Pink\Build\boost_1_52_0_python\boost_1_52_0_python_2.7_x86_64\stage\lib"
set LIB=%LIB%;"C:\Program Files\Python\Python 2.7\x86_64\libs"
set LIBPATH="C:\Program Files\Python\Python 2.7\x86_64\libs";%LIBPATH%
set LIBPATH="C:\Pink\Build\boost_1_52_0_python\boost_1_52_0_python_2.7_x86_64\stage\lib";%LIBPATH%

mkdir release-x86_64
cd release-x86_64

call cmake --debug-trycompile -G "Visual Studio 10 Win64" -C C:\Pink\Jenkins\pink-hg\data\configs\pink_windows_config_python27_64.txt -DPINK_REVISION:STRING=OFFLINE_BUILD C:\Pink\Jenkins\pink-hg

call cmake --debug-trycompile -G "Visual Studio 10 Win64" -C C:\Pink\Jenkins\pink-hg\data\configs\pink_windows_config_python27_64.txt -DPINK_REVISION:STRING=OFFLINE_BUILD C:\Pink\Jenkins\pink-hg

call cmake --debug-trycompile -G "Visual Studio 10 Win64" -C C:\Pink\Jenkins\pink-hg\data\configs\pink_windows_config_python27_64.txt -DPINK_REVISION:STRING=OFFLINE_BUILD C:\Pink\Jenkins\pink-hg

call msbuild -p:FrameworkPathOverride="c:\Program Files\Reference Assemblies\Microsoft\Framework\v3.5" /maxcpucount:2 /verbosity:detailed /target:Build /property:Configuration=Release /property:Platform=x64 pink-image.sln

call msbuild -p:FrameworkPathOverride="c:\Program Files\Reference Assemblies\Microsoft\Framework\v3.5" /maxcpucount:2 /verbosity:detailed /target:Build /property:Configuration=Release /property:Platform=x64 PACKAGE.vcxproj

rem move pink-image-1.0.%BUILD_NUMBER%-win64.exe pink-image-1.0.%BUILD_NUMBER%-python27-win64.exe
