from transformers import AutoModelForCausalLM, AutoTokenizer
import torch

MODEL_NAME = 'distilgpt2'


def load_model(model_name: str = MODEL_NAME):
    """Load a small language model for text generation."""
    tokenizer = AutoTokenizer.from_pretrained(model_name)
    model = AutoModelForCausalLM.from_pretrained(model_name)
    return model, tokenizer


def generate_text(prompt: str, model, tokenizer, max_length: int = 50) -> str:
    inputs = tokenizer.encode(prompt, return_tensors='pt')
    outputs = model.generate(inputs, max_length=max_length, num_return_sequences=1)
    text = tokenizer.decode(outputs[0], skip_special_tokens=True)
    # Only return the new generated part
    return text[len(prompt):].strip()
