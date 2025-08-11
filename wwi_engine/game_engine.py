"""Simple WWI-themed game engine with memory constraints."""
from __future__ import annotations

import resource
from dataclasses import dataclass, field
from typing import Dict, List


@dataclass
class Asset:
    name: str
    data: bytearray
    quality: str


@dataclass
class GameEngine:
    """A minimal game engine enforcing a memory ceiling.

    The engine simulates asset loading and automatically degrades
    asset quality when loading high quality assets would exceed the
    configured memory limit.
    """

    max_memory_bytes: int = 4 * 1024 ** 3  # 4GB by default
    assets: List[Asset] = field(default_factory=list)

    HIGH_RES_SIZE: int = 10 * 1024 ** 2  # 10MB per high-res asset
    LOW_RES_SIZE: int = 1 * 1024 ** 2   # 1MB per low-res asset

    def _current_memory(self) -> int:
        """Return current memory used by assets in bytes."""
        return sum(len(asset.data) for asset in self.assets)

    def _system_memory(self) -> int:
        """Return the resident set size of the current process."""
        usage = resource.getrusage(resource.RUSAGE_SELF)
        # ru_maxrss is kilobytes on Linux
        return usage.ru_maxrss * 1024

    def _within_limit(self) -> bool:
        return self._current_memory() <= self.max_memory_bytes

    def load_asset(self, name: str, quality: str = "high") -> str:
        """Load an asset, degrading quality if necessary.

        Parameters
        ----------
        name: str
            Name of the asset.
        quality: str
            Requested quality level ("high" or "low").

        Returns
        -------
        str
            Actual quality level used after potential degradation.
        """

        size = self.HIGH_RES_SIZE if quality == "high" else self.LOW_RES_SIZE
        self.assets.append(Asset(name=name, data=bytearray(size), quality=quality))

        if not self._within_limit():
            # If exceeding limit with high quality, degrade to low quality
            if quality == "high":
                self.assets.pop()
                return self.load_asset(name, quality="low")
            # Low quality still exceeds limit -> clean up and raise
            self.assets.pop()
            raise MemoryError("Memory limit exceeded even with low quality asset")

        return quality

    def summary(self) -> Dict[str, int]:
        """Return a summary of loaded assets and memory usage."""
        return {
            "asset_count": len(self.assets),
            "asset_memory": self._current_memory(),
            "process_memory": self._system_memory(),
            "max_memory": self.max_memory_bytes,
        }
