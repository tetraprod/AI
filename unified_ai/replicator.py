from __future__ import annotations

import argparse
import asyncio
import json
import base64
from typing import Any, Optional


class SystemReplicator:
    """Create duplicates of a running :class:`UnifiedAI` instance."""

    def __init__(self, engine: "UnifiedAI", token: str = "replica", encryption_key: Optional[str] = None) -> None:
        self.engine = engine
        self.token = token
        self.key = encryption_key.encode() if encryption_key else None

    async def _snapshot(self) -> dict[str, Any]:
        """Collect BrainEngine memories and enabled features."""
        memories: list[dict[str, Any]] = []
        async with self.engine.brain.db.execute(
            "SELECT key, content, timestamp, access_count FROM memories"
        ) as cursor:
            async for row in cursor:
                memories.append(
                    {
                        "key": row[0],
                        "content": row[1],
                        "timestamp": row[2],
                        "access_count": row[3],
                    }
                )
        return {"memories": memories, "features": list(self.engine.feature_manager.enabled)}

    def _secure(self, data: Any) -> str:
        payload = json.dumps({"token": self.token, "data": data})
        if not self.key:
            return payload
        encoded = payload.encode()
        key = self.key
        xored = bytes(b ^ key[i % len(key)] for i, b in enumerate(encoded))
        return base64.b64encode(xored).decode()

    async def duplicate_local(self) -> "UnifiedAI":
        """Spawn a copy of the engine within the current process."""
        from . import UnifiedAI

        snapshot = await self._snapshot()
        duplicate = UnifiedAI(self.engine.redis_url)
        await duplicate.connect()
        await duplicate.initialize()
        for feat in snapshot["features"]:
            duplicate.feature_manager.enable(feat)
        for mem in snapshot["memories"]:
            await duplicate.brain.store_memory(mem["key"], mem["content"])
        await self.engine.optical.transfer_data(self._secure({"status": "spawned"}), f"sync:{self.token}")
        return duplicate

    async def duplicate_remote(self, url: str) -> bool:
        """Send a snapshot to a remote listener via the OpticalEngine."""
        snapshot = await self._snapshot()
        return await self.engine.optical.transfer_data(self._secure(snapshot), url)


def _parse_args() -> argparse.Namespace:
    parser = argparse.ArgumentParser(description="Replicate a UnifiedAI instance")
    parser.add_argument("--remote", help="Remote host channel")
    parser.add_argument("--token", default="replica", help="Authentication token")
    parser.add_argument("--key", help="Optional encryption key")
    return parser.parse_args()


def main() -> None:
    from . import UnifiedAI

    args = _parse_args()
    engine = UnifiedAI()
    replicator = SystemReplicator(engine, token=args.token, encryption_key=args.key)

    async def run() -> None:
        await engine.connect()
        await engine.initialize()
        if args.remote:
            await replicator.duplicate_remote(args.remote)
        else:
            await replicator.duplicate_local()

    asyncio.run(run())


if __name__ == "__main__":
    main()
