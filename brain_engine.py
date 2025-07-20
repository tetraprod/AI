from typing import Any, Optional, Dict, List
import logging
from datetime import datetime
import json


class BrainEngine:
    """Engine for advanced reasoning, problem-solving, and memory storage."""

    def __init__(self, memory_limit: int = 1000) -> None:
        """Initialize the engine with a configurable memory limit."""
        self.memories: Dict[str, Dict[str, Any]] = {}
        self.memory_limit = memory_limit
        self._setup_logging()
        self.logger.info("BrainEngine initialized.")

    def _setup_logging(self) -> None:
        """Configure logging for the engine."""
        logging.basicConfig(
            level=logging.INFO,
            format="%(asctime)s - %(name)s - %(levelname)s - %(message)s",
        )
        self.logger = logging.getLogger(__name__)

    async def _call_llm(
        self,
        prompt: str,
        model: str = "gemini-2.0-flash",
        response_schema: Optional[Dict] = None,
    ) -> Optional[str]:
        """Internal helper to call the Gemini API asynchronously."""
        try:
            self.logger.info(
                "Calling LLM with prompt (first 100 chars): %s...", prompt[:100]
            )
            chat_history = [{"role": "user", "parts": [{"text": prompt}]}]
            payload = {"contents": chat_history}
            if response_schema:
                payload["generationConfig"] = {
                    "responseMimeType": "application/json",
                    "responseSchema": response_schema,
                }

            apiKey = ""  # Provided at runtime
            api_url = (
                f"https://generativelanguage.googleapis.com/v1beta/models/{model}:"
                f"generateContent?key={apiKey}"
            )

            response = await fetch(
                api_url,
                {
                    "method": "POST",
                    "headers": {"Content-Type": "application/json"},
                    "body": json.dumps(payload),
                },
            )

            result = await response.json()
            candidates = result.get("candidates", [])
            if (
                candidates
                and candidates[0].get("content")
                and candidates[0]["content"].get("parts")
            ):
                text_part = candidates[0]["content"]["parts"][0]["text"]
                if response_schema:
                    try:
                        return json.loads(text_part)
                    except json.JSONDecodeError as exc:
                        self.logger.error(
                            "Failed to parse JSON response from LLM: %s", exc
                        )
                        return None
                return text_part
            self.logger.warning(
                "LLM response structure unexpected or empty: %s", result
            )
            return None
        except Exception as exc:  # pragma: no cover - defensive
            self.logger.error("Error calling LLM: %s", exc)
            return None

    async def reason(self, observation: str, context: Optional[str] = None) -> Optional[str]:
        """Analyze an observation using the LLM and optional contextual memories."""
        try:
            self.logger.info("Reasoning about observation: %s", observation)
            relevant = self.retrieve_contextual_memories(observation, top_k=3)
            memory_context = ""
            if relevant:
                memory_context = "\n\nRelevant Memories:\n" + "\n".join(
                    [
                        f"- {m['content']} (Accessed {m['access_count']} times, Tags: {', '.join(m.get('tags', []))})"
                        for m in relevant
                    ]
                )

            prompt = (
                "You are an advanced reasoning engine within a digital forge. "
                "Analyze the following observation and provide a concise, insightful analysis. "
                "Consider any provided context and relevant memories.\n\n"
                f"Observation: {observation}\n"
                f"Context: {context if context else 'None'}"
                f"{memory_context}\n\nAnalysis:"
            )
            return await self._call_llm(prompt)
        except Exception as exc:  # pragma: no cover - defensive
            self.logger.error("Error in reasoning: %s", exc)
            return None

    async def solve_problem(
        self, problem: str, context: Optional[str] = None
    ) -> Optional[str]:
        """Use the LLM to generate a solution for the provided problem."""
        try:
            self.logger.info("Solving problem: %s", problem)
            relevant = self.retrieve_contextual_memories(problem, top_k=5)
            memory_context = ""
            if relevant:
                memory_context = "\n\nRelevant Knowledge & Past Solutions:\n" + "\n".join(
                    [
                        f"- {m['content']} (Accessed {m['access_count']} times, Tags: {', '.join(m.get('tags', []))})"
                        for m in relevant
                    ]
                )

            prompt = (
                "You are a problem-solving AI within a digital forge. "
                "Analyze the following problem and provide a clear, actionable solution. "
                "Break down the steps if necessary. "
                "Consider any provided context and relevant knowledge from memory.\n\n"
                f"Problem: {problem}\n"
                f"Context: {context if context else 'None'}"
                f"{memory_context}\n\nSolution:"
            )
            return await self._call_llm(prompt)
        except Exception as exc:  # pragma: no cover - defensive
            self.logger.error("Error in problem solving: %s", exc)
            return None

    def learn(
        self, item_key: str, content: Any, tags: Optional[List[str]] = None
    ) -> bool:
        """Store an item in memory, serializing if needed."""
        try:
            self.logger.info("Learning item with key: %s", item_key)
            if not isinstance(content, (str, int, float, bool, type(None))):
                try:
                    content_to_store = json.dumps(content)
                except TypeError:
                    self.logger.warning(
                        "Content for key '%s' is not JSON serializable. Storing as string",
                        item_key,
                    )
                    content_to_store = str(content)
            else:
                content_to_store = content

            self.store_memory(
                item_key,
                {
                    "content": content_to_store,
                    "timestamp": datetime.now().isoformat(),
                    "access_count": 0,
                    "tags": tags if tags is not None else [],
                },
            )
            return True
        except Exception as exc:  # pragma: no cover - defensive
            self.logger.error("Error in learning: %s", exc)
            return False

    def store_memory(self, key: str, value: Dict[str, Any]) -> None:
        """Save a memory item, evicting the oldest if necessary."""
        if len(self.memories) >= self.memory_limit:
            self.logger.warning("Memory limit reached. Evicting oldest memory.")
            oldest_key = next(iter(self.memories))
            self.memories.pop(oldest_key)
            self.logger.debug("Evicted memory with key: %s", oldest_key)

        self.memories[key] = value
        self.logger.debug("Stored memory with key: %s", key)

    def retrieve_memory(self, key: str) -> Optional[Any]:
        """Fetch a memory and update its access count."""
        memory_data = self.memories.get(key)
        if memory_data:
            memory_data["access_count"] += 1
            self.logger.debug(
                "Retrieved memory with key: %s, access_count: %d",
                key,
                memory_data["access_count"],
            )
            content = memory_data["content"]
            if isinstance(content, str):
                try:
                    return json.loads(content)
                except json.JSONDecodeError:
                    pass
            return content
        self.logger.warning("No memory found for key: %s", key)
        return None

    def retrieve_contextual_memories(
        self, query: str, top_k: int = 3
    ) -> List[Dict[str, Any]]:
        """Return memories relevant to the query using simple keyword matching."""
        relevant: List[Dict[str, Any]] = []
        query_lower = query.lower()
        for memory_data in self.memories.values():
            content_lower = str(memory_data["content"]).lower()
            tags = memory_data.get("tags", [])
            if query_lower in content_lower or any(t.lower() in query_lower for t in tags):
                relevant.append(memory_data)
        relevant.sort(key=lambda x: x["access_count"], reverse=True)
        return relevant[:top_k]

    def clear_memories(self) -> None:
        """Remove all stored memories."""
        self.memories.clear()
        self.logger.info("All memories cleared.")


