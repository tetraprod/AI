import logging
from textblob import TextBlob

try:
    from transformers import pipeline
except Exception:  # pragma: no cover - optional dependency
    pipeline = None


class SoulEngine:
    """Handle empathetic interactions and emotional analysis."""

    def __init__(self) -> None:
        self.logger = logging.getLogger(self.__class__.__name__)
        if pipeline:
            # pragma: no cover - model download can be slow
            self.classifier = pipeline(
                "text-classification", model="distilbert-base-uncased-emotion"
            )
        else:
            self.classifier = None

    async def analyze_emotion(self, text: str) -> str:
        """Return an emotion label for the text."""
        if self.classifier:
            try:
                result = self.classifier(text)[0]
                return result["label"].lower()
            except Exception as exc:  # pragma: no cover - defensive
                self.logger.error("Emotion analysis failed: %s", exc)
                return "neutral"
        polarity = TextBlob(text).sentiment.polarity
        if polarity > 0.2:
            return "positive"
        if polarity < -0.2:
            return "negative"
        return "neutral"

    async def craft_reply(self, text: str, emotion: str) -> str:
        """Craft an empathetic reply based on detected emotion."""
        responses = {
            "joy": "That's wonderful!",
            "sadness": "I'm here for you.",
            "anger": "I understand your frustration.",
            "fear": "That sounds scary.",
            "surprise": "That's surprising!",
            "neutral": "I see.",
            "positive": "I'm glad to hear that!",
            "negative": "I'm sorry to hear that.",
        }
        base = responses.get(emotion, "I see.")
        return f"{base} You said: {text}"
