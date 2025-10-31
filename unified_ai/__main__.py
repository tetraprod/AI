from __future__ import annotations

from contextlib import asynccontextmanager
from fastapi import FastAPI, HTTPException, Request

try:  # pragma: no cover - optional dependency
    import uvicorn
except ModuleNotFoundError:  # pragma: no cover - used in tests
    uvicorn = None

from . import UnifiedAI, lifespan as engine_lifespan

engine = UnifiedAI()

@asynccontextmanager
async def app_lifespan(app: FastAPI):
    async with engine_lifespan(app, engine) as eng:
        app.state.engine = eng
        yield

app = FastAPI(lifespan=app_lifespan)

@app.post("/query")
async def query_endpoint(payload: dict, request: Request):
    ai: UnifiedAI = request.app.state.engine
    query = payload.get("query") if isinstance(payload, dict) else None
    if not isinstance(query, str):
        raise HTTPException(status_code=400, detail="Invalid query payload")
    try:
        response = await ai.interact(query)
    except ValueError as exc:
        raise HTTPException(status_code=400, detail=str(exc))
    return {"response": response}

@app.get("/health")
async def health_endpoint(request: Request):
    ai: UnifiedAI = request.app.state.engine
    redis_ok = await ai.redis.ping()
    return {"redis": bool(redis_ok), "features": ai.list_enabled_features()}

@app.get("/metrics")
async def metrics_endpoint(request: Request):
    ai: UnifiedAI = request.app.state.engine
    mem_count = await ai.brain.memory_count()
    return {"memory_count": mem_count, "network_features": ai.list_enabled_features()}

def main() -> None:
    if not uvicorn:
        raise RuntimeError("uvicorn is not available in this environment")
    uvicorn.run("unified_ai.__main__:app", host="0.0.0.0", port=8000)


if __name__ == "__main__":
    main()
