{
  description = "";
  inputs.self.submodules = true;
  outputs = { self, nixpkgs }:
  let
    os = nixpkgs.legacyPackages.x86_64-linux.callPackage ./os.nix { inherit self; };
  in
  {
    packages = {
      x86_64-linux = {
        dev = {
          windows = self.packages.x86_64-linux._windows-debug;
          linux = self.packages.x86_64-linux._linux-debug;
          arm = self.packages.x86_64-linux._arm-debug;
        };
        debug = {
          linux = os.linux-debug;
          arm = os.arm-debug;
          windows = os.windows-debug;
          all = os.all-debug;
        };
        release = {
          linux = os.linux-release;
          arm = os.arm-release;
          windows = os.windows-release;
          all = os.all-release;
        };

        default = self.packages.x86_64-linux._linux-debug;
        linux = os.linux;
        windows = os.windows;
        arm = os.arm;
        all = os.all;

#        # calls _variant and puts output in result/<os>/<file>        
        _linux-debug = nixpkgs.legacyPackages.x86_64-linux.callPackage ./starlight.nix { buildType = "Debug"; };
        _linux-release = nixpkgs.legacyPackages.x86_64-linux.callPackage ./starlight.nix { buildType = "Release"; };
        _windows-debug = nixpkgs.legacyPackages.x86_64-linux.pkgsCross.mingwW64.callPackage ./starlight.nix { buildType = "Debug"; };
        _windows-release = nixpkgs.legacyPackages.x86_64-linux.pkgsCross.mingwW64.callPackage ./starlight.nix { buildType = "Release"; };
        _arm-debug = nixpkgs.legacyPackages.x86_64-linux.pkgsCross.aarch64-multiplatform.callPackage ./starlight.nix { buildType = "Debug"; };
        _arm-release = nixpkgs.legacyPackages.x86_64-linux.pkgsCross.aarch64-multiplatform.callPackage ./starlight.nix { buildType = "Release"; };
      };
    };
  };
}
