#!/usr/bin/env bash
# Example build script for Wizard War I
# Requires Unreal Engine installed and environment variables set.
UAT_PATH="${UE_PATH:-/path/to/UnrealEngine}/Engine/Build/BatchFiles/RunUAT.sh"
"$UAT_PATH" BuildCookRun -project="$(pwd)/WizardWarI.uproject" -noP4 -platform=Win64 -clientconfig=Development -cook -allmaps -build -stage -pak -archive -archivedirectory="Build"
