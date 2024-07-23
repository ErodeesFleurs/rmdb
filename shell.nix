 with import <nixpkgs> {};

mkShell {
    buildInputs = [
        gcc14
        gnumake
        bison
        flex
        readline
        cmake
    ];
}
