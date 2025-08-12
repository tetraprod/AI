import asyncio
import logging
from typing import Any, AsyncGenerator, Dict

from fastapi import FastAPI, HTTPException
from pydantic import BaseModel


class SoulEngine:
    """Handle empathetic interactions and manage Resource Parity.

    The Resource Parity framework represents a pool of "resource clouds"
    assigned to each resident. Baseline guarantees ensure a minimum
    allocation while real-time needs can temporarily adjust the size of a
    resident's cloud. ``analyze_resource_needs`` distributes available
    capacity according to these rules so residents with greater need
    receive more resources without dropping others below their baselines.
    """

    def __init__(self, total_capacity: int = 100) -> None:
        self.logger = logging.getLogger(self.__class__.__name__)
        self.total_capacity = total_capacity
        self.baseline: Dict[str, int] = {}
        self.current_needs: Dict[str, int] = {}
        self.resource_clouds: Dict[str, int] = {}

    def register_resident(self, user_id: str, baseline: int) -> None:
        """Register a new resident with a baseline guarantee."""
        self.baseline[user_id] = baseline
        self.current_needs.setdefault(user_id, baseline)
        self.resource_clouds.setdefault(user_id, baseline)

    def update_need(self, user_id: str, need: int) -> None:
        """Update a resident's current resource need."""
        if user_id not in self.baseline:
            self.register_resident(user_id, 0)
        self.current_needs[user_id] = need

    def analyze_emotion(self, text: str) -> str:
        """Very small sentiment check used for replies."""
        lowered = text.lower()
        if any(word in lowered for word in ["happy", "great", "awesome"]):
            return "positive"
        if any(word in lowered for word in ["sad", "angry", "upset"]):
            return "negative"
        return "neutral"

    def craft_reply(self, text: str, emotion: str) -> str:
        base = {
            "positive": "I'm glad to hear that!",
            "negative": "I'm sorry to hear that.",
            "neutral": "I understand.",
        }.get(emotion, "I see.")
        return f"{base} You said: {text}"

    def analyze_resource_needs(self) -> None:
        """Apply context-aware equity rules to adjust allocations."""
        total_baseline = sum(self.baseline.values())
        leftover = max(self.total_capacity - total_baseline, 0)
        extra_needs = {
            uid: max(self.current_needs.get(uid, 0) - self.baseline.get(uid, 0), 0)
            for uid in self.baseline
        }
        total_extra = sum(extra_needs.values())
        for uid in self.baseline:
            allocation = self.baseline[uid]
            if leftover and total_extra:
                share = extra_needs[uid] / total_extra if total_extra else 0
                allocation += int(leftover * share)
            self.resource_clouds[uid] = allocation


class BrainEngine:
    """Store memories and simple rule-based reasoning."""

    def __init__(self) -> None:
        self.logger = logging.getLogger(self.__class__.__name__)
        self.memories: Dict[str, str] = {}

    async def initialize(self) -> None:
        pass  # placeholder for database setup

    async def store_memory(self, key: str, value: str) -> None:
        self.memories[key] = value

    async def retrieve_memory(self, key: str) -> str | None:
        return self.memories.get(key)

    async def reason(self, text: str) -> str:
        mem = await self.retrieve_memory(text.lower())
        if mem:
            return f"I recall you said: {mem}"
        await self.store_memory(text.lower(), text)
        return text

    async def memory_count(self) -> int:
        return len(self.memories)


class AuraEngine:
    """Very small ethics layer checking for forbidden words."""

    def __init__(self) -> None:
        self.logger = logging.getLogger(self.__class__.__name__)
        self.forbidden = {"hate", "kill", "malicious"}

    async def validate_output(self, text: str) -> bool:
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
        self.aura = AuraEngine()
        self.logger = logging.getLogger(self.__class__.__name__)

    async def initialize(self) -> None:
        await self.brain.initialize()

    async def close(self) -> None:
        pass

    async def interact(self, text: str) -> str:
        if not await self.aura.validate_output(text):
            raise HTTPException(status_code=400, detail="Inappropriate content")
        emotion = self.soul.analyze_emotion(text)
        memory_response = await self.brain.reason(text)
        if await self.aura.validate_output(memory_response):
            return self.soul.craft_reply(memory_response, emotion)
        return "Output blocked due to ethics rules"


logging.basicConfig(level=logging.INFO)
app = FastAPI()
engine = UnifiedAI()


class QueryRequest(BaseModel):
    query: str


@app.on_event("startup")
async def startup() -> None:
    await engine.initialize()


@app.on_event("shutdown")
async def shutdown() -> None:
    await engine.close()


@app.post("/query")
async def query_endpoint(data: QueryRequest):
    try:
        response = await engine.interact(data.query)
        return {"response": response}
    except HTTPException as exc:
        raise exc
    except Exception as exc:  # pragma: no cover - defensive
        raise HTTPException(status_code=500, detail=str(exc))


@app.get("/health")
async def health():
    count = await engine.brain.memory_count()
    return {"status": "healthy", "memory_count": count}


if __name__ == "__main__":
    import uvicorn

    uvicorn.run(app, host="0.0.0.0", port=8000)
