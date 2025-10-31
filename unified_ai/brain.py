import logging
from dataclasses import dataclass
from datetime import datetime
from typing import Any, Dict, List, Optional


@dataclass
class MemoryEntry:
    content: str
    timestamp: datetime
    access_count: int = 0


class BrainEngine:
    """Reasoning, memory management, and learning with an in-memory store."""

    def __init__(self, db_path: str = "brain.db") -> None:
        self.db_path = db_path
        self.logger = logging.getLogger(self.__class__.__name__)
        self._memories: Dict[str, MemoryEntry] = {}
        self.rules = {
            "productivity": "Try time-blocking and prioritizing tasks with a Pomodoro technique.",
            "learning": "Consider spaced repetition and hands-on projects.",
        }
        # ``SystemReplicator`` historically accessed ``brain.db`` directly, so
        # we keep a ``db`` attribute pointing at the engine itself for
        # compatibility.  Tests can still monkeypatch ``db`` if desired.
        self.db: "BrainEngine" = self

    async def initialize(self) -> None:
        # Nothing to initialise for the in-memory backend, but the coroutine is
        # retained for interface compatibility.
        return None

    async def store_memory(self, key: str, value: str) -> None:
        entry = self._memories.get(key)
        if entry:
            entry.content = value
            entry.timestamp = datetime.utcnow()
        else:
            self._memories[key] = MemoryEntry(content=value, timestamp=datetime.utcnow())

    async def retrieve_memory(self, key: str) -> Optional[str]:
        entry = self._memories.get(key)
        if not entry:
            return None
        entry.access_count += 1
        return entry.content

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
        except Exception as exc:  # pragma: no cover - defensive
            self.logger.error("Learning failed: %s", exc)
            return False

    async def memory_count(self) -> int:
        return len(self._memories)

    async def close(self) -> None:
        self._memories.clear()

    async def export_memories(self) -> List[Dict[str, Any]]:
        """Return a serialisable snapshot of stored memories."""

        return [
            {
                "key": key,
                "content": entry.content,
                "timestamp": entry.timestamp.isoformat(),
                "access_count": entry.access_count,
            }
            for key, entry in self._memories.items()
        ]
