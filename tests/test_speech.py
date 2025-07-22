import pytest
from pathlib import Path
import sys
sys.path.append(str(Path(__file__).resolve().parents[1]))
from unified_ai import UnifiedAI
from unittest.mock import AsyncMock


def test_speech_engine_created():
    engine = UnifiedAI()
    assert hasattr(engine, "speech")
    assert engine.speech.optical is engine.optical


@pytest.mark.asyncio
async def test_interact_sequence(monkeypatch):
    engine = UnifiedAI()
    call_order = []

    async def aura_validate_output(text):
        call_order.append(f"aura:{text}")
        return True

    async def analyze_emotion(text):
        call_order.append(f"soul_analyze:{text}")
        return "neutral"

    async def reason(text):
        call_order.append(f"brain:{text}")
        return "memory"

    async def transfer(data, target):
        call_order.append(f"optical:{data}")
        return True

    async def craft_reply(text, emo):
        call_order.append(f"soul_craft:{text},{emo}")
        return "reply"

    async def speech_analyze(text):
        call_order.append(f"speech_analyze:{text}")
        return "analysis"

    async def speech_synth(analysis):
        call_order.append(f"speech_synthesize:{analysis}")
        return "audio"

    async def speech_transmit(audio, target="SpeechOutput"):
        call_order.append(f"speech_transmit:{audio}")
        return True

    monkeypatch.setattr(engine.aura, "validate_output", AsyncMock(side_effect=aura_validate_output))
    monkeypatch.setattr(engine.soul, "analyze_emotion", AsyncMock(side_effect=analyze_emotion))
    monkeypatch.setattr(engine.brain, "reason", AsyncMock(side_effect=reason))
    monkeypatch.setattr(engine.optical, "transfer_data", AsyncMock(side_effect=transfer))
    monkeypatch.setattr(engine.soul, "craft_reply", AsyncMock(side_effect=craft_reply))
    monkeypatch.setattr(engine.speech, "analyze_text", AsyncMock(side_effect=speech_analyze))
    monkeypatch.setattr(engine.speech, "synthesize_speech", AsyncMock(side_effect=speech_synth))
    monkeypatch.setattr(engine.speech, "transmit_speech", AsyncMock(side_effect=speech_transmit))

    result = await engine.interact("hi")

    assert result == "reply"
    assert call_order == [
        "aura:hi",
        "soul_analyze:hi",
        "brain:hi",
        "optical:memory",
        "aura:memory",
        "soul_craft:memory,neutral",
        "speech_analyze:reply",
        "speech_synthesize:analysis",
        "speech_transmit:audio",
    ]
