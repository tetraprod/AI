from typing import List, Dict, Optional
from dataclasses import dataclass
import uuid
import random
from textblob import TextBlob

# ----------------------------
# EMOTIONAL CORE REPRESENTATION
# ----------------------------

@dataclass
class EmotionalCore:
    visitor_id: str
    tone_bias: str
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
            symbolic_density=random.uniform(0.6, 1.0),
            emotional_opacity=random.uniform(0.5, 1.0),
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

# ----------------------------
# REINDEXING EXISTING VISITORS
# ----------------------------

class VisitorMemory:
    def __init__(self):
        self.memory: Dict[str, EmotionalCore] = {}

    def reindex_visitors(self, archetype: Archetype):
        print(f"[REINDEXING] Checking visitor cores against: {archetype.name}")
        for vid, core in self.memory.items():
            if archetype.name not in core.archetype_affinity:
                match = random.uniform(0.6, 0.95)
                core.archetype_affinity[archetype.name] = match
                if match > 0.75:
                    print(f"→ Visitor {vid} now aligns with {archetype.name} ({match:.2f})")

# ----------------------------
# HELPER FUNCTIONS
# ----------------------------

def core_from_text(text: str) -> EmotionalCore:
    """Create an EmotionalCore from raw text with basic sentiment analysis."""
    words = [w.strip() for w in text.split() if w.strip()]
    blob = TextBlob(text)
    polarity = blob.sentiment.polarity
    if polarity > 0.3:
        tone = "positive"
    elif polarity < -0.3:
        tone = "negative"
    else:
        tone = "neutral"
    return EmotionalCore(
        visitor_id=str(uuid.uuid4()),
        tone_bias=tone,
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
    return "Thank you for sharing."
