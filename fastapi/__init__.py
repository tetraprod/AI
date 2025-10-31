"""Tiny subset of the FastAPI interface required for the tests."""

from __future__ import annotations

from contextlib import AbstractAsyncContextManager
from types import SimpleNamespace
from typing import Any, Awaitable, Callable, Dict, List, Optional, Tuple


class HTTPException(Exception):
    def __init__(self, status_code: int, detail: str) -> None:
        super().__init__(detail)
        self.status_code = status_code
        self.detail = detail


class Request:
    def __init__(self, app: "FastAPI") -> None:
        self.app = app


RouteHandler = Callable[..., Awaitable[Any]]


class FastAPI:
    def __init__(self, lifespan: Optional[Callable[["FastAPI"], AbstractAsyncContextManager]] = None) -> None:
        self._routes: Dict[Tuple[str, str], RouteHandler] = {}
        self.lifespan = lifespan
        self.state = SimpleNamespace()
        self._startup_events: List[Callable[[], Awaitable[Any] | Any]] = []
        self._shutdown_events: List[Callable[[], Awaitable[Any] | Any]] = []

    def _register(self, method: str, path: str, func: RouteHandler) -> RouteHandler:
        self._routes[(method.upper(), path)] = func
        return func

    def post(self, path: str) -> Callable[[RouteHandler], RouteHandler]:
        def decorator(func: RouteHandler) -> RouteHandler:
            return self._register("POST", path, func)

        return decorator

    def get(self, path: str) -> Callable[[RouteHandler], RouteHandler]:
        def decorator(func: RouteHandler) -> RouteHandler:
            return self._register("GET", path, func)

        return decorator

    def on_event(self, name: str) -> Callable[[Callable[..., Any]], Callable[..., Any]]:
        if name not in {"startup", "shutdown"}:
            raise ValueError("Unsupported event type")
        target = self._startup_events if name == "startup" else self._shutdown_events

        def decorator(func: Callable[..., Any]) -> Callable[..., Any]:
            target.append(func)
            return func

        return decorator


__all__ = ["FastAPI", "HTTPException", "Request"]
