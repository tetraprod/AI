import json
import logging


class AuraEngine:
    """Ethical oversight and contextual awareness."""

    def __init__(self, rules_file: str = "ethics_rules.json") -> None:
        self.rules_file = rules_file
        self.logger = logging.getLogger(self.__class__.__name__)
        self.ethics_rules = {
            "no_harm": "Do not generate harmful content",
            "no_bias": "Avoid biased or discriminatory language",
            "privacy": "Protect user data and privacy",
        }
        self.load_rules()

    def load_rules(self) -> None:
        try:
            with open(self.rules_file, "r", encoding="utf-8") as f:
                self.ethics_rules.update(json.load(f))
        except FileNotFoundError:
            self.logger.warning("Ethics rules file not found, using defaults")
        except Exception as exc:  # pragma: no cover - defensive
            self.logger.error("Failed to load ethics rules: %s", exc)

    async def validate_output(self, output: str) -> bool:
        lowered = output.lower()
        if "harm" in lowered and "no_harm" in self.ethics_rules:
            return False
        if any(word in lowered for word in ["bias", "discriminate"]):
            return False
        return True
