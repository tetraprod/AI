import sys
import os
sys.path.append(os.path.dirname(os.path.dirname(__file__)))
from sre_engine import core_from_text, generate_response


def test_core_fields():
    core = core_from_text("Hello world! This is great")
    assert core.tone_bias in {"positive", "excited", "neutral", "curious", "negative", "anxious"}
    assert 0 <= core.subjectivity <= 1
    assert 0 <= core.certainty <= 1
    assert core.metaphor_field


def test_generate_response():
    core = core_from_text("I am happy")
    reply = generate_response(core)
    assert isinstance(reply, str)
