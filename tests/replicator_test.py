import pytest
from pathlib import Path
from unittest.mock import AsyncMock

import sys
sys.path.append(str(Path(__file__).resolve().parents[1]))

from unified_ai.replicator import SystemReplicator
from unified_ai import UnifiedAI


@pytest.mark.asyncio
async def test_duplicate_local_invokes_store(monkeypatch):
    engine = UnifiedAI()
    engine.connect = AsyncMock()
    engine.initialize = AsyncMock()
    engine.optical.transfer_data = AsyncMock()
    # snapshot to return one memory and feature
    async def fake_snapshot(self):
        return {"memories": [{"key": "k", "content": "v"}], "features": ["feat"]}

    monkeypatch.setattr(SystemReplicator, "_snapshot", fake_snapshot)

    duplicate = AsyncMock()
    duplicate.connect = AsyncMock()
    duplicate.initialize = AsyncMock()
    duplicate.brain.store_memory = AsyncMock()
    from unittest.mock import MagicMock
    duplicate.feature_manager.enable = MagicMock()
    # patch UnifiedAI constructor to return our duplicate
    monkeypatch.setattr("unified_ai.UnifiedAI", lambda *a, **k: duplicate)

    replicator = SystemReplicator(engine, token="t")
    result = await replicator.duplicate_local()

    duplicate.connect.assert_awaited_once()
    duplicate.initialize.assert_awaited_once()
    duplicate.brain.store_memory.assert_awaited_with("k", "v")
    duplicate.feature_manager.enable.assert_called_with("feat")
    assert result is duplicate


@pytest.mark.asyncio
async def test_duplicate_remote_sends(monkeypatch):
    engine = UnifiedAI()
    engine.optical.transfer_data = AsyncMock(return_value=True)
    engine.brain.db = AsyncMock()  # avoid real DB

    async def fake_snapshot(self):
        return {"foo": "bar"}

    monkeypatch.setattr(SystemReplicator, "_snapshot", fake_snapshot)
    replicator = SystemReplicator(engine, token="t")
    res = await replicator.duplicate_remote("channel")
    engine.optical.transfer_data.assert_awaited()
    assert res is True
