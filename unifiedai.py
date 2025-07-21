import asyncio
import logging
from typing import Optional, AsyncGenerator

from fastapi import FastAPI, HTTPException
from pydantic import BaseModel
from contextlib import asynccontextmanager
import redis.asyncio as redis
import aiosqlite
from textblob import TextBlob


class SoulEngine:
    """Handle empathetic interactions and emotional analysis."""

    def __init__(self) -> None:
        self.logger = logging.getLogger(self.__class__.__name__)

    async def analyze_emotion(self, text: str) -> str:
        """Return a basic emotion label for the text."""
        try:
            polarity = TextBlob(text).sentiment.polarity
            if polarity > 0.2:
                return "positive"
            if polarity < -0.2:
                return "negative"
            return "neutral"
        except Exception as exc:  # pragma: no cover - defensive
            self.logger.error("Emotion analysis failed: %s", exc)
            return "unknown"

    async def craft_reply(self, text: str, emotion: str) -> str:
        """Craft a simple empathetic reply."""
        base = {
            "positive": "I'm glad to hear that!",
            "negative": "I'm sorry to hear that.",
            "neutral": "I understand.",
        }.get(emotion, "I see.")
        return f"{base} You said: {text}"


class BrainEngine:
    """Reasoning, memory management, and learning using SQLite."""

    def __init__(self, db_path: str = "brain.db") -> None:
        self.db_path = db_path
        self.logger = logging.getLogger(self.__class__.__name__)

    async def initialize(self) -> None:
        async with aiosqlite.connect(self.db_path) as db:
            await db.execute(
                "CREATE TABLE IF NOT EXISTS memories (key TEXT PRIMARY KEY, value TEXT)"
            )
            await db.commit()

    async def store_memory(self, key: str, value: str) -> None:
        try:
            async with aiosqlite.connect(self.db_path) as db:
                await db.execute(
                    "REPLACE INTO memories (key, value) VALUES (?, ?)",
                    (key, value),
                )
                await db.commit()
        except Exception as exc:
            self.logger.error("Storing memory failed: %s", exc)

    async def retrieve_memory(self, key: str) -> Optional[str]:
        try:
            async with aiosqlite.connect(self.db_path) as db:
                async with db.execute(
                    "SELECT value FROM memories WHERE key = ?", (key,)
                ) as cursor:
                    row = await cursor.fetchone()
                    if row:
                        return row[0]
            return None
        except Exception as exc:
            self.logger.error("Retrieving memory failed: %s", exc)
            return None

    async def reason(self, text: str) -> str:
        """Very simple reasoning: echo known memories or store new."""
        mem = await self.retrieve_memory(text)
        if mem:
            return f"I recall you said: {mem}"
        await self.store_memory(text, text)
        return "Thanks for telling me."        


class OpticalEngine:
    """High-speed data processing and communication via Redis."""

    def __init__(self, url: str = "redis://localhost:6379/0") -> None:
        self.redis = redis.from_url(url)
        self.logger = logging.getLogger(self.__class__.__name__)

    async def publish(self, channel: str, message: str) -> None:
        try:
            await self.redis.publish(channel, message)
        except Exception as exc:
            self.logger.error("Publish failed: %s", exc)

    async def subscribe(self, channel: str) -> AsyncGenerator[str, None]:
        pubsub = self.redis.pubsub()
        await pubsub.subscribe(channel)
        try:
            async for item in pubsub.listen():
                if item.get("type") == "message":
                    yield item.get("data")
        finally:
            await pubsub.unsubscribe(channel)


class AuraEngine:
    """Ethical oversight and contextual awareness."""

    def __init__(self) -> None:
        self.forbidden = {"hate", "kill", "malicious"}
        self.logger = logging.getLogger(self.__class__.__name__)

    async def check(self, text: str) -> bool:
        """Return True if text passes ethical check."""
        lowered = text.lower()
        if any(word in lowered for word in self.forbidden):
            self.logger.warning("Blocked unethical input: %s", text)
            return False
        return True


class UnifiedAI:
    """Central orchestrator coordinating all engines."""

    def __init__(self) -> None:
        self.soul = SoulEngine()
        self.brain = BrainEngine()
        self.optical = OpticalEngine()
        self.aura = AuraEngine()
        self.logger = logging.getLogger(self.__class__.__name__)

    async def setup(self) -> None:
        await self.brain.initialize()

    async def interact(self, text: str) -> str:
        if not await self.aura.check(text):
            raise HTTPException(status_code=400, detail="Inappropriate content")
        emotion = await self.soul.analyze_emotion(text)
        memory_response = await self.brain.reason(text)
        await self.optical.publish("unifiedai_log", text)
        reply = await self.soul.craft_reply(memory_response, emotion)
        return reply


logging.basicConfig(level=logging.INFO)
engine = UnifiedAI()

@asynccontextmanager
async def lifespan(app: FastAPI):
    """Handle startup and shutdown events for the app."""
    await engine.setup()
    yield
    # No specific shutdown logic but placeholder for future cleanup

app = FastAPI(lifespan=lifespan)


class Interaction(BaseModel):
    message: str




@app.post("/chat")
async def chat(data: Interaction) -> dict:
    response = await engine.interact(data.message)
    return {"response": response}


if __name__ == "__main__":
    import uvicorn

    uvicorn.run(app, host="0.0.0.0", port=8000)
