import asyncio
import logging
from contextlib import asynccontextmanager
from typing import Any, AsyncGenerator, Optional

try:  # pragma: no cover - optional dependency
    import redis.asyncio as redis
except ModuleNotFoundError:  # pragma: no cover - used in tests
    from . import _redis_stub as redis

from .soul import SoulEngine
from .brain import BrainEngine
from .optical import OpticalEngine
from .aura import AuraEngine
from .speech import SpeechEngine
from .network_features import NetworkFeatureManager, NETWORK_FEATURES
from .replicator import SystemReplicator


class UnifiedAI:
    """Central orchestrator coordinating all engines."""

    def __init__(self, redis_url: str = "redis://localhost:6379/0") -> None:
        self.feature_manager = NetworkFeatureManager()
        self.redis_url = redis_url
        self.redis = redis.from_url(redis_url, decode_responses=True)
        self.soul = SoulEngine()
        self.brain = BrainEngine()
        self.optical = OpticalEngine(self.redis, self.feature_manager)
        self.aura = AuraEngine()
        self.speech = SpeechEngine(self.optical)
        self.logger = logging.getLogger(self.__class__.__name__)
        self.bg_tasks: list[asyncio.Task] = []

    async def connect(self) -> None:
        """Establish the Redis connection."""
        self.redis = redis.from_url(self.redis_url, decode_responses=True)

    async def initialize(self) -> None:
        await self.brain.initialize()
        await self.optical.initialize()
        self.bg_tasks.append(asyncio.create_task(self._listener()))

    async def close(self) -> None:
        for task in self.bg_tasks:
            task.cancel()
        if self.redis:
            await self.redis.close()
        await self.brain.close()

    async def enable_feature(self, feature: str) -> None:
        self.feature_manager.enable(feature)

    def list_enabled_features(self) -> list[str]:
        return self.feature_manager.list_enabled()

    async def interact(self, text: str) -> str:
        if not await self.aura.validate_output(text):
            raise ValueError("Inappropriate content")
        emotion = await self.soul.analyze_emotion(text)
        memory_response = await self.brain.reason(text)
        await self.optical.transfer_data(memory_response, "UnifiedAI")
        if not await self.aura.validate_output(memory_response):
            return "Output blocked due to ethics rules"
        reply = await self.soul.craft_reply(memory_response, emotion)
        analysis = await self.speech.analyze_text(reply)
        speech_data = await self.speech.synthesize_speech(analysis)
        await self.speech.transmit_speech(speech_data)
        return reply

    async def _listener(self) -> None:
        async for message in self.optical.subscribe("UnifiedAI"):
            await self.optical.process_message(message)


@asynccontextmanager
async def lifespan(
    app: Optional[Any] = None, engine: Optional[UnifiedAI] = None
) -> AsyncGenerator[UnifiedAI, None]:
    engine = engine or UnifiedAI()
    await engine.connect()
    await engine.initialize()
    try:
        yield engine
    finally:
        await engine.close()

__all__ = ["UnifiedAI", "lifespan", "SystemReplicator"]
