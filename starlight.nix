{ 
  pkgs,
  stdenv,
  lib, 

  clang-tools,
  gdb,
  jq,
  cmake,
  ninja,
  pkg-config,
  util-linux,
  sdl3,
  vulkan-headers,
  vulkan-loader,
  vk-bootstrap,
  vulkan-validation-layers,
  vulkan-memory-allocator,
  slang,
#  glslang,
  libuuid,
  buildType ? "Debug",
}:

let
  programName = "StarLight";
  BUILD_TYPE = buildType;

  WINDOWS = stdenv.targetPlatform.isWindows;
  LINUX = stdenv.targetPlatform.isLinux;

  PLATFORM = if LINUX then "Linux" else if WINDOWS then "Windows" else "";
  CMAKE_FLAGS = [
    "-DCMAKE_EXPORT_COMPILE_COMMANDS=ON"
    "-DCMAKE_BUILD_TYPE=${BUILD_TYPE}"
    "-DPLATFORM=${PLATFORM}"
  ];

  manualPaths = [
    "\${workspaceFolder}/Engine/**"
    "\${workspaceFolder}/Runtime/**"
    "\${workspaceFolder}/Editor/**"
    "\${workspaceFolder}/Testbed/**"
  ];
in
stdenv.mkDerivation {
  name = programName;
  src = ./.;
  WINDOWS = stdenv.targetPlatform.isWindows;
  LINUX = stdenv.targetPlatform.isLinux;

  buildInputs = [ sdl3 vulkan-headers vulkan-loader vulkan-validation-layers ] ++ (lib.optional LINUX libuuid);
  nativeBuildInputs = [ jq cmake ninja pkg-config util-linux ];

  preferLocalBuild = true;
  cmakeFlags = CMAKE_FLAGS;
  NIX_CFLAGS_COMPILE = if buildType == "Release" then [ "-O2" "-DNDEBUG" ] else [ "-g" "-O0" ];
  hardeningDisable = [ "all" ];
  manualPaths = manualPaths;
  PLATFORM=PLATFORM;
#$($PKG_CONFIG)
  shellHook = ''
    export VK_LAYER_PATH="${vulkan-validation-layers}/share/vulkan/explicit_layer.d"
    jq -n --arg platform $PLATFORM --arg args "$NIX_CFLAGS_COMPILE" --arg includes "$buildInputs $manualPaths" --arg cc "$(which $CC)" '{ "configurations": [{ "name": $platform, "compilerArgs": $args | split(" "), "includePath": $includes | split(" "), "compilerPath": $cc, "cStandard": "c17", "cppStandard": "c++20" }], "version": 4 }' > .vscode/c_cpp_properties.json
  '';
}