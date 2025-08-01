from __future__ import annotations

from contextlib import asynccontextmanager
from fastapi import FastAPI, HTTPException, Request
from pydantic import BaseModel
import uvicorn

from . import UnifiedAI, lifespan

engine = UnifiedAI()

@asynccontextmanager
async def app_lifespan(app: FastAPI):
    async with lifespan(app=app, engine=engine):
        app.state.engine = engine
        yield

app = FastAPI(lifespan=app_lifespan)

class QueryModel(BaseModel):
    query: str

@app.post("/query")
async def query_endpoint(payload: QueryModel, request: Request):
    ai: UnifiedAI = request.app.state.engine
    try:
        response = await ai.interact(payload.query)
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
    return {"memory_count": mem_count, "enabled_features": ai.list_enabled_features()}

def main() -> None:
    uvicorn.run("unified_ai.__main__:app", host="0.0.0.0", port=8000)

if __name__ == "__main__":
    main()
=======
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
