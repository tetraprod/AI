from __future__ import annotations

import logging
from datetime import datetime
from typing import Any, Dict, Optional


class BrainEngine:
    """Simple engine for reasoning and memory management."""

    def __init__(self, memory_limit: int = 1000) -> None:
        """Initialize the engine and configure logging.

        Args:
            memory_limit: Maximum number of memories to keep in memory.
        """

        self.memories: Dict[str, Dict[str, Any]] = {}
        self.memory_limit = memory_limit
        self._setup_logging()

    def _setup_logging(self) -> None:
        """Configure the logger for the engine."""

        logging.basicConfig(
            level=logging.INFO,
            format="%(asctime)s - %(name)s - %(levelname)s - %(message)s",
        )
        self.logger = logging.getLogger(__name__)

    # ------------------------------------------------------------------
    # Core behaviours
    # ------------------------------------------------------------------
    def reason(self, observation: str) -> Optional[str]:
        """Return a basic processed version of an observation."""

        try:
            self.logger.info("Processing observation: %s", observation)
            return f"Analyzed: {observation.strip().lower()}"
        except Exception as exc:  # pragma: no cover - defensive
            self.logger.error("Reasoning error: %s", exc)
            return None

    def solve_problem(self, problem: str) -> Optional[str]:
        """Return a placeholder solution for the given problem."""

        try:
            self.logger.info("Solving problem: %s", problem)
            return f"Solution to: {problem}"
        except Exception as exc:  # pragma: no cover - defensive
            self.logger.error("Problem solving error: %s", exc)
            return None

    def learn(self, item: str) -> bool:
        """Learn from an item and store it in memory."""

        try:
            self.logger.info("Learning item: %s", item)
            key = f"item_{len(self.memories)}"
            self.store_memory(
                key,
                {
                    "content": item,
                    "timestamp": datetime.now().isoformat(),
                    "access_count": 0,
                },
            )
            return True
        except Exception as exc:  # pragma: no cover - defensive
            self.logger.error("Learning error: %s", exc)
            return False

    # ------------------------------------------------------------------
    # Memory handling
    # ------------------------------------------------------------------
    def store_memory(self, key: str, value: Any) -> None:
        """Store a memory, pruning oldest if the limit is reached."""

        if len(self.memories) >= self.memory_limit:
            oldest_key = next(iter(self.memories))
            self.logger.warning("Memory limit reached, dropping: %s", oldest_key)
            self.memories.pop(oldest_key)

        self.memories[key] = value
        self.logger.debug("Stored memory %s", key)

    def retrieve_memory(self, key: str) -> Optional[Any]:
        """Retrieve a memory by key and increment its access count."""

        memory = self.memories.get(key)
        if memory is None:
            self.logger.warning("Memory not found: %s", key)
            return None

        memory["access_count"] += 1
        self.logger.debug(
            "Retrieved memory %s (access_count=%d)", key, memory["access_count"]
        )
        return memory["content"]

    def clear_memories(self) -> None:
        """Remove all memories."""

        self.memories.clear()
        self.logger.info("All memories cleared")

