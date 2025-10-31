"""Very small synchronous test client compatible with the local FastAPI stub."""

from __future__ import annotations

import asyncio
import inspect
import threading
from dataclasses import dataclass
from typing import Any, Callable, Optional

from . import FastAPI, HTTPException, Request


@dataclass
class _Response:
    status_code: int
    _payload: Any

    def json(self) -> Any:
        return self._payload


class TestClient:
    def __init__(self, app: FastAPI) -> None:
        self.app = app
        self._loop: Optional[asyncio.AbstractEventLoop] = None
        self._thread: Optional[threading.Thread] = None
        self._lifespan_cm = None

    def __enter__(self) -> "TestClient":
        self._loop = asyncio.new_event_loop()
        self._thread = threading.Thread(target=self._run_loop, daemon=True)
        self._thread.start()

        if self.app.lifespan:
            self._lifespan_cm = self.app.lifespan(self.app)
            asyncio.run_coroutine_threadsafe(self._lifespan_cm.__aenter__(), self._loop).result()
        asyncio.run_coroutine_threadsafe(self._run_events(self.app._startup_events), self._loop).result()
        return self

    def __exit__(self, exc_type, exc, tb) -> None:
        if self._loop is None:
            return
        asyncio.run_coroutine_threadsafe(self._run_events(self.app._shutdown_events), self._loop).result()
        if self._lifespan_cm:
            asyncio.run_coroutine_threadsafe(self._lifespan_cm.__aexit__(exc_type, exc, tb), self._loop).result()
        self._loop.call_soon_threadsafe(self._loop.stop)
        if self._thread:
            self._thread.join()
        self._loop.close()
        self._loop = None
        self._thread = None

    # ------------------------------------------------------------------ helpers

    def _call(self, method: str, path: str, payload: Any = None) -> _Response:
        handler = self.app._routes.get((method.upper(), path))
        if handler is None:
            raise ValueError(f"No route registered for {method} {path}")

        request = Request(self.app)
        payload = payload or {}
        assert self._loop is not None

        async def invoke() -> Any:
            args = self._build_args(handler, payload, request)
            if inspect.iscoroutinefunction(handler):
                return await handler(*args)
            return handler(*args)

        future = asyncio.run_coroutine_threadsafe(invoke(), self._loop)
        try:
            result = future.result()
            return _Response(200, result)
        except HTTPException as exc:
            return _Response(exc.status_code, {"detail": exc.detail})

    async def _run_events(self, events):
        for func in events:
            result = func()
            if inspect.isawaitable(result):
                await result

    @staticmethod
    def _build_args(handler: Callable[..., Any], payload: Any, request: Request) -> list[Any]:
        signature = inspect.signature(handler)
        params = list(signature.parameters.values())
        if len(params) >= 2:
            return [payload, request]
        if len(params) == 1:
            param = params[0]
            if param.annotation is Request or param.name == "request":
                return [request]
            return [payload]
        return []

    # ----------------------------------------------------------------- requests

    def post(self, path: str, json: Any = None) -> _Response:
        return self._call("POST", path, json)

    def get(self, path: str) -> _Response:
        return self._call("GET", path)

    def _run_loop(self) -> None:
        assert self._loop is not None
        asyncio.set_event_loop(self._loop)
        self._loop.run_forever()


__all__ = ["TestClient"]
