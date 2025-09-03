use age::{Decryptor, Encryptor};
use age::x25519::Identity;
use anyhow::{bail, Result};
use std::io::{Read, Write};
use std::str::FromStr;

/// Encrypts data for the given recipients (in age string format).
pub fn encrypt_bytes(data: &[u8], recipients: &[String]) -> Result<Vec<u8>> {
    let recps = recipients
        .iter()
        .map(|r| r.parse().map_err(|e| anyhow::anyhow!(e)))
        .collect::<Result<Vec<_>>>()?;
    let encryptor = Encryptor::with_recipients(recps);
    let mut out = vec![];
    let mut writer = encryptor.wrap_output(&mut out)?;
    writer.write_all(data)?;
    writer.finish()?;
    Ok(out)
}

/// Decrypts data using the given secret key string.
pub fn decrypt_bytes(cipher: &[u8], secret: &str) -> Result<Vec<u8>> {
    let decryptor = Decryptor::new(cipher)?;
    let mut out = vec![];
    match decryptor {
        Decryptor::Recipients(d) => {
            let id = Identity::from_str(secret)?;
            let mut reader = d.decrypt(&[id][..])?;
            reader.read_to_end(&mut out)?;
        }
        _ => bail!("passphrase decryption not supported"),
    }
    Ok(out)
}

#[cfg(test)]
mod tests {
    use super::*;
    use crate::crypto::keys;

    #[test]
    fn roundtrip_encrypt_decrypt() {
        let (secret, recipient) = keys::generate();
        let data = b"hello world";
        let cipher = encrypt_bytes(data, &[recipient]).unwrap();
        let plain = decrypt_bytes(&cipher, &secret).unwrap();
        assert_eq!(plain, data);
    }
}
