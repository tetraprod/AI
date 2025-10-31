from fastapi import FastAPI, HTTPException
from unified_ai import UnifiedAI

app = FastAPI()
engine = UnifiedAI()

@app.on_event("startup")
async def startup() -> None:
    await engine.connect()
    await engine.initialize()

@app.on_event("shutdown")
async def shutdown() -> None:
    await engine.close()

@app.post("/chat")
async def chat(msg: dict):
    message = msg.get("message") if isinstance(msg, dict) else None
    if not isinstance(message, str):
        raise HTTPException(status_code=400, detail="Invalid message payload")
    try:
        reply = await engine.interact(message)
    except ValueError as exc:
        raise HTTPException(status_code=400, detail=str(exc))
    return {"reply": reply}

