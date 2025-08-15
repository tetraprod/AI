use bcrypt::{hash, verify, DEFAULT_COST};
use serde::{Deserialize, Serialize};
use std::collections::HashMap;

#[derive(Debug, Serialize, Deserialize, Clone)]
pub struct User {
    pub username: String,
    pub password_hash: String,
}

#[derive(Default)]
pub struct UserDb {
    users: HashMap<String, String>,
}

impl UserDb {
    pub fn add_user(&mut self, username: &str, password: &str) -> anyhow::Result<()> {
        let hashed = hash(password, DEFAULT_COST)?;
        self.users.insert(username.to_string(), hashed);
        Ok(())
    }

    pub fn verify(&self, username: &str, password: &str) -> bool {
        if let Some(hash) = self.users.get(username) {
            verify(password, hash).unwrap_or(false)
        } else {
            false
        }
    }
}
