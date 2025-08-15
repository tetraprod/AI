#![cfg_attr(not(debug_assertions), windows_subsystem = "windows")]

mod client;
mod config;
mod crypto;
mod links;
mod logging;
mod server;
mod transfers;
mod ui_bridge;

fn main() {
    logging::init_logging().expect("logging");
    tauri::Builder::default()
        .invoke_handler(tauri::generate_handler![
            ui_bridge::start_server,
            ui_bridge::stop_server,
            ui_bridge::client_connect,
            ui_bridge::encrypt_example
        ])
        .run(tauri::generate_context!())
        .expect("error while running tauri application");
}
