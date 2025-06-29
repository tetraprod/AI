"""Language pack loader."""

from . import en, es, fr, de

LANGUAGES = {
    'en': en.PROMPTS,
    'es': es.PROMPTS,
    'fr': fr.PROMPTS,
    'de': de.PROMPTS,
}


def get_prompts(lang: str = 'en') -> dict:
    """Return prompts for the given language code."""
    return LANGUAGES.get(lang, en.PROMPTS)
