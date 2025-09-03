use suppaftp::AsyncFtpStream;

/// Connects to an FTP/FTPS server and performs a simple login.
pub async fn simple_login(host: &str, port: u16, user: &str, pass: &str) -> anyhow::Result<()> {
    let addr = format!("{host}:{port}");
    let mut ftp = AsyncFtpStream::connect(addr).await?;
    ftp.login(user, pass).await?;
    ftp.quit().await?;
    Ok(())
}
