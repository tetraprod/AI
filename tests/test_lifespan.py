import pytest
from pathlib import Path
import sys
sys.path.append(str(Path(__file__).resolve().parents[1]))
from unified_ai import UnifiedAI, lifespan
from unittest.mock import AsyncMock


@pytest.mark.asyncio
async def test_lifespan_calls_close():
    engine = UnifiedAI()
    engine.connect = AsyncMock()
    engine.close = AsyncMock()

    async with lifespan(engine=engine):
        engine.connect.assert_awaited_once()

    engine.close.assert_awaited_once()
