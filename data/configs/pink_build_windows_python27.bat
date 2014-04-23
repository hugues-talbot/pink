rem set up the msvc 64bit compiler
call

rem JOM
set PATH=%PATH%;C:\Program Files\jom_1_0_11

rem Python 2.7
set PATH=%PATH%;D:\Build\Python27_64
set INCLUDE=%INCLUDE%;D:\Build\boost_1_49_0\boost_python27\boost_1_49_0
set LIB=%LIB%;D:\Build\boost_1_49_0\boost_python27\boost_1_49_0\stage\lib
set LIB=%LIB%;D:\Build\Python27_64\libs

cmake -G "NMake Makefiles JOM" -C C:\Pink\Source\Pink-hg\data\configs\pink_windows_config.txt C:\Pink\Source\Pink-hg
cmake -G "NMake Makefiles JOM" -C C:\Pink\Source\Pink-hg\data\configs\pink_windows_config.txt C:\Pink\Source\Pink-hg
cmake -G "NMake Makefiles JOM" -C C:\Pink\Source\Pink-hg\data\configs\pink_windows_config.txt C:\Pink\Source\Pink-hg

rem nmake
jom -j4
jom -j4 source_package
jom -j4 package
