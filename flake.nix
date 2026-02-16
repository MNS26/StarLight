{
  description = "";
  inputs.self.submodules = true;
  outputs = { self, nixpkgs }:
  let


    os = nixpkgs.legacyPackages.x86_64-linux.callPackage ./os.nix { inherit self; };

  
  in
  {

    packages = {
#      aarch64-multiplatform.default = nixpkgs.legacyPackages.x86_64-linux.pkgsCross.aarch64-multiplatform.callPackage ./. {};
      x86_64-linux = {
        linux = os.linux;
        windows = os.windows;
        all = os.linux.all;

#        # calls _variant and puts output in result/<os>/<file>        
        _linux = nixpkgs.legacyPackages.x86_64-linux.callPackage ./. {};
        _windows = nixpkgs.legacyPackages.x86_64-linux.pkgsCross.mingwW64.callPackage ./. {};
      };
    };
  };
}