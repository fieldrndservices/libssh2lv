mkdir build32 & pushd build32
cmake -G "Visual Studio 16 2019" ..
cmake --build . --config Release > build.log
popd
mkdir build64 & pushd build64
cmake -G "Visual Studio 16 2019 Win64" ..
cmake --build . --config Release > build.log
popd
