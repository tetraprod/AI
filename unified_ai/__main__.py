from __future__ import annotations

from fastapi import FastAPI, HTTPException, Request
from contextlib import asynccontextmanager
import uvicorn

from . import lifespan as engine_lifespan, UnifiedAI


@asynccontextmanager
async def lifespan(app: FastAPI):
    # reuse the package lifespan but store the engine on app.state
    async with engine_lifespan(app=app) as engine:
        app.state.engine = engine
        yield


app = FastAPI(lifespan=lifespan)


@app.post("/query")
async def query(payload: dict, request: Request):
    text = payload.get("query")
    if not text:
        raise HTTPException(status_code=400, detail="'query' field required")
    engine: UnifiedAI = request.app.state.engine
    result = await engine.interact(text)
    return {"response": result}


@app.get("/health")
async def health(request: Request):
    engine: UnifiedAI = request.app.state.engine
    try:
        redis_ok = await engine.redis.ping()
    except Exception:
        redis_ok = False
    return {"redis": redis_ok, "features": engine.list_enabled_features()}


@app.get("/metrics")
async def metrics(request: Request):
    engine: UnifiedAI = request.app.state.engine
    count = await engine.brain.memory_count()
    return {"memory_count": count, "network_features": engine.list_enabled_features()}


if __name__ == "__main__":
    uvicorn.run(app, host="0.0.0.0", port=8000)
