{
  description = "Flake for C++/CMake dev with Raylib";

  inputs = {
    nixpkgs.url = "github:NixOS/nixpkgs/nixpkgs-unstable";
    flake-utils.url = "github:numtide/flake-utils";
  };

  outputs = { self, nixpkgs, flake-utils, ... }:
    flake-utils.lib.eachDefaultSystem (system:
      let
        pkgs = import nixpkgs { inherit system; };
      in {
        devShells.default = pkgs.mkShell {
          name = "cpp-raylib-dev";

          packages = with pkgs; [
            cmake
            gcc
            gdb
            gnumake
            pkg-config
            clang-tools
            cmake-language-server
            ninja
            clang
          ];

          buildInputs = with pkgs; [
            libGL
            libGLU
            xorg.libX11
            xorg.libXcursor
            xorg.libXi
            xorg.libXinerama
            xorg.libXrandr
            xorg.libXext
            xorg.libXfixes
            mesa
          ];

          shellHook = ''
            export LD_LIBRARY_PATH="${pkgs.lib.makeLibraryPath [
              pkgs.libGL
              pkgs.libGLU
              pkgs.mesa
              pkgs.xorg.libX11
              pkgs.xorg.libXcursor
              pkgs.xorg.libXext
              pkgs.xorg.libXi
              pkgs.xorg.libXinerama
              pkgs.xorg.libXrandr
            ]}:$LD_LIBRARY_PATH"
            echo "Environment ready for Raylib development";
          '';
        };
      });
}