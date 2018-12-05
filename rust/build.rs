fn main() {
    println!("cargo:rustc-link-lib=static=sdl2");
    println!("cargo:root=c:\\repos\\thirdparty\\vcpkg\\installed\\x64-windows");
    println!("cargo:rustc-link-search=native=c:\\repos\\thirdparty\\vcpkg\\installed\\x64-windows\\lib");
    println!("cargo:rustc-link-search=native=c:\\repos\\thirdparty\\vcpkg\\installed\\x64-windows\\bin");
}