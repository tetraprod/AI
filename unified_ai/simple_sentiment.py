"""Lightweight text analysis utilities used across the project.

This module avoids heavyweight NLP dependencies by relying on a small
collection of keyword heuristics.  The goal is not to provide nuanced
language understanding, but to supply deterministic signals for tests
and simple demos while keeping the runtime footprint tiny.
"""

from __future__ import annotations

from dataclasses import dataclass
import re
from typing import Iterable, List


TOKEN_PATTERN = re.compile(r"[A-Za-z']+")

POSITIVE_WORDS = {
    "happy",
    "great",
    "good",
    "love",
    "wonderful",
    "excited",
    "excellent",
    "fantastic",
    "joy",
    "glad",
    "amazing",
    "awesome",
}

NEGATIVE_WORDS = {
    "sad",
    "bad",
    "angry",
    "hate",
    "upset",
    "terrible",
    "horrible",
    "anxious",
    "worried",
    "afraid",
    "scared",
    "pain",
}

SUBJECTIVE_WORDS = {
    "feel",
    "think",
    "believe",
    "hope",
    "maybe",
    "seems",
    "guess",
    "personally",
}

UNCERTAIN_WORDS = {
    "maybe",
    "perhaps",
    "unsure",
    "uncertain",
    "doubt",
    "guess",
    "probably",
}


@dataclass(frozen=True)
class SentimentSignals:
    tokens: List[str]
    score: int
    positives: int
    negatives: int
    subjectivity: float
    certainty: float


def _tokenise(text: str) -> List[str]:
    return [match.group(0).lower() for match in TOKEN_PATTERN.finditer(text)]


def _count_matches(tokens: Iterable[str], vocabulary: set[str]) -> int:
    return sum(1 for token in tokens if token in vocabulary)


def analyse_text(text: str) -> SentimentSignals:
    """Return coarse sentiment signals for *text*.

    The heuristics are intentionally simple but deterministic.  They favour
    clarity over linguistic sophistication which keeps the tests stable and
    avoids any optional third-party dependencies.
    """

    tokens = _tokenise(text)
    positives = _count_matches(tokens, POSITIVE_WORDS)
    negatives = _count_matches(tokens, NEGATIVE_WORDS)
    score = positives - negatives

    subjective_tokens = positives + negatives + _count_matches(tokens, SUBJECTIVE_WORDS)
    emphasis_bonus = min(0.3, 0.1 * text.count("!"))
    if tokens:
        subjectivity = min(1.0, (subjective_tokens / len(tokens)) + emphasis_bonus)
    else:
        subjectivity = 0.0

    uncertainty = _count_matches(tokens, UNCERTAIN_WORDS)
    if tokens:
        certainty = 0.6 + min(0.3, abs(score) / len(tokens)) - min(0.4, uncertainty / len(tokens))
    else:
        certainty = 0.6
    certainty = max(0.0, min(1.0, certainty))

    return SentimentSignals(
        tokens=tokens,
        score=score,
        positives=positives,
        negatives=negatives,
        subjectivity=subjectivity,
        certainty=certainty,
    )


def tone_from_signals(signals: SentimentSignals, text: str) -> str:
    """Translate ``SentimentSignals`` into one of the supported tone labels."""

    score = signals.score
    if "?" in text and abs(score) <= 1:
        return "curious"
    if score >= 3 or (score >= 2 and text.count("!") >= 1):
        return "excited"
    if score >= 1:
        return "positive"
    if score <= -3 or any(word in text.lower() for word in ("worry", "anxious", "panic")):
        return "anxious"
    if score <= -1:
        return "negative"
    return "neutral"


def top_tokens(tokens: Iterable[str], limit: int = 5) -> List[str]:
    seen: set[str] = set()
    ordered: List[str] = []
    for token in tokens:
        if token not in seen:
            seen.add(token)
            ordered.append(token)
        if len(ordered) >= limit:
            break
    if not ordered:
        ordered = ["echo"]
    return ordered
