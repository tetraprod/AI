use crate::{client::ftp_client, crypto::{e2e, keys}, server::start};
use std::net::SocketAddr;

#[tauri::command]
pub async fn start_server(port: u16, root: String) -> Result<(), String> {
    let addr: SocketAddr = format!("0.0.0.0:{port}").parse().map_err(|e| e.to_string())?;
    start::start(addr, root).await.map_err(|e| e.to_string())
}

#[tauri::command]
pub async fn stop_server() {
    start::stop().await;
}

#[tauri::command]
pub async fn client_connect(host: String, port: u16, user: String, pass: String) -> Result<(), String> {
    ftp_client::simple_login(&host, port, &user, &pass).await.map_err(|e| e.to_string())
}

#[tauri::command]
pub fn encrypt_example(text: String) -> Result<Vec<u8>, String> {
    let (_secret, recip) = keys::generate();
    e2e::encrypt_bytes(text.as_bytes(), &[recip]).map_err(|e| e.to_string())
}
