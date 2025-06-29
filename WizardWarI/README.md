# Wizard War I (WWI)

This directory contains a minimal skeleton for an Unreal Engine project. It does not include game assets or full gameplay logic, but provides starting source files for building the 1-on-1 spell combat game described in the repository.

## Overview

* Players chain `Power`, `Area`, and `Effect` tokens to cast spells.
* Each arm can hold a chain of tokens for dual wielding.
* Tokens are acquired through combat and target practice.
* The game is played in a first-person view.
* The left and right triggers cast spells from the left and right arms.
* At level 5, players can assign token chains to quick slots and switch them with `X` (left arm) or `Y` (right arm).
* At level 10, the left and right buttons unlock powerful double-effect attacks.
* Character creation and local multiplayer are planned features.
* Online multiplayer lets players wager token chains against a challenger.
* An arena battle mode supports up to twenty entrants. Each wizard adds their
  stake to a common pool and anyone still alive after sixty seconds earns back
  double their bet. Fallen players forfeit their tokens, which remain in the
  pool until a daily fifty-player deathmatch awards the entire stockpile to one
  victor.

* Experience rewards scale with match length. Matches over 30 seconds grant double XP and those over 90 seconds grant quadruple XP. Levels require progressively more XP with a cap of 1000. At level 666 a wizard turns evil, while level 777 grants a holy white appearance.

* A comprehensive character creation menu lets players fine tune their wizard's height, eye and hair colour, skin tone, body build and hairstyle when starting a profile.
* Upon reaching level 1000, wizards instantly defeat opponents with any hit, even in multiplayer. Every level also adds a small amount of damage resistance.
* Player progress can be saved to a slot and loaded later.

Physics simulation is enabled for spells, characters and environmental objects so the `Power` and `Area` tokens scale force and size appropriately.

Levitation tokens combine with `Power` and `Area` tokens to keep an arm levitating for an entire duel. While active, they raise the wizard's movement speed according to the chosen power value, preventing quick slot switches on that arm.
Shield tokens can also be chained with `Power` tokens. Activating one locks the arm for the duel and projects a glowing barrier that reduces incoming damage based on the power value.

Use the controller's **Menu** button to open the character menu. Here you customise appearance, manage your token inventory and assign quick-load chains. Each arm begins with three slots and gains three more every ten levels.

Spells inflict special effects on opponents: water freezes them, electricity stuns and drops them, fire burns with an animation, and explosive spells launch them with strong physics.

Each elemental effect token prompts a different hand gesture and a reserved movement adjustment when cast: earth nudges the caster down, air lifts them slightly, fire steps them back, water slows them, electricity speeds them up for a moment and weapon summons lunge forward.
Facial animation accompanies these gestures so the wizard's face reflects the casting style while their eyes remain focused on the opponent.
Spells also spawn coloured point lights that scale with their power value, enhancing scene lighting and creating dramatic shadows.
Spells emit 3D surround sound cues so the audio matches their position in the arena.
The game plays "In the Hall of the Mountain King" on the main menu for atmospheric background music.
Gameplay is tuned for a smooth **60 FPS** and the frame rate is capped with the `t.MaxFPS 60` console command during play.

## Building

1. Install Unreal Engine 5 (or later) from the Epic Games Launcher.
2. Clone this repository and open `WizardWarI.uproject` in the Unreal Editor.
3. Use `File > Generate Visual Studio project files` if needed.
4. Build the project from the editor or via command line:
   ```sh
   /path/to/UnrealEngine/Engine/Build/BatchFiles/RunUAT.sh \
     BuildCookRun -project="$(pwd)/WizardWarI.uproject" \
     -noP4 -platform=Win64 -clientconfig=Development \
     -cook -allmaps -build -stage -pak -archive -archivedirectory="Build"
   ```
5. The packaged game will appear in the `Build` directory.
6. For best visuals, open **Project Settings > Rendering** and enable Lumen
   Global Illumination and Hardware Ray Tracing so the spell lighting looks
   vibrant.

## Note

Compilation requires Unreal Engine which is not included in this repository. The project is provided as a starting template only.
