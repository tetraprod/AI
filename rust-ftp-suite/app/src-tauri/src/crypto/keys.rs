use age::x25519::{Identity, Recipient};
use rand::rngs::OsRng;

/// Generates a new age X25519 keypair.
pub fn generate() -> (String, String) {
    let secret = Identity::generate(&mut OsRng);
    let recipient = Recipient::from(&secret);
    (secret.to_string(), recipient.to_string())
}
