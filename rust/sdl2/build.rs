fn main() {
    use std::env;
    match env::var("CARGO_CFG_windows") {
        Ok(_s) => println!("cargo:rustc-link-lib=shell32"),
        Err(_err) => println!(""),
    }
}