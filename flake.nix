{
	description = "StarLight - C++ Vulkan/SDL3 Application";

	inputs = {
		# Release inputs
			nixpkgs-master.url = "github:nixos/nixpkgs/master";
			nixpkgs-staging-next.url = "github:nixos/nixpkgs/staging-next";
			nixpkgs-staging.url = "github:nixos/nixpkgs/staging";
			nixpkgs-unstable.url = "github:nixos/nixpkgs/nixos-unstable";

			nixpkgs.url = "github:nixos/nixpkgs/nixos-25.11";
			# nixpkgs.url = "git+file:///nix/persist/NiXium/vendor/nixpkgs-stable";

			nixpkgs-25_11.url = "github:nixos/nixpkgs/nixos-25.11";
			nixpkgs-25_05.url = "github:nixos/nixpkgs/nixos-25.05";
			nixpkgs-24_11.url = "github:nixos/nixpkgs/nixos-24.11";
			nixpkgs-24_05.url = "github:nixos/nixpkgs/nixos-24.05";
			nixpkgs-23_11.url = "github:nixos/nixpkgs/nixos-23.11";
			nixpkgs-23_05.url = "github:nixos/nixpkgs/nixos-23.05";
			nixpkgs-22_11.url = "github:nixos/nixpkgs/nixos-22.11";
			nixpkgs-22_05.url = "github:nixos/nixpkgs/nixos-22.05";

		# Principle inputs
			nixos-flake.url = "github:srid/nixos-flake";
			flake-parts.url = "github:hercules-ci/flake-parts";
			mission-control.url = "github:Platonic-Systems/mission-control";

			flake-root.url = "github:srid/flake-root";
	};

	outputs = inputs @ { self, ... }:
		inputs.flake-parts.lib.mkFlake { inherit inputs; } {
			imports = [
				./tasks # Include Tasks

				inputs.flake-root.flakeModule
				inputs.mission-control.flakeModule
			];

			# Set Supported Systems
			systems = [
				"x86_64-linux"
				"aarch64-linux"
				"riscv64-linux"
				"armv7l-linux"
			];

	perSystem = { system, config, inputs', ... }: {
			devShells.default = inputs.nixpkgs.legacyPackages.${system}.mkShell {
					name = "StarLight-devshell";
					nativeBuildInputs = [
						# Shell
						inputs.nixpkgs.legacyPackages.${system}.ksh # For Scripting
						inputs.nixpkgs.legacyPackages.${system}.bashInteractive # For terminal
						inputs.nixpkgs.legacyPackages.${system}.shellcheck # Linting of shell files

						# Nix
						inputs.nixpkgs.legacyPackages.${system}.nil # Needed for linting
						inputs.nixpkgs.legacyPackages.${system}.nixpkgs-fmt # Nixpkgs formatter

						# Build tools
						inputs.nixpkgs.legacyPackages.${system}.cmake # CMake build system
						inputs.nixpkgs.legacyPackages.${system}.ninja # Ninja build system
						inputs.nixpkgs.legacyPackages.${system}.pkg-config # Package config

						# C++ toolchain
						inputs.nixpkgs.legacyPackages.${system}.clang-tools # Clang tools (clangd)
						inputs.nixpkgs.legacyPackages.${system}.gdb # Debugger

						# Vulkan/Graphics development
						inputs.nixpkgs.legacyPackages.${system}.vulkan-tools # Vulkan tools
						inputs.nixpkgs.legacyPackages.${system}.vulkan-validation-layers # Vulkan validation

						# CMake Language Server
						inputs.nixpkgs.legacyPackages.${system}.cmake-language-server # CMake LSP

						# Benchmarking
						inputs.nixpkgs.legacyPackages.${system}.perf

						# Utilities
						inputs.nixpkgs.legacyPackages.${system}.git # Working with the codebase
						inputs.nixpkgs.legacyPackages.${system}.fira-code # For ligatures in code editors
						inputs.nixpkgs.legacyPackages.${system}.nano # Editor to work with the codebase in cli
						inputs.nixpkgs.legacyPackages.${system}.openssl

						inputs.nixpkgs.legacyPackages.${system}.ungoogled-chromium # Web browser used in the integrated developer environment for interacting with the outside resources

						inputs.nixpkgs.legacyPackages.${system}.opencode
					];
					inputsFrom = [
						config.mission-control.devShell
						config.flake-root.devShell
					];
				};

				formatter = inputs.nixpkgs.legacyPackages.${system}.nixpkgs-fmt;
			};
		};
}
