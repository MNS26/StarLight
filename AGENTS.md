# StarLight Development Guide

## Project Overview

StarLight is a C++ Vulkan/SDL3 application built with CMake and Ninja. The project uses Nix flakes for declarative development environments.

## NixOS Development Environment

### Entering the Development Shell

```bash
nix develop
```

This provides a reproducible development environment with all necessary tools.

### Available Packages in devShell

#### Build Tools
- `cmake` - CMake build system
- `ninja` - Ninja build system
- `pkg-config` - Package configuration tool

#### C++ Toolchain
- `clang-tools` - Clang tools including clangd (LSP server)
- `gdb` - Debugger

#### Vulkan/Graphics Development
- `vulkan-tools` - Vulkan tools (vulkaninfo, etc.)
- `vulkan-validation-layers` - Vulkan validation layers

#### Language Servers
- `nil` - Nix language server (linter/formatter)
- `cmake-language-server` - CMake LSP

#### Shell/Terminal
- `ksh` - KornShell for scripting
- `bashInteractive` - Interactive bash
- `shellcheck` - Shell script linter

#### Utilities
- `git` - Version control
- `fira-code` - Font with ligatures
- `nano` - CLI text editor
- `openssl` - SSL/TLS toolkit
- `perf` - Performance analysis tools
- `opencode` - AI coding assistant
- `ungoogled-chromium` - Web browser

## Editor Integrations

The project includes pre-configured editor integrations via `mission-control` scripts. Run these from the project root using the comma prefix:

### VSCodium

```bash
nix develop --command , codium
```

Opens VSCodium with pre-bundled extensions:
- Nix IDE (nix-ide, nix-env-selector)
- Better Comments
- direnv
- EditorConfig
- Material Icon Theme
- ShellCheck
- GitLens
- TODO Tree
- Indent Rainbow

### Neovim

```bash
nix develop --command , vim
```

Opens Neovim for development.

### Zed Editor

```bash
nix develop --command , zed
```

Opens Zed Editor (FHS variant) with auto-installing extensions configured in `.zed/settings.json`:
- Nix, Rust, Clangd, C/C++, Python, Shell, Docker, TOML, JSON, YAML, Markdown, Vim, Lua, Git

The Zed extensions are automatically installed on first launch via the `auto_install_extensions` setting.

## Task Scripts

The project uses `mission-control` for task automation. Available commands:

```bash
# List all available scripts
mc --list

# Run specific tasks
mc codium    # Open VSCodium
mc vim       # Open Neovim
mc zed       # Open Zed Editor
```

### Available Task Categories

- **Editors** - IDE integrations (codium, vim, zed)
- **Checks** - Code verification tasks
- **Docs** - Documentation generation
- **Release** - Release management
- **Tools** - Development tools

## Development Workflow

### Building the Project

```bash
# Enter dev shell
nix develop

# Build
cmake -B build
cmake --build build
```

### Code Formatting

```bash
# Format Nix files
nix fmt

# Format via editor (automatic on save in VSCodium/Neovim)
```

### Debugging

The project generates `compile_commands.json` for IDE support:
```bash
cmake -B build -DCMAKE_EXPORT_COMPILE_COMMANDS=ON
```

## Legacy Migration Notes

The following legacy files have been removed:
- `flake.nix.old` - Replaced by modern `flake.nix`
- `shell.nix` - Integrated into flake devShell
- `linux.nix` - Build via default.nix derivation
- `windows.nix` - Cross-compile via pkgsCross.mingwW64
- `all.nix` - Combined output now in flake outputs

## Cross-Platform Builds

The project supports both Linux and Windows builds:

```bash
# Linux
nix build .#packages.x86_64-linux.default

# Windows (cross-compile)
nix build .#packages.x86_64-linux.windows
```

## Configuration Files

- `flake.nix` - Main flake configuration
- `default.nix` - Package derivation
- `default.code-workspace` - VSCodium workspace settings
- `tasks/` - Mission-control task definitions

## Notes

- The devShell is desktop-agnostic (works with GTK, Qt, or Wayland)
- VSCodium uses FHS environment for extension compatibility
- Zed Editor uses FHS variant for extension support
