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

# Linux only
    NativeBuildInputs = with pkgs; [
      cmake
      ninja
      pkg-config
      util-linux
    ];

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
in
stdenv.mkDerivation {
  name = programName;
  src = ./.;
  buildInputs = commonLibraties;
  nativeBuildInputs =linuxLibraries++NativeBuildInputs;
  preferLocalBuild = true;
  WINDOWS = stdenv.targetPlatform.isWindows;
  cmakeFlags = CMAKE_FLAGS;
}