import tkinter as tk
from tkinter import scrolledtext

from .model import load_model, generate_text
from .image_engine import create_image
from .network_interface import send_request
from .language_pack import en


class AIGUI(tk.Tk):
    def __init__(self):
        super().__init__()
        self.title('AI GUI')
        self.geometry('600x400')

        self.input_box = scrolledtext.ScrolledText(self, height=5)
        self.input_box.pack(fill=tk.BOTH, padx=5, pady=5)

        self.generate_button = tk.Button(self, text='Generate', command=self.on_generate)
        self.generate_button.pack(pady=5)

        self.output_box = scrolledtext.ScrolledText(self, height=10)
        self.output_box.pack(fill=tk.BOTH, padx=5, pady=5)

        self.model, self.tokenizer = load_model()

    def on_generate(self):
        prompt = self.input_box.get('1.0', tk.END).strip()
        if not prompt:
            return
        response = generate_text(prompt, self.model, self.tokenizer)
        self.output_box.insert(tk.END, response + '\n')
        # create a simple image with the response text
        create_image(response, 'output.png')
        # send a dummy network request with the prompt and response
        send_request({'prompt': prompt, 'response': response})


def main():
    app = AIGUI()
    app.mainloop()


if __name__ == '__main__':
    main()
