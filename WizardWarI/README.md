# Wizard War I (WWI)

This directory contains a minimal skeleton for an Unreal Engine project. It does not include game assets or full gameplay logic, but provides starting source files for building the 1-on-1 spell combat game described in the repository.

## Overview

* Players chain `Power`, `Area`, and `Effect` tokens to cast spells.
* Each arm can hold a chain of tokens for dual wielding.
* Tokens are acquired through combat and target practice.
* Character creation and local multiplayer are planned features.
* Planned single-player mode lets you duel randomly generated wizards.
  Winning a match awards one **Power** token, one **Area** token, and two
  random **Effect** tokens to expand your arsenal.

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

## Note

Compilation requires Unreal Engine which is not included in this repository. The project is provided as a starting template only.

## Roadmap

The project currently contains only a basic UE 5 setup. Future updates will focus on adopting modern engine features and improving game quality.

* **Nanite** – Rebuild environment and character meshes with Nanite to support extremely high polygon counts while keeping draw calls low.
* **Lumen** – Enable real-time global illumination and reflections using Lumen with fallbacks for older hardware.
* **Niagara VFX** – Migrate all spell and ambient effects to Niagara for GPU-accelerated simulations.

### Development Goals

* **High-fidelity assets** – Target film-quality materials and textures optimized for Nanite.
* **Multiplayer replication** – Ensure reliable replication of movement and spell effects for online duels.
* **Performance targets** – Maintain 60 FPS at 1080p on mid-range GPUs (around an RTX 2070) with scalability options for lower-end machines.
