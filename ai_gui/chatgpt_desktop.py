import os
import tkinter as tk
from tkinter import scrolledtext

import openai

OPENAI_API_KEY = os.getenv("OPENAI_API_KEY")
if not OPENAI_API_KEY:
    raise RuntimeError("Please set the OPENAI_API_KEY environment variable")

openai.api_key = OPENAI_API_KEY

class ChatGPTDesktop(tk.Tk):
    def __init__(self):
        super().__init__()
        self.title("ChatGPT Desktop Assistant")
        self.geometry("600x400")
        self.conversation = []

        self.output_box = scrolledtext.ScrolledText(self, height=15)
        self.output_box.pack(fill=tk.BOTH, padx=5, pady=5)
        self.output_box.insert(tk.END, "ChatGPT ready.\n")

        self.input_box = scrolledtext.ScrolledText(self, height=4)
        self.input_box.pack(fill=tk.BOTH, padx=5, pady=5)

        self.send_button = tk.Button(self, text="Send", command=self.on_send)
        self.send_button.pack(pady=5)

    def on_send(self):
        user_msg = self.input_box.get("1.0", tk.END).strip()
        if not user_msg:
            return
        self.input_box.delete("1.0", tk.END)
        self.output_box.insert(tk.END, f"You: {user_msg}\n")
        self.conversation.append({"role": "user", "content": user_msg})
        self.output_box.insert(tk.END, "ChatGPT: ...\n")
        self.output_box.see(tk.END)
        self.update()
        try:
            resp = openai.ChatCompletion.create(
                model="gpt-3.5-turbo",
                messages=self.conversation,
            )
            reply = resp["choices"][0]["message"]["content"].strip()
        except Exception as e:
            reply = f"Error: {e}"
        self.output_box.delete("end-2l", "end-1l")  # remove placeholder
        self.output_box.insert(tk.END, f"ChatGPT: {reply}\n")
        self.output_box.see(tk.END)
        self.conversation.append({"role": "assistant", "content": reply})


def main():
    app = ChatGPTDesktop()
    app.mainloop()


if __name__ == "__main__":
    main()
