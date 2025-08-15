use rcgen::generate_simple_self_signed;
use rustls::{Certificate, PrivateKey};

pub fn generate_self_signed() -> anyhow::Result<(Certificate, PrivateKey)> {
    let cert = generate_simple_self_signed(["localhost".into()])?;
    let cert_der = cert.serialize_der()?;
    let priv_key = cert.serialize_private_key_der();
    Ok((Certificate(cert_der), PrivateKey(priv_key)))
}
