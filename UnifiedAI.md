# UnifiedAI

UnifiedAI is a modular architecture that combines four engines to provide empathetic interactions, reasoning, high‑speed data processing and ethical oversight.  It exposes a small FastAPI service for demonstration purposes.

## Components and Intent

- **SoulEngine** – detects sentiment in user messages and crafts human‑like replies.
- **BrainEngine** – stores memories in SQLite and performs simple reasoning based on past interactions.
- **OpticalEngine** – communicates through Redis channels to broadcast events.
- **AuraEngine** – checks messages for forbidden content to enforce basic ethical rules.

The orchestrator coordinates these engines so that input flows through AuraEngine for validation, then SoulEngine and BrainEngine to generate a reply, while OpticalEngine publishes logs asynchronously.

## Running the Example

1. Install dependencies:
   ```bash
   pip install -r requirements.txt
   ```
2. Start a local Redis server (required for the OpticalEngine).
3. Run the API server:
   ```bash
   python unifiedai.py
   ```
4. Interact with the system using `curl` or any HTTP client:
   ```bash
   curl -X POST -H "Content-Type: application/json" \
        -d '{"message": "Hello there"}' http://localhost:8000/chat
   ```

The request is processed asynchronously. The reply contains an empathetic acknowledgement, demonstrates memory use and the message is published to Redis.

