import logging
from typing import Optional
import aiosqlite


class BrainEngine:
    """Reasoning, memory management, and learning using SQLite."""

    def __init__(self, db_path: str = "brain.db") -> None:
        self.db_path = db_path
        self.logger = logging.getLogger(self.__class__.__name__)

    async def initialize(self) -> None:
        self.db = await aiosqlite.connect(self.db_path)
        await self.db.execute(
            "CREATE TABLE IF NOT EXISTS memories (key TEXT PRIMARY KEY, content TEXT, timestamp TEXT, access_count INTEGER)"
        )
        await self.db.commit()
        self.rules = {
            "productivity": "Try time-blocking and prioritizing tasks with a Pomodoro technique.",
            "learning": "Consider spaced repetition and hands-on projects.",
        }

    async def store_memory(self, key: str, value: str) -> None:
        try:
            await self.db.execute(
                "INSERT OR REPLACE INTO memories (key, content, timestamp, access_count) VALUES (?, ?, datetime('now'), COALESCE((SELECT access_count FROM memories WHERE key = ?),0))",
                (key, value, key),
            )
            await self.db.commit()
        except Exception as exc:
            self.logger.error("Storing memory failed: %s", exc)

    async def retrieve_memory(self, key: str) -> Optional[str]:
        try:
            async with self.db.execute(
                "SELECT content, access_count FROM memories WHERE key = ?",
                (key,),
            ) as cursor:
                row = await cursor.fetchone()
                if row:
                    await self.db.execute(
                        "UPDATE memories SET access_count = access_count + 1 WHERE key = ?",
                        (key,),
                    )
                    await self.db.commit()
                    return row[0]
            return None
        except Exception as exc:
            self.logger.error("Retrieving memory failed: %s", exc)
            return None

    async def reason(self, text: str) -> str:
        lowered = text.lower()
        for key, val in self.rules.items():
            if key in lowered:
                await self.learn(text)
                return f"Reasoned: {val}"
        mem = await self.retrieve_memory(lowered)
        if mem:
            return f"I recall you said: {mem}"
        await self.store_memory(lowered, text)
        return f"Reasoned: {lowered}"

    async def learn(self, item: str) -> bool:
        try:
            key = f"item_{await self.memory_count()}"
            await self.store_memory(key, item)
            return True
        except Exception as exc:
            self.logger.error("Learning failed: %s", exc)
            return False

    async def memory_count(self) -> int:
        async with self.db.execute("SELECT COUNT(*) FROM memories") as cursor:
            row = await cursor.fetchone()
            return row[0] if row else 0

    async def close(self) -> None:
        await self.db.close()
