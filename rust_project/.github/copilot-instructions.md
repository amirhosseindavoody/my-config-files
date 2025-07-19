# Copilot Instructions for UPF Parser

<!-- Use this file to provide workspace-specific custom instructions to Copilot. For more details, visit https://code.visualstudio.com/docs/copilot/copilot-customization#_use-a-githubcopilotinstructionsmd-file -->

## Project Context

Example rust project to setup toolchain.

## Toolchain
- Use a locally installed version of rust and cargo that is managed by pixi.
- Instead of calling `cargo` directly, use `pixi` package manager to invoke the local version, so instead of `cargo build`, use `pixi run cargo build`.
- Use `pixi run cargo` to run any cargo command, such as `pixi run cargo test`.