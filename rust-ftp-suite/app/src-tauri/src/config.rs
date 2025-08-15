use directories::ProjectDirs;
use serde::{Deserialize, Serialize};
use std::fs;
use std::path::PathBuf;

const CONFIG_NAME: &str = "config.toml";

#[derive(Debug, Serialize, Deserialize)]
pub struct AppConfig {
    pub port: u16,
    pub upnp: bool,
}

impl Default for AppConfig {
    fn default() -> Self {
        Self { port: 2121, upnp: true }
    }
}

impl AppConfig {
    pub fn load() -> Self {
        if let Some(path) = Self::config_path() {
            if let Ok(contents) = fs::read_to_string(&path) {
                if let Ok(cfg) = toml::from_str(&contents) {
                    return cfg;
                }
            }
        }
        Self::default()
    }

    pub fn save(&self) -> anyhow::Result<()> {
        if let Some(path) = Self::config_path() {
            if let Some(parent) = path.parent() { fs::create_dir_all(parent)?; }
            fs::write(path, toml::to_string_pretty(self)?)?;
        }
        Ok(())
    }

    fn config_path() -> Option<PathBuf> {
        ProjectDirs::from("com", "example", "RustFTP")
            .map(|d| d.config_dir().join(CONFIG_NAME))
    }
}
