{ inputs, ... }:
{
	perSystem = { inputs', lib, pkgs, ... }: {
		mission-control.scripts = {
			codium = {
				description = "Open VSCodium with extensions for development";
				category = "Integrated Development Environments";
				exec = let
					vscodium-pkgs = import inputs.nixpkgs {
						system = pkgs.system;
						config.allowUnfree = true;
					};
					vscodium-with-extensions = vscodium-pkgs.vscode-with-extensions.override {
						vscode = vscodium-pkgs.vscodium;
						vscodeExtensions = with vscodium-pkgs.vscode-extensions; [
							jnoortheen.nix-ide
							arrterian.nix-env-selector
							aaron-bond.better-comments
							mkhl.direnv
							editorconfig.editorconfig
							pkief.material-icon-theme
							timonwong.shellcheck
							eamodio.gitlens
							gruntfuggly.todo-tree
							oderwat.indent-rainbow
						];
					};
				in "${vscodium-with-extensions}/bin/codium \"$FLAKE_ROOT/default.code-workspace\"";
			};
		};
	};
}
