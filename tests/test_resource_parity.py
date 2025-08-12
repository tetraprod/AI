from pathlib import Path
import sys

sys.path.append(str(Path(__file__).resolve().parents[1]))

from unifiedai import SoulEngine


def test_basic_allocation():
    engine = SoulEngine(total_capacity=40)
    engine.register_resident("user1", baseline=10)
    engine.register_resident("user2", baseline=10)

    engine.update_need("user1", 5)
    engine.update_need("user2", 25)
    engine.analyze_resource_needs()

    assert engine.resource_clouds["user1"] == 10
    assert engine.resource_clouds["user2"] == 30


def test_no_extra_need():
    engine = SoulEngine(total_capacity=20)
    engine.register_resident("a", baseline=10)
    engine.register_resident("b", baseline=10)

    engine.update_need("a", 8)
    engine.update_need("b", 9)
    engine.analyze_resource_needs()

    assert engine.resource_clouds["a"] == 10
    assert engine.resource_clouds["b"] == 10
