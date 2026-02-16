{ 
  pkgs,
  stdenv,
  lib, 
  util-linux,
}:

let
  programName = "StarLight";
  BUILD_TYPE = "RelWithDebug";
  CMAKE_FLAGS = [
    "-DCMAKE_EXPORT_COMPILE_COMMANDS=ON"
    "-DCMAKE_BUILD_TYPE=${BUILD_TYPE}"
  ];

  Toolchain = with pkgs; [
    clang-tools
    gdb
  ];
  nativeBuildInputs = with pkgs; [
    jq
    cmake
    ninja
    pkg-config
    util-linux
  ]++Toolchain;

# Linux only
  linuxLibraries = with pkgs; [
    vk-bootstrap
    vulkan-tools
    vulkan-extension-layer
    vulkan-validation-layers
    vulkan-memory-allocator
    libuuid
  ];


  #both cross-compiler and native them
  commonLibraties = with pkgs; [
    sdl3
    vulkan-loader
    vulkan-headers

  ];
  buildInputs = commonLibraties;
in
stdenv.mkDerivation {
  name = programName;
  src = ./.;
  buildInputs = commonLibraties;
  nativeBuildInputs = nativeBuildInputs++linuxLibraries;
#  nativeBuildInputs =linuxLibraries++nativeBuildInputs;
  preferLocalBuild = true;
  WINDOWS = stdenv.targetPlatform.isWindows;
  cmakeFlags = CMAKE_FLAGS;

  preConfigure = ''
  pwd
  ls -lh Engine Editor Runtime
  '';

#$(PKG_CONFIG)
  shellHook = ''
    jq -n --arg args "$NIX_CFLAGS_COMPILE" --arg includes "$buildInputs" --arg cc "$(which $CC)" '{ "configurations": [{ "name": "Linux", "compilerArgs": $args | split(" "), "includePath": $includes | split(" "), "compilerPath": $cc, "cStandard": "c17", "cppStandard": "c++20" }], "version": 4 }' > .vscode/c_cpp_properties.json
  '';

}