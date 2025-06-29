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

## Wizard War I Skeleton


A minimal Unreal Engine project is available in the `WizardWarI` directory. It provides starter source files for a 1-on-1 spell combat game with Xbox controller support, including example input bindings for casting with each arm. The game is designed from a first-person perspective. Once players reach level five they can swap token quick slots using the `X` button for the left arm and `Y` for the right. See `WizardWarI/README.md` for build instructions.

The project enables physics simulation for characters, spells and environmental objects so effects like power and area directly influence spell behavior.

Levitation tokens may be chained with power and area tokens. Once activated on an arm they cannot be swapped out for the rest of the match and increase movement speed based on the selected power token.
Shield tokens likewise lock the chosen arm but surround the caster with a translucent barrier. Their damage reduction scales with the attached power token.

Elemental effect tokens now cause subtle posture changes and small movement nudges when cast. Earth spells push the player slightly downward, air gently lifts them, fire prompts a short backward step, water slows briefly, electricity gives a small burst of speed and weapon summons perform a short lunge.
Each effect also plays a short facial expression animation while the caster keeps their eyes locked on the opponent.

An online multiplayer mode allows players to wager token chains against each other. A challenger can host a bet match with a set of tokens and another player may join if they can stake an identical chain. The winner receives all wagered tokens.

Experience is earned based on how long each duel lasts. Time under thirty seconds awards one XP per second, while matches longer than thirty seconds double the gain and those past ninety seconds double it again. The level cap is 1000 with special titles at 666 and 777.

Players can now design their wizard using a full character creation screen that includes size sliders, skin and hair colour pickers, eye colour selection, body type and hairstyle options.
Reaching level 1000 grants automatic one-hit kills even in online matches, turning every duel into a quick draw.
Each level also provides a small amount of additional damage resistance.
Progress can be saved to a slot and loaded later through a basic save system.

Spells now emit dynamic lighting that matches their element colour and intensity,
enhancing shading when Lumen global illumination is enabled in your project.
Spells also play positional surround sound effects for added immersion, and the
main menu features the classic "In the Hall of the Mountain King" as background
music.
=======
=======
A minimal Unreal Engine project is available in the `WizardWarI` directory. It provides starter source files for a 1-on-1 spell combat game with Xbox controller support. See `WizardWarI/README.md` for build instructions.


