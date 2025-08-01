import sys
from pathlib import Path
from contextlib import asynccontextmanager
from unittest.mock import AsyncMock

import pytest
from fastapi.testclient import TestClient

sys.path.append(str(Path(__file__).resolve().parents[1]))

import unified_ai.__main__ as api  # noqa: E402
from unified_ai import UnifiedAI


@pytest.mark.asyncio
async def test_api_endpoints(monkeypatch):
    engine = UnifiedAI()
    engine.interact = AsyncMock(return_value="reply")
    engine.redis.ping = AsyncMock(return_value=True)
    engine.brain.memory_count = AsyncMock(return_value=5)
    engine.list_enabled_features = lambda: ["net"]

    @asynccontextmanager
    async def fake_engine_lifespan(app=None, engine_param=None):
        yield engine

    monkeypatch.setattr(api, "engine_lifespan", fake_engine_lifespan)

    with TestClient(api.app) as client:
        resp = client.post("/query", json={"query": "hi"})
        assert resp.status_code == 200
        assert resp.json() == {"response": "reply"}

        health = client.get("/health").json()
        assert health == {"redis": True, "features": ["net"]}

        metrics = client.get("/metrics").json()
        assert metrics == {"memory_count": 5, "network_features": ["net"]}
