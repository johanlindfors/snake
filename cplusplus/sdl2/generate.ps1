$path = "build"
If(!(test-path $path))
{
      New-Item -ItemType Directory -Force -Path $path
}
cd build
cmake -DCMAKE_TOOLCHAIN_FILE="D:\\repos\\thirdparty\\vcpkg\\scripts\\buildsystems\\vcpkg.cmake" -DCMAKE_BUILD_TYPE=Debug ../