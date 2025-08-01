import tkinter as tk
from tkinter import scrolledtext
import random

from sre_engine import (
    DivergentEmpathyPool,
    ArchetypeEngine,
    VisitorMemory,
    core_from_text,
    generate_response,
)
from brain_engine import BrainEngine

THINKING_MESSAGES = [
    "SRE is thinking...",
    "Hmm...",
    "Processing...",
    "Let me think...",
    "Hold on...",
]

THINKING_MESSAGES = [
    "SRE is thinking...",
    "Hmm...",
    "Processing...",
    "Let me think...",
    "Hold on...",
]

THINKING_MESSAGES = [
    "SRE is thinking...",
    "Hmm...",
    "Processing...",
    "Let me think...",
    "Hold on...",
]


class SREChatGUI(tk.Tk):
    """Simple Tkinter chat interface for the SRE engine."""

    def __init__(self):
        super().__init__()
        self.title("SRE Chatbot")
        self.geometry("600x400")

        self.dep = DivergentEmpathyPool()
        self.engine = ArchetypeEngine()
        self.memory = VisitorMemory()
        self.brain = BrainEngine()

        self.output_box = scrolledtext.ScrolledText(self, height=15)
        self.output_box.pack(fill=tk.BOTH, padx=5, pady=5)
        self.output_box.insert(tk.END, "SRE ready.\n")
        self.output_box.tag_config("positive", foreground="green")
        self.output_box.tag_config("negative", foreground="red")
        self.output_box.tag_config("excited", foreground="orange")
        self.output_box.tag_config("anxious", foreground="purple")
        self.output_box.tag_config("curious", foreground="blue")
        self.output_box.tag_config("neutral", foreground="black")

        self.input_box = scrolledtext.ScrolledText(self, height=4)
        self.input_box.pack(fill=tk.BOTH, padx=5, pady=5)
        # Pressing Enter in the input box will send the message
        self.input_box.bind("<Return>", self.on_enter)

        self.send_button = tk.Button(self, text="Send", command=self.on_send)
        self.send_button.pack(pady=5)

    def on_send(self):
        text = self.input_box.get("1.0", tk.END).strip()
        if not text:
            return
        self.input_box.delete("1.0", tk.END)
        self.output_box.insert(tk.END, f"You: {text}\n")
        self.output_box.see(tk.END)

        placeholder = random.choice(THINKING_MESSAGES)
        self.output_box.insert(tk.END, f"SRE: {placeholder}\n")
        self.output_box.see(tk.END)
        self.update()

        core = core_from_text(text)
        self.memory.memory[core.visitor_id] = core
        if core.visitor_id not in self.memory.primordial:
            self.memory.primordial[core.visitor_id] = "Wanderer"
        self.memory.add_message(core.visitor_id, text)
        self.dep.add_entry(core)

        reply = generate_response(core)
        self.output_box.delete("end-2l", "end-1l")  # remove placeholder
        self.output_box.insert(tk.END, f"SRE: {reply}\n", core.tone_bias)
        self.output_box.see(tk.END)
=======
=======
        self.output_box.insert(tk.END, f"SRE: {reply}\n")
        self.output_box.see(tk.END)
=======

        # Demonstrate BrainEngine integration
        self.brain.learn(reply)

        cluster = self.dep.detect_emergent_patterns()
        if cluster:
            archetype = self.engine.propose_archetype(cluster)
            self.engine.integrate_archetype(archetype)
            self.memory.reindex_visitors(archetype)
            self.output_box.insert(
                tk.END,
                f"[Archetype Emerged] {archetype.name}: {archetype.invocation}\n",
            )
            self.output_box.see(tk.END)

    def on_enter(self, event):
        """Handle Return keypress in the input box."""
        self.on_send()
        return "break"


def main():
    app = SREChatGUI()
    app.mainloop()


if __name__ == "__main__":
    main()
