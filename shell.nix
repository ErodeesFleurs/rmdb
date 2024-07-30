with import <nixpkgs> {};

let mcc-env = (callPackage "/home/fleurs/Github/mini_compile_commands/" {}).wrap stdenv;

in (mkShell.override {stdenv = mcc-env;}) {
    buildInputs = [
        gcc14
        gtest
        gnumake
        bison
        flex
        readline
        cmake
        python3
        bintools
        gdb
    ];
}
