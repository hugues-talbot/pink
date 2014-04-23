rem set up the msvc 32bit compiler release mode
call "c:\Program Files\Microsoft SDKs\Windows\v7.1\Bin\SetEnv.Cmd" /Release /x86 /win7

rem Python 3.3
set PATH=%PATH%;"C:\Program Files (x86)\Python\Python 3.3\x86"
set INCLUDE=%INCLUDE%;"C:\Pink\Build\boost_1_52_0_python\boost_1_52_0_python_3.3_x86"
set LIB=%LIB%;"C:\Pink\Build\boost_1_52_0_python\boost_1_52_0_python_3.3_x86\stage\lib"
set LIB=%LIB%;"C:\Program Files (x86)\Python\Python 3.3\x86\libs"
set LIBPATH="C:\Program Files (x86)\Python\Python 3.3\x86\libs";%LIBPATH%
set LIBPATH="C:\Pink\Build\boost_1_52_0_python\boost_1_52_0_python_3.3_x86\stage\lib";%LIBPATH%

mkdir release-x86_32
cd release-x86_32

call cmake --debug-trycompile -G "Visual Studio 10" -C C:\Pink\Jenkins\pink-hg\data\configs\pink_windows_config_python33_32.txt -DPINK_REVISION:STRING=OFFLINE_BUILD C:\Pink\Jenkins\pink-hg

call cmake --debug-trycompile -G "Visual Studio 10" -C C:\Pink\Jenkins\pink-hg\data\configs\pink_windows_config_python33_32.txt -DPINK_REVISION:STRING=OFFLINE_BUILD C:\Pink\Jenkins\pink-hg

call cmake --debug-trycompile -G "Visual Studio 10" -C C:\Pink\Jenkins\pink-hg\data\configs\pink_windows_config_python33_32.txt -DPINK_REVISION:STRING=OFFLINE_BUILD C:\Pink\Jenkins\pink-hg

call msbuild -p:FrameworkPathOverride="c:\Program Files\Reference Assemblies\Microsoft\Framework\v3.5" /maxcpucount:2 /verbosity:detailed /target:Build /property:Configuration=Release /property:Platform=Win32 pink-image.sln

call msbuild -p:FrameworkPathOverride="c:\Program Files\Reference Assemblies\Microsoft\Framework\v3.5" /maxcpucount:2 /verbosity:detailed /target:Build /property:Configuration=Release /property:Platform=Win32 PACKAGE.vcxproj

rem move pink-image-1.0.%BUILD_NUMBER%-win32.exe pink-image-1.0.%BUILD_NUMBER%-python33-win32.exe
