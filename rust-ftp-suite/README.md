# Rust FTP Suite

Rust-based desktop FTP/FTPS client and server with optional end-to-end encryption. Built with [Tauri](https://tauri.app/) for a lightweight Windows executable.

## Features
- Integrated FTP/FTPS server using `libunftp`.
- FTP/FTPS client powered by `suppaftp`.
- Optional end-to-end file encryption using the `age` crate.
- QR code generation for shareable links.
- Configuration stored under `%APPDATA%\RustFTP`.

## Building
```bash
cargo tauri build
```

For development on Linux, required system packages include a modern webkit2gtk. On Windows, install the Rust toolchain and Node.js then run the same command.

## Running Tests
```bash
cargo test
```
