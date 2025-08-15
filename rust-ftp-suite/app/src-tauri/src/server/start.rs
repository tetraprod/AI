use anyhow::Result;
use std::net::SocketAddr;

/// Starts an FTP server bound to the given address. This is a simplified
/// placeholder that demonstrates wiring with `libunftp` but doesn't run
/// a full implementation.
pub async fn start(addr: SocketAddr, root: String) -> Result<()> {
    let _server = libunftp::Server::with_fs(root);
    println!("Server would start on {addr}");
    Ok(())
}

pub async fn stop() {
    // In a full implementation this would signal the running server task.
    println!("Server stopped");
}
