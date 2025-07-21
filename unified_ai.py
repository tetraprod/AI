import redis.asyncio as redis
from contextlib import asynccontextmanager
from typing import Optional, AsyncGenerator, Any


class UnifiedAI:
    """Simple AI engine using Redis for storage."""

    def __init__(self, redis_url: str = "redis://localhost:6379/0") -> None:
        self.redis_url = redis_url
        self.redis: Optional[redis.Redis] = None

    async def connect(self) -> None:
        """Establish the Redis connection."""
        self.redis = redis.from_url(self.redis_url, decode_responses=True)

    async def close(self) -> None:
        """Close the Redis connection and cleanup."""
        if self.redis:
            await self.redis.close()
            await self.redis.connection_pool.disconnect()
            self.redis = None


@asynccontextmanager
async def lifespan(
    app: Optional[Any] = None, engine: Optional[UnifiedAI] = None
) -> AsyncGenerator[UnifiedAI, None]:
    """Application lifespan context managing the engine."""
    engine = engine or UnifiedAI()
    await engine.connect()
    try:
        yield engine
    finally:
        await engine.close()