async def main() -> None:
    """Demonstration of BrainEngine usage (requires 'fetch')."""
    engine = BrainEngine(memory_limit=5)

    print("\n--- Learning Phase ---")
    await engine.learn(
        "concept_void",
        "The void is the primordial expanse, source of all raw material.",
    )
    await engine.learn(
        "protocol_forge",
        "The Forgemaster's Mandate guides all creation processes.",
    )
    await engine.learn(
        "ai_core_design",
        {"name": "EchoMind", "version": "1.0", "status": "active"},
        tags=["AI", "design"],
    )
    await engine.learn(
        "problem_solving_strategy",
        "Break down complex problems into smaller, manageable sub-problems.",
    )
    await engine.learn(
        "history_first_absence",
        "The first absence marked a critical turning point in our understanding of resonance.",
    )
    await engine.learn("test_data_1", "This is some test data for a query.", tags=["test"])

    print("\n--- Memory Retrieval ---")
    retrieved_concept = engine.retrieve_memory("concept_void")
    print(f"Retrieved 'concept_void': {retrieved_concept}")

    retrieved_ai_design = engine.retrieve_memory("ai_core_design")
    print(
        f"Retrieved 'ai_core_design': {retrieved_ai_design} (Type: {type(retrieved_ai_design)})"
    )

    print("\n--- Contextual Memory Retrieval ---")
    relevant_to_ai = engine.retrieve_contextual_memories("AI development", top_k=2)
    print("Memories relevant to 'AI development':")
    for mem in relevant_to_ai:
        print(f"  - {mem['content']} (Accesses: {mem['access_count']})")

    print("\n--- Reasoning Phase (LLM Integration) ---")
    observation = "The latest data influx shows unexpected resonance patterns."
    reasoned_response = await engine.reason(observation)
    print(f"Reasoned response: {reasoned_response}")

    print("\n--- Problem Solving Phase (LLM Integration) ---")
    problem = (
        "How can we optimize the energy consumption of the Aetherium Anvil while maintaining output quality?"
    )
    solution_response = await engine.solve_problem(problem)
    print(f"Solution: {solution_response}")

    print("\n--- Memory Limit Test ---")
    for i in range(6):
        await engine.learn(f"ephemeral_data_{i}", f"This is ephemeral data {i}")
    print(f"Current memory count: {len(engine.memories)}")

    engine.clear_memories()
    print(f"Memories after clearing: {len(engine.memories)}")


# To run this example in Canvas you would call main(). For local testing you
# may need to mock the fetch function. Example mock:
# import asyncio
# async def mock_fetch(url, options):
#     print(f"MOCK FETCH: Calling {url} with {options}")
#     return type('obj', (object,), {
#         'json': lambda: {
#             'candidates': [{
#                 'content': {
#                     'parts': [{'text': 'Mocked response.'}]
#                 }
#             }]
#         }
#     })()
# global fetch
# fetch = mock_fetch
# if __name__ == "__main__":
#     asyncio.run(main())
