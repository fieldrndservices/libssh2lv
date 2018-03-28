mkdir build32 & pushd build32
cmake -G "Visual Studio 15 2017" ..
cmake --build . --config Release > build.log
popd
mkdir build64 & pushd build64
cmake -G "Visual Studio 15 2017 Win64" ..
cmake --build . --config Release > build.log
popd
