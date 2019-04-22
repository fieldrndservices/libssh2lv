mkdir build32 & pushd build32
cmake -G "Visual Studio 16 2019" -A Win32 ..
cmake --build . --config Release > build.log
popd
mkdir build64 & pushd build64
cmake -G "Visual Studio 16 2019" -A x64 ..
cmake --build . --config Release > build.log
popd
