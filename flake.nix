{
  description = "";  
  outputs = { self, nixpkgs } :{
    packages = {
#      aarch64-multiplatform.default = nixpkgs.legacyPackages.x86_64-linux.pkgsCross.aarch64-multiplatform.callPackage ./. {};
      x86_64-linux = {
        # calls _variant and puts output in result/<os>/<file>
        linux = nixpkgs.legacyPackages.x86_64-linux.callPackage ./linux.nix { inherit self; };
        windows = nixpkgs.legacyPackages.x86_64-linux.pkgsCross.mingwW64.callPackage ./windows.nix { inherit self; };
        all = nixpkgs.legacyPackages.x86_64-linux.callPackage ./all.nix { inherit self; };
        
        _linux = nixpkgs.legacyPackages.x86_64-linux.callPackage ./. {};
        _windows = nixpkgs.legacyPackages.x86_64-linux.pkgsCross.mingwW64.callPackage ./. {};
      };
    };
  };
}