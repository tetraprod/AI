use igd::aio::search_gateway;
use std::net::{IpAddr, Ipv4Addr, SocketAddrV4};

pub async fn map_port(port: u16) -> anyhow::Result<Option<IpAddr>> {
    if let Ok(gateway) = search_gateway(Default::default()).await {
        let local = SocketAddrV4::new(Ipv4Addr::UNSPECIFIED, port);
        gateway.add_port(igd::PortMappingProtocol::TCP, port, local, 0, "rustftpsuite").await?;
        if let Ok(ext) = gateway.get_external_ip().await { return Ok(Some(ext.into())); }
    }
    Ok(None)
}
