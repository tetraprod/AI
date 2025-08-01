from sre_engine import core_from_text, generate_response, DivergentEmpathyPool, ArchetypeEngine, VisitorMemory

messages = [
    "Hello there",
    "I feel nervous about tomorrow",
    "But maybe it'll be fine!",
    "What do you think?",
]

dep = DivergentEmpathyPool()
engine = ArchetypeEngine()
mem = VisitorMemory()

for msg in messages:
    core = core_from_text(msg)
    mem.memory[core.visitor_id] = core
    dep.add_entry(core)
    print("You:", msg)
    print("SRE:", generate_response(core))

cluster = dep.detect_emergent_patterns()
if cluster:
    archetype = engine.propose_archetype(cluster)
    engine.integrate_archetype(archetype)
