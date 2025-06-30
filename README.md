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

The GUI includes a simple language pack system. Prompts are available in American English (`en`, the default), Spanish (`es`), French (`fr`) and German (`de`). Retrieve them with:

```python
from ai_gui.language_pack import get_prompts
prompts = get_prompts('es')
```

The default language is American English if an unknown code is provided.

## Windows Customizer

The repository also includes a small Windows-only GUI that can tweak a few
appearance settings. It lets you pick a new accent colour, switch the Start menu
to the dark theme, set a custom logon background and draw a mouse cursor. Your
choices are saved to `~/.win_customizer.json`.

This app relies on the **Pillow** library. Install all dependencies with
`pip install -r requirements.txt` or install Pillow individually using
`pip install pillow`.

Run it with:

```bash
python -m windows_customizer.app
```

To bundle the customizer as a standalone executable run PyInstaller on a
Windows machine:

```bash
pip install pyinstaller
pyinstaller windows_customizer/app.py --onefile
```

The generated binary will be placed in the `dist` folder as `app.exe`.

## Wizard War I Skeleton

A minimal Unreal Engine project is available in the `WizardWarI` directory. It provides starter source files for a 1-on-1 spell combat game with Xbox controller support, including example input bindings for casting with each arm. The game is designed from a first-person perspective. Once players reach level five they can swap token quick slots using the `X` button for the left arm and `Y` for the right. A settings screen on the main menu lets you remap these controls and adjust audio or video preferences. See `WizardWarI/README.md` for build instructions.

The project enables physics simulation for characters, spells and environmental objects so effects like power and area directly influence spell behavior.

Interface text is localized. American English is the default with Spanish,
French and German translations also available. Change your language in the game
settings.

Levitation tokens may be chained with power and area tokens. Once activated on an arm they cannot be swapped out for the rest of the match and increase movement speed based on the selected power token.
Shield tokens likewise lock the chosen arm but surround the caster with a translucent barrier. Their damage reduction scales with the attached power token.
Area tokens now offer a **Self** option for personal effects like shields and levitation. When levitating, the wizard rises slightly off the ground with the height determined by the chosen power token.

Each token displays a small symbol and emits a coloured glow matching its attribute so you can quickly recognise power, area and elemental effects.

Companion tokens can be spent to unlock hell hound allies. Each hound tier costs more tokens and grants more health and damage. Hounds take damage like any foe and, when one falls, another leaps in to replace it. You may swap the active hound in the character menu between rounds.
Tokens are also used in a simple store where you can buy new hounds or robe styles. Robes provide attack and defence bonuses. Everyone automatically owns a colourful **tie dye robe** that unlocks a room-filling fireball by pulling **both triggers** at once, printing "I didn't ask how big the room is, I said I cast fireball!" for dramatic flair. A special **Speedy Robe** can also be purchased which shrinks you to one tenth size and triples your speed when both triggers are pressed. While this robe is active, your spells only deal **10 %** of their normal power and area.

Opponents react to elements: water spells freeze them in place, electricity knocks them down stunned (interrupting the spell they were casting), fire sets them ablaze and explosive spells send them flying with a big physics impulse.

Press the controller's **Menu** button to open the character menu where you arrange quick-slot chains and manage a large token inventory. Each arm starts with three slots and gains three more every ten levels.

As you experiment with new token combinations the game automatically records each custom spell in a personal spell log accessible from this menu. Every time you discover a unique chain you'll unlock an achievement and can give the spell a custom name.
=======
The default language is English if an unknown code is provided.

## Wizard War I Skeleton


=======

=======

=======


A minimal Unreal Engine project is available in the `WizardWarI` directory. It provides starter source files for a 1-on-1 spell combat game with Xbox controller support, including example input bindings for casting with each arm. The game is designed from a first-person perspective. Once players reach level five they can swap token quick slots using the `X` button for the left arm and `Y` for the right. See `WizardWarI/README.md` for build instructions.

The project enables physics simulation for characters, spells and environmental objects so effects like power and area directly influence spell behavior.

Levitation tokens may be chained with power and area tokens. Once activated on an arm they cannot be swapped out for the rest of the match and increase movement speed based on the selected power token.
Shield tokens likewise lock the chosen arm but surround the caster with a translucent barrier. Their damage reduction scales with the attached power token.


=======


Opponents react to elements: water spells freeze them in place, electricity knocks them down stunned, fire sets them ablaze and explosive spells send them flying with a big physics impulse.

Press the controller's **Menu** button to open the character menu where you arrange quick-slot chains and manage a large token inventory. Each arm starts with three slots and gains three more every ten levels.


=======
=======


Elemental effect tokens now cause subtle posture changes and small movement nudges when cast. Earth spells push the player slightly downward, air gently lifts them, fire prompts a short backward step, water slows briefly, electricity gives a small burst of speed and weapon summons perform a short lunge.
Each effect also plays a short facial expression animation while the caster keeps their eyes locked on the opponent.

An online multiplayer mode allows players to wager token chains against each other. A challenger can host a bet match with a set of tokens and another player may join if they can stake an identical chain. The winner receives all wagered tokens.

=======

=======


For larger matches an **arena battle** mode supports up to twenty players. Each
contestant adds their wager to a shared pool when joining. Anyone who survives
sixty seconds takes back their tokens plus an equal bonus. Eliminated players
forfeit their stake, leaving it in the pool. Once per day the accumulated pool
is offered in a fifty‑player deathmatch where the last wizard standing wins the
entire hoard.

Duels take place in a random arena chosen from a sprawling dungeon, a dense
forest, a shrinking island, a grand Roman colosseum or a steep mountain top.
The selected environment determines where both players spawn but otherwise has
no bearing on mechanics in this starter project.
=======

=======
=======


Experience is earned based on how long each duel lasts. Time under thirty seconds awards one XP per second, while matches longer than thirty seconds double the gain and those past ninety seconds double it again. The level cap is 1000 with special titles at 666 and 777.

Players can now design their wizard using a full character creation screen that includes size sliders, skin and hair colour pickers, eye colour selection, body type and hairstyle options.
Reaching level 1000 grants automatic one-hit kills even in online matches, turning every duel into a quick draw.
Each level also provides a small amount of additional damage resistance.
Progress can be saved to a slot and loaded later through a basic save system.
=======

=======

=======



Spells now emit dynamic lighting that matches their element colour and intensity,
enhancing shading when Lumen global illumination is enabled in your project.
Spells also play positional surround sound effects for added immersion, and the
main menu features the classic "In the Hall of the Mountain King" as background
music.

Between matches your wizard relaxes in a small **home castle** that acts as a
waiting room. Here you can open a chest to review your token stash, admire a
display of collected robes and browse books listing every spell you've learned
along with earned achievements.

Taunts are fully customisable. Purchase extra shout attacks in the token store
then edit the accompanying text in the character menu. Press the left thumbstick
to bellow the equipped taunt; any swear words are automatically censored.
=======

To keep gameplay smooth the project locks the frame rate to **60 FPS** when the
match starts. The cap can be adjusted using the `t.MaxFPS` console command if
desired.
=======
=======
=======
=======
=======
A minimal Unreal Engine project is available in the `WizardWarI` directory. It provides starter source files for a 1-on-1 spell combat game with Xbox controller support. See `WizardWarI/README.md` for build instructions.





