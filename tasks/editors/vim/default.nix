{ ... }:
{
	perSystem = { inputs', lib, pkgs, config, ... }: {
		mission-control.scripts = {
			vim = {
				description = "Open Neovim for development";
				category = "Integrated Development Environments";
				exec = "${pkgs.neovim}/bin/nvim .";
			};
		};
	};
}
