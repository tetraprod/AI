import logging
from typing import Any, AsyncGenerator

try:  # pragma: no cover - optional dependency
    import redis.asyncio as redis
except ModuleNotFoundError:  # pragma: no cover - used in tests
    from . import _redis_stub as redis

from .network_features import NetworkFeatureManager


class OpticalEngine:
    """High-speed data processing and communication via Redis."""

    def __init__(self, redis_client: redis.Redis, feature_manager: NetworkFeatureManager) -> None:
        self.redis = redis_client
        self.features = feature_manager
        self.logger = logging.getLogger(self.__class__.__name__)

    async def initialize(self) -> None:
        """Enable important networking features."""
        for feat in [
            "optical_path_diversity_planner",
            "real_time_bfd_path_scoring",
            "smart_packet_shaping",
        ]:
            self.features.enable(feat)
        self.logger.info("OpticalEngine enabled: %s", self.features.list_enabled())

    async def transfer_data(self, data: Any, target: str) -> bool:
        """Publish data to a Redis channel with optional feature logic."""
        try:
            if "smart_packet_shaping" in self.features.enabled:
                self.logger.debug("Applying smart packet shaping")
            await self.redis.publish(target, str(data))
            return True
        except Exception as exc:
            self.logger.error("Publish failed: %s", exc)
            return False

    async def subscribe(self, channel: str) -> AsyncGenerator[str, None]:
        pubsub = self.redis.pubsub()
        await pubsub.subscribe(channel)
        try:
            async for item in pubsub.listen():
                if item.get("type") == "message":
                    yield item.get("data")
        finally:
            await pubsub.unsubscribe(channel)

    async def process_message(self, message: str) -> None:
        """Placeholder for future message handling."""
        self.logger.info("Received message: %s", message)
