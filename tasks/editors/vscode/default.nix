{ inputs, ... }:
{
	perSystem = { inputs', lib, pkgs, ... }: {
		mission-control.scripts = {
			vscode = {
				description = "Open VSCode with extensions for development";
				category = "Integrated Development Environments";
				exec = let
					vscode-pkgs = import inputs.nixpkgs {
						system = pkgs.system;
						config.allowUnfree = true;
					};
					vscode-with-extensions = vscode-pkgs.vscode-with-extensions.override {
						vscodeExtensions = with vscode-pkgs.vscode-extensions; [
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
				in "${vscode-with-extensions}/bin/code \"$FLAKE_ROOT/default.code-workspace\"";
			};
		};
	};
}
