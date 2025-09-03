"""Tkinter-based GUI workshop for building WWI-themed scenes."""
from __future__ import annotations

import tkinter as tk
from tkinter import messagebox

from .game_engine import GameEngine


class WWIWorkshop(tk.Tk):
    """GUI for assembling WWI scenes using :class:`GameEngine`."""

    def __init__(self, engine: GameEngine | None = None):
        super().__init__()
        self.title("WWI Workshop")
        self.engine = engine or GameEngine()
        self.status_var = tk.StringVar(value="Ready")

        self._build_menu()
        self._build_status()

    def _build_menu(self) -> None:
        menu = tk.Menu(self)
        self.config(menu=menu)

        asset_menu = tk.Menu(menu, tearoff=0)
        asset_menu.add_command(label="Add Trench", command=lambda: self._add_asset("trench"))
        asset_menu.add_command(label="Add Biplane", command=lambda: self._add_asset("biplane"))
        asset_menu.add_command(label="Add Tank", command=lambda: self._add_asset("tank"))
        menu.add_cascade(label="Assets", menu=asset_menu)

        light_menu = tk.Menu(menu, tearoff=0)
        light_menu.add_command(label="Add Light", command=self._add_light)
        menu.add_cascade(label="Lighting", menu=light_menu)

        physics_menu = tk.Menu(menu, tearoff=0)
        physics_menu.add_command(label="Drop Crate", command=self._add_crate)
        physics_menu.add_command(label="Step Physics", command=self._step_physics)
        menu.add_cascade(label="Physics", menu=physics_menu)

    def _build_status(self) -> None:
        status_bar = tk.Label(self, textvariable=self.status_var, bd=1, relief=tk.SUNKEN, anchor=tk.W)
        status_bar.pack(side=tk.BOTTOM, fill=tk.X)

    def _add_asset(self, name: str) -> None:
        try:
            quality = self.engine.load_asset(name)
            self.status_var.set(f"Added {name} in {quality} quality")
        except MemoryError:
            messagebox.showerror("Memory Limit", "Cannot add asset; memory limit reached.")

    def _add_light(self) -> None:
        self.engine.add_light((0.0, 0.0), 100.0)
        self.status_var.set("Added light at origin")

    def _add_crate(self) -> None:
        self.engine.add_physics_object("crate", position=(0.0, 10.0))
        self.status_var.set("Dropped crate from height 10")

    def _step_physics(self) -> None:
        self.engine.step_physics(1.0)
        if self.engine.physics_objects:
            y = self.engine.physics_objects[0].position[1]
            self.status_var.set(f"Physics stepped; crate y={y:.2f}")



if __name__ == "__main__":
    workshop = WWIWorkshop()
    workshop.mainloop()
