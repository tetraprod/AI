import tkinter as tk
from tkinter import scrolledtext
import asyncio
import threading

from unified_ai import UnifiedAI


class UnifiedAIGUI(tk.Tk):
    """Tkinter interface to interact with UnifiedAI."""

    def __init__(self) -> None:
        super().__init__()
        self.title("UnifiedAI Control Panel")
        self.geometry("600x400")

        self.engine: UnifiedAI | None = None

        self.output_box = scrolledtext.ScrolledText(self, height=15, state=tk.DISABLED)
        self.output_box.pack(fill=tk.BOTH, padx=5, pady=5)

        self.input_box = tk.Entry(self)
        self.input_box.pack(fill=tk.X, padx=5, pady=5)
        self.input_box.bind("<Return>", self.on_send)

        btn_frame = tk.Frame(self)
        btn_frame.pack(pady=5)
        self.start_btn = tk.Button(btn_frame, text="Start", command=self.start_engine)
        self.start_btn.pack(side=tk.LEFT, padx=5)
        self.stop_btn = tk.Button(btn_frame, text="Stop", command=self.stop_engine, state=tk.DISABLED)
        self.stop_btn.pack(side=tk.LEFT, padx=5)
        self.send_btn = tk.Button(btn_frame, text="Send", command=self.on_send, state=tk.DISABLED)
        self.send_btn.pack(side=tk.LEFT, padx=5)

        self.protocol("WM_DELETE_WINDOW", self.on_close)

    def log(self, message: str) -> None:
        self.output_box.configure(state=tk.NORMAL)
        self.output_box.insert(tk.END, message + "\n")
        self.output_box.configure(state=tk.DISABLED)
        self.output_box.see(tk.END)

    def async_task(self, coro) -> None:
        threading.Thread(target=lambda: asyncio.run(coro), daemon=True).start()

    def start_engine(self) -> None:
        if self.engine is not None:
            return

        async def _start():
            self.engine = UnifiedAI()
            await self.engine.connect()
            await self.engine.initialize()
            self.log("Engine started.")
            self.start_btn.config(state=tk.DISABLED)
            self.stop_btn.config(state=tk.NORMAL)
            self.send_btn.config(state=tk.NORMAL)

        self.log("Starting engine ...")
        self.async_task(_start())

    def stop_engine(self) -> None:
        if self.engine is None:
            return

        async def _stop():
            await self.engine.close()
            self.engine = None
            self.log("Engine stopped.")
            self.start_btn.config(state=tk.NORMAL)
            self.stop_btn.config(state=tk.DISABLED)
            self.send_btn.config(state=tk.DISABLED)

        self.log("Stopping engine ...")
        self.async_task(_stop())

    def on_send(self, event=None) -> None:  # noqa: ANN001
        text = self.input_box.get().strip()
        if not text or self.engine is None:
            return
        self.input_box.delete(0, tk.END)
        self.log(f"You: {text}")

        async def _interact():
            try:
                reply = await self.engine.interact(text)
            except Exception as exc:  # pragma: no cover - defensive
                reply = f"Error: {exc}"
            self.log(f"AI: {reply}")

        self.async_task(_interact())

    def on_close(self) -> None:
        if self.engine is not None:
            asyncio.run(self.engine.close())
        self.destroy()


def main() -> None:
    app = UnifiedAIGUI()
    app.mainloop()


if __name__ == "__main__":
    main()
