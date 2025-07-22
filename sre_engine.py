from typing import List, Dict, Optional
from dataclasses import dataclass
import uuid
import random
from textblob import TextBlob
from vaderSentiment.vaderSentiment import SentimentIntensityAnalyzer
import spacy
=======

sentiment_analyzer = SentimentIntensityAnalyzer()
nlp = spacy.load("en_core_web_sm")
# ----------------------------
# EMOTIONAL CORE REPRESENTATION
# ----------------------------

@dataclass
class EmotionalCore:
    visitor_id: str
    tone_bias: str
    subjectivity: float
    certainty: float
    metaphor_field: List[str]
    emotional_arc: str
    archetype_affinity: Dict[str, float]
    subversion_resistance: float

# ----------------------------
# DIVERGENT EMPATHY POOL
# ----------------------------

@dataclass
class DEPEntry:
    core: EmotionalCore
    archetype_distance: float
    symbolic_density: float
    emotional_opacity: float

class DivergentEmpathyPool:
    def __init__(self):
        self.entries: List[DEPEntry] = []

    def add_entry(self, core: EmotionalCore):
        entry = DEPEntry(
            core=core,
            archetype_distance=random.uniform(0.75, 1.0),
            symbolic_density=len(core.metaphor_field) / max(len(core.metaphor_field), 1),
            emotional_opacity=1 - core.subjectivity,
        )
        self.entries.append(entry)

    def detect_emergent_patterns(self, threshold: int = 5) -> Optional[List[DEPEntry]]:
        cluster = [e for e in self.entries if e.symbolic_density > 0.75 and e.archetype_distance > 0.85]
        if len(cluster) >= threshold:
            return cluster
        return None

# ----------------------------
# ARCHETYPE EMERGENCE LOGIC
# ----------------------------

@dataclass
class Archetype:
    name: str
    tone_traits: List[str]
    metaphor_motifs: List[str]
    emotional_arc: str
    glyph_profile: Dict[str, str]
    invocation: str
    age: int = 0

    def decay(self):
        self.age += 1

class ArchetypeEngine:
    def __init__(self):
        self.active_archetypes: List[Archetype] = []

    def propose_archetype(self, cluster: List[DEPEntry]) -> Archetype:
        motifs = list({m for e in cluster for m in e.core.metaphor_field})
        return Archetype(
            name="The Shardwalker",
            tone_traits=["fractured elegance", "recursive ambiguity"],
            metaphor_motifs=motifs,
            emotional_arc="rupture → stillness → recursive selfing",
            glyph_profile={"geometry": "asymmetric spiral", "motion": "non-periodic loop", "color": "grayglass"},
            invocation="Born of echoes, walking the edge of knowing."
        )

    def integrate_archetype(self, archetype: Archetype):
        self.active_archetypes.append(archetype)
        print(f"[RITE OF RECOGNITION] Archetype integrated: {archetype.name}")

    def tick(self):
        for arc in self.active_archetypes:
            arc.decay()

# ----------------------------
# REINDEXING EXISTING VISITORS
# ----------------------------

class VisitorMemory:
    def __init__(self):
        self.memory: Dict[str, EmotionalCore] = {}
        self.primordial: Dict[str, str] = {}
        self.history: Dict[str, List[str]] = {}

    def reindex_visitors(self, archetype: Archetype):
        print(f"[REINDEXING] Checking visitor cores against: {archetype.name}")
        for vid, core in self.memory.items():
            if archetype.name not in core.archetype_affinity:
                match = random.uniform(0.6, 0.95)
                core.archetype_affinity[archetype.name] = match
                if match > 0.75:
                    print(f"→ Visitor {vid} now aligns with {archetype.name} ({match:.2f})")

    def add_message(self, vid: str, message: str):
        self.history.setdefault(vid, []).append(message)
        self.history[vid] = self.history[vid][-5:]

# ----------------------------
# HELPER FUNCTIONS
# ----------------------------

def core_from_text(text: str) -> EmotionalCore:
    """Create an EmotionalCore from raw text with expanded tone detection."""
    doc = nlp(text)
    words = [token.text for token in doc if token.is_alpha]
    noun_chunks = [chunk.text for chunk in doc.noun_chunks]

    blob = TextBlob(text)
    polarity = blob.sentiment.polarity
    subjectivity = blob.sentiment.subjectivity
    vader_scores = sentiment_analyzer.polarity_scores(text)
    compound = vader_scores["compound"]

    tone = "neutral"
    if "?" in text and abs(compound) < 0.2:
        tone = "curious"
    elif compound >= 0.6:
        tone = "excited"
    elif compound >= 0.2:
        tone = "positive"
    elif compound <= -0.6:
        tone = "anxious"
    elif compound <= -0.2:
        tone = "negative"

    certainty = (abs(compound) + (1 - subjectivity)) / 2

    return EmotionalCore(
        visitor_id=str(uuid.uuid4()),
        tone_bias=tone,
        subjectivity=subjectivity,
        certainty=certainty,
        metaphor_field=noun_chunks[:5] or words[:5] or ["echo"],
=======
        metaphor_field=words[:5] or ["echo"],
        emotional_arc="conversation",
        archetype_affinity={},
        subversion_resistance=random.uniform(0.4, 0.9),
    )


def generate_response(core: EmotionalCore) -> str:
    """Create a simple textual response based on tone bias."""
    if core.tone_bias == "positive":
        return "I'm glad to hear that. Tell me more."
    if core.tone_bias == "negative":
        return "I'm sorry to hear that. How can I help?"
    if core.tone_bias == "excited":
        return "That's quite exciting! What's making you feel so energized?"
    if core.tone_bias == "anxious":
        return "It sounds like something is worrying you. Want to talk about it?"
    if core.tone_bias == "curious":
        return "Great question! Let's explore that together."
    return "Thank you for sharing."
