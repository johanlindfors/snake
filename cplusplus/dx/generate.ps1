$path = "build"
If (!(test-path $path)) {
    New-Item -ItemType Directory -Force -Path $path
}
cd build
cmake -DCMAKE_BUILD_TYPE=Debug ../