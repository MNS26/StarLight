{ inputs, ... }:
{
	perSystem = { inputs', lib, pkgs, ... }: {
		mission-control.scripts = {
			zed = {
				description = "Open Zed Editor (FHS) for development with auto-installed extensions";
				category = "Integrated Development Environments";
				exec = let
					zed-pkgs = import inputs.nixpkgs {
						system = pkgs.system;
						config.allowUnfree = true;
					};
				in "${zed-pkgs.zed-editor.fhs}/bin/zeditor \"$FLAKE_ROOT\"";
			};
		};
	};
}
