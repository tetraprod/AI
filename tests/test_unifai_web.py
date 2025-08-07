import sys
from pathlib import Path

import pytest
from fastapi.testclient import TestClient

sys.path.append(str(Path(__file__).resolve().parents[1]))

import unifai_web as web  # noqa: E402


@pytest.fixture
def client(monkeypatch):
    # Use a dummy engine to avoid startup overhead
    class DummyEngine:
        async def connect(self):
            pass
        async def initialize(self):
            pass
        async def close(self):
            pass
        async def interact(self, msg):
            return "echo:" + msg

    dummy = DummyEngine()
    monkeypatch.setattr(web, "engine", dummy)
    with TestClient(web.app) as c:
        yield c


def test_chat_endpoint(client):
    resp = client.post("/chat", json={"message": "hi"})
    assert resp.status_code == 200
    assert resp.json() == {"reply": "echo:hi"}
