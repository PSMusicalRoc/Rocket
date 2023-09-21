{ pkgs ? import <nixpkgs> {} }:
  pkgs.mkShell {
    nativeBuildInputs = with pkgs.buildPackages; [
      bear
      premake5
      llvmPackages_latest.lldb

      clang-tools
      llvmPackages_latest.clang

      llvmPackages_latest.libcxx
      glfw
    ];
    shellHook = ''
      export CMAKE_C_COMPILER=${pkgs.clang}/bin/clang
      export CMAKE_CXX_COMPILER=${pkgs.clang}/bin/clang++
      export PATH_CLANGD=${pkgs.clang}/bin/clangd
    '';
  }
