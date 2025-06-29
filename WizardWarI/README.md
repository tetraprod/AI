# Wizard War I (WWI)

This directory contains a minimal skeleton for an Unreal Engine project. It does not include game assets or full gameplay logic, but provides starting source files for building the 1-on-1 spell combat game described in the repository.

## Overview

* Players chain `Power`, `Area`, and `Effect` tokens to cast spells.
* Each arm can hold a chain of tokens for dual wielding.
* Tokens are acquired through combat and target practice.


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
