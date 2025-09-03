"""Simple WWI-themed game engine with memory constraints."""
from __future__ import annotations

import psutil
from dataclasses import dataclass, field
from typing import Dict, List, Tuple


@dataclass
class Asset:
    name: str
    data: bytearray
    quality: str


@dataclass
class PhysicsObject:
    """Simple physics body tracked by the engine."""

    name: str
    position: List[float]
    velocity: List[float]
    mass: float = 1.0


@dataclass
class Light:
    """Point light with an intensity value."""

    position: Tuple[float, float]
    intensity: float


@dataclass
class GameEngine:
    """A minimal game engine with memory, physics and lighting.

    The engine simulates asset loading and automatically degrades
    asset quality when loading high quality assets would exceed the
    configured memory limit.  It also maintains a list of physics
    bodies affected by gravity and point lights for simple lighting
    calculations.
    """

    max_memory_bytes: int = 4 * 1024 ** 3  # 4GB by default
    assets: List[Asset] = field(default_factory=list)
    physics_objects: List[PhysicsObject] = field(default_factory=list)
    lights: List[Light] = field(default_factory=list)
    gravity: float = 9.81

    HIGH_RES_SIZE: int = 10 * 1024 ** 2  # 10MB per high-res asset
    LOW_RES_SIZE: int = 1 * 1024 ** 2   # 1MB per low-res asset

    def _current_memory(self) -> int:
        """Return current memory used by assets in bytes."""
        return sum(len(asset.data) for asset in self.assets)

    def _system_memory(self) -> int:
        """Return the resident set size of the current process."""
        process = psutil.Process()
        return process.memory_info().rss

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

    # ------------------------------------------------------------------
    # Physics
    # ------------------------------------------------------------------
    def add_physics_object(
        self,
        name: str,
        position: Tuple[float, float],
        velocity: Tuple[float, float] = (0.0, 0.0),
        mass: float = 1.0,
    ) -> PhysicsObject:
        """Register a new physics body in the world."""

        obj = PhysicsObject(name, list(position), list(velocity), mass)
        self.physics_objects.append(obj)
        return obj

    def step_physics(self, dt: float) -> None:
        """Advance the physics simulation by ``dt`` seconds."""

        for obj in self.physics_objects:
            # Apply gravity in the negative Y direction
            obj.velocity[1] -= self.gravity * dt
            obj.position[0] += obj.velocity[0] * dt
            obj.position[1] += obj.velocity[1] * dt

    # ------------------------------------------------------------------
    # Lighting
    # ------------------------------------------------------------------
    def add_light(self, position: Tuple[float, float], intensity: float) -> Light:
        """Add a point light to the scene."""

        light = Light(position, intensity)
        self.lights.append(light)
        return light

    def compute_lighting(self, point: Tuple[float, float]) -> float:
        """Return the combined light intensity at ``point``.

        Uses a simple inverse-square law for attenuation.
        """

        total = 0.0
        for light in self.lights:
            dx = point[0] - light.position[0]
            dy = point[1] - light.position[1]
            dist_sq = dx * dx + dy * dy
            if dist_sq == 0:
                # If the point is on the light, avoid division by zero
                total += light.intensity
            else:
                total += light.intensity / dist_sq
        return total

    def summary(self) -> Dict[str, int]:
        """Return a summary of loaded assets and memory usage."""
        return {
            "asset_count": len(self.assets),
            "asset_memory": self._current_memory(),
            "process_memory": self._system_memory(),
            "max_memory": self.max_memory_bytes,
        }
