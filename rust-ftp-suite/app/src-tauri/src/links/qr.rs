use qrcode::QrCode;
use qrcode::render::unicode;

/// Returns a string containing a textual QR code representation.
pub fn qr_to_string(data: &str) -> anyhow::Result<String> {
    let code = QrCode::new(data.as_bytes())?;
    let string = code.render::<unicode::Dense1x2>().build();
    Ok(string)
}
