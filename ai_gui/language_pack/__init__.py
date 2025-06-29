"""Language pack loader."""

from . import en, es

LANGUAGES = {
    'en': en.PROMPTS,
    'es': es.PROMPTS,
}


def get_prompts(lang: str = 'en') -> dict:
    """Return prompts for the given language code."""
    return LANGUAGES.get(lang, en.PROMPTS)
