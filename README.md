# AI GUI

This project contains a minimal example of a Python GUI application that uses a language model for text generation, provides simple image creation, and demonstrates a network request. The GUI is built with Tkinter, and the model uses the `transformers` library.

## Features

- Text generation using a small pretrained model (distilgpt2)
- Image creation using Pillow
- Network interface example using `requests`
- Simple language pack structure

## Running

Install the dependencies and run the GUI:

```bash
pip install -r requirements.txt
python -m ai_gui.app
```

## HTML Interface

A basic web interface is provided in the `html` directory. Open `html/index.html` in a browser or serve the folder with a simple HTTP server:

```bash
python -m http.server --directory html
```

The page allows you to send a prompt to the `/generate` endpoint of a backend server and displays the response.

## Language Packs

The GUI includes a simple language pack system. Prompts are available in English (`en`) and Spanish (`es`). Retrieve them with:

```python
from ai_gui.language_pack import get_prompts
prompts = get_prompts('es')
```

The default language is English if an unknown code is provided.
