# UnifiedAI

`UnifiedAI` is a minimal async engine that relies on Redis for short term storage.
It exposes a `connect()` method to establish the connection and a `close()`
coroutine to gracefully shut it down.

## Shutdown behavior

When used inside an application's lifespan context, ensure `close()` is awaited
on shutdown so the Redis connection is released:

```python
from fastapi import FastAPI
from unified_ai import lifespan

app = FastAPI(lifespan=lifespan)
```

The provided context manager automatically invokes `engine.close()` after the
`yield`, ensuring Redis resources are released.
