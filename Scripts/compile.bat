cmake -H. -Bcmake-build-msvc -G "Visual Studio 15 2017" -DCMAKE_GENERATOR_PLATFORM=x64
cmake --build cmake-build-msvc
pause