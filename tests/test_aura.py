import json
from pathlib import Path

from unified_ai.aura import AuraEngine


def test_load_rules(tmp_path):
    rules = {"extra_rule": "be excellent"}
    file = tmp_path / "rules.json"
    file.write_text(json.dumps(rules))
    engine = AuraEngine(rules_file=str(file))
    assert engine.ethics_rules["extra_rule"] == "be excellent"

