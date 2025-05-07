{
  description = "A Nix-flake-based cpp development environment";

  inputs = {
    nixpkgs.url = "github:nixos/nixpkgs/nixos-unstable";
  };

  outputs =
    { self, nixpkgs, ... }:
    let
      system = "x86_64-linux";
    in
    {
      devShells."${system}".default =
        let
          pkgs = import nixpkgs {
            inherit system;
          };
        in
        pkgs.mkShell {
          packages = with pkgs; [
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

          shellHook = '''';
        };
    };
}
