fn main() {
    if cfg!(target_os = "windows") {
        println!("cargo:rustc-link-lib=shell32");
    }
}