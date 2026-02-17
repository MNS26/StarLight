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
          windows = self.packages.x86_64-linux._windows;
          linux = self.packages.x86_64-linux._linux;
          arm = self.packages.x86_64-linux._arm;
        };
        default = self.packages.x86_64-linux._linux;
        linux = os.linux;
        windows = os.windows;
        arm = os.arm;
        all = os.linux.all;

#        # calls _variant and puts output in result/<os>/<file>        
        _linux = nixpkgs.legacyPackages.x86_64-linux.callPackage ./. {};
        _windows = nixpkgs.legacyPackages.x86_64-linux.pkgsCross.mingwW64.callPackage ./. {};
        _arm = nixpkgs.legacyPackages.x86_64-linux.pkgsCross.aarch64-multiplatform.callPackage ./. {};
      };
    };
  };
}