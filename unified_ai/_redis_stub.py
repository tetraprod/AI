"""Fallback Redis asyncio client for environments without redis-py."""

from __future__ import annotations

from typing import Any


class FakePubSub:
    async def subscribe(self, channel: str) -> None:  # pragma: no cover - simple stub
        self._channel = channel

    async def unsubscribe(self, channel: str) -> None:  # pragma: no cover - simple stub
        self._channel = None

    async def listen(self):  # pragma: no cover - simple stub
        if False:
            yield {}


class Redis:
    """Minimal subset of ``redis.asyncio.Redis`` used in tests."""

    def __init__(self) -> None:
        self._published: list[tuple[str, Any]] = []

    async def publish(self, channel: str, data: Any) -> int:
        self._published.append((channel, data))
        return 1

    def pubsub(self) -> FakePubSub:
        return FakePubSub()

    async def ping(self) -> bool:
        return True

    async def close(self) -> None:  # pragma: no cover - nothing to close
        return None


def from_url(*_args: Any, **_kwargs: Any) -> Redis:
    return Redis()
