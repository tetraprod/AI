from fastapi import FastAPI, HTTPException
from pydantic import BaseModel
from unified_ai import UnifiedAI

app = FastAPI()
engine = UnifiedAI()

class ChatMessage(BaseModel):
    message: str

@app.on_event("startup")
async def startup() -> None:
    await engine.connect()
    await engine.initialize()

@app.on_event("shutdown")
async def shutdown() -> None:
    await engine.close()

@app.post("/chat")
async def chat(msg: ChatMessage):
    try:
        reply = await engine.interact(msg.message)
    except ValueError as exc:
        raise HTTPException(status_code=400, detail=str(exc))
    return {"reply": reply}

