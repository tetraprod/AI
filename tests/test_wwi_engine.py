from wwi_engine.game_engine import GameEngine


def test_asset_quality_degrades_when_memory_low():
    engine = GameEngine(max_memory_bytes=6 * 1024 ** 2)  # 6MB limit
    quality = engine.load_asset("biplane", quality="high")
    assert quality == "low"
    summary = engine.summary()
    assert summary["asset_memory"] <= engine.max_memory_bytes
