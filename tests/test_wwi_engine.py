import pytest

from wwi_engine.game_engine import GameEngine


def test_asset_quality_degrades_when_memory_low():
    engine = GameEngine(max_memory_bytes=6 * 1024 ** 2)  # 6MB limit
    quality = engine.load_asset("biplane", quality="high")
    assert quality == "low"
    summary = engine.summary()
    assert summary["asset_memory"] <= engine.max_memory_bytes


def test_physics_step_moves_object():
    engine = GameEngine()
    engine.add_physics_object("crate", position=(0.0, 10.0))
    engine.step_physics(1.0)
    obj = engine.physics_objects[0]
    assert obj.position[1] == pytest.approx(0.19, rel=1e-2)


def test_light_intensity_uses_inverse_square():
    engine = GameEngine()
    engine.add_light((0.0, 0.0), 100.0)
    intensity = engine.compute_lighting((0.0, 10.0))
    assert intensity == pytest.approx(1.0, rel=1e-3)
