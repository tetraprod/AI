"""Minimal pytest helpers for asynchronous tests."""

from __future__ import annotations

import asyncio

import pytest


@pytest.hookimpl(tryfirst=True)
def pytest_pyfunc_call(pyfuncitem: pytest.Function) -> bool | None:
    marker = pyfuncitem.get_closest_marker("asyncio")
    if not marker:
        return None

    loop = asyncio.new_event_loop()
    try:
        asyncio.set_event_loop(loop)
        loop.run_until_complete(pyfuncitem.obj(**pyfuncitem.funcargs))
    finally:
        asyncio.set_event_loop(None)
        loop.close()
    return True


def pytest_configure(config: pytest.Config) -> None:  # pragma: no cover - pytest hook
    config.addinivalue_line("markers", "asyncio: execute the test inside an event loop")
