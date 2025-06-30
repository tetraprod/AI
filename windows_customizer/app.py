import json
import os
import sys
import tkinter as tk
from tkinter import filedialog, colorchooser, messagebox
from PIL import Image, ImageDraw

if sys.platform != 'win32':
    raise OSError('This application only runs on Windows.')

try:
    import winreg
    import ctypes
except ImportError:
    messagebox.showerror('Error', 'winreg and ctypes modules are required on Windows.')
    raise

CONFIG_PATH = os.path.join(os.path.expanduser('~'), '.win_customizer.json')


def load_config():
    if os.path.exists(CONFIG_PATH):
        with open(CONFIG_PATH, 'r') as f:
            return json.load(f)
    return {}


def save_config(cfg):
    with open(CONFIG_PATH, 'w') as f:
        json.dump(cfg, f, indent=2)


def set_registry_value(root, path, name, value, value_type=winreg.REG_DWORD):
    with winreg.CreateKey(root, path) as key:
        winreg.SetValueEx(key, name, 0, value_type, value)


def apply_accent_color(color):
    r, g, b = color
    value = (b << 16) | (g << 8) | r
    set_registry_value(winreg.HKEY_CURRENT_USER,
                       r'SOFTWARE\Microsoft\Windows\DWM',
                       'AccentColor', value)


def apply_dark_start_menu(enable=True):
    value = 0 if enable else 1
    set_registry_value(winreg.HKEY_CURRENT_USER,
                       r'SOFTWARE\Microsoft\Windows\CurrentVersion\Themes\Personalize',
                       'SystemUsesLightTheme', value)
    set_registry_value(winreg.HKEY_CURRENT_USER,
                       r'SOFTWARE\Microsoft\Windows\CurrentVersion\Themes\Personalize',
                       'AppsUseLightTheme', value)


def set_boot_background(image_path):
    dest_dir = os.path.join(os.environ.get('WINDIR', 'C:\\Windows'), 'System32', 'oobe', 'info', 'backgrounds')
    os.makedirs(dest_dir, exist_ok=True)
    dest = os.path.join(dest_dir, 'backgroundDefault.jpg')
    Image.open(image_path).resize((1920, 1080)).save(dest)
    set_registry_value(winreg.HKEY_LOCAL_MACHINE,
                       r'SOFTWARE\Microsoft\Windows\CurrentVersion\Authentication\LogonUI\Background',
                       'OEMBackground', 1)


def set_cursor(image_path):
    dest = os.path.join(os.environ.get('WINDIR', 'C:\\Windows'), 'Cursors', 'custom.cur')
    Image.open(image_path).save(dest)
    with winreg.OpenKey(winreg.HKEY_CURRENT_USER, r'Control Panel\Cursors', 0, winreg.KEY_SET_VALUE) as key:
        winreg.SetValueEx(key, 'Arrow', 0, winreg.REG_SZ, dest)
    # apply system cursor change
    ctypes.windll.user32.SystemParametersInfoW(0x0057, 0, dest, 1)


class DrawPad(tk.Toplevel):
    def __init__(self, master, callback):
        super().__init__(master)
        self.title('Draw Cursor')
        self.canvas = tk.Canvas(self, width=32, height=32, bg='white')
        self.canvas.pack()
        self.img = Image.new('RGBA', (32, 32), (255, 255, 255, 0))
        self.draw = ImageDraw.Draw(self.img)
        self.canvas.bind('<B1-Motion>', self.paint)
        self.callback = callback
        tk.Button(self, text='Save', command=self.save).pack()

    def paint(self, event):
        x, y = event.x, event.y
        self.canvas.create_oval(x, y, x+2, y+2, fill='black')
        self.draw.ellipse([x, y, x+2, y+2], fill='black')

    def save(self):
        temp = os.path.join(os.path.expanduser('~'), 'cursor.png')
        self.img.save(temp)
        self.callback(temp)
        self.destroy()


class CustomizerApp(tk.Tk):
    def __init__(self):
        super().__init__()
        self.title('Windows Customizer')
        self.config = load_config()

        tk.Button(self, text='Accent Color', command=self.choose_color).pack(fill='x')
        tk.Button(self, text='Dark Start Menu', command=self.toggle_dark).pack(fill='x')
        tk.Button(self, text='Set Boot Background', command=self.choose_boot_image).pack(fill='x')
        tk.Button(self, text='Draw Cursor', command=self.open_draw_pad).pack(fill='x')
        tk.Button(self, text='Exit', command=self.on_close).pack(fill='x')

        self.protocol('WM_DELETE_WINDOW', self.on_close)

    def choose_color(self):
        color = colorchooser.askcolor()[0]
        if color:
            apply_accent_color(tuple(int(c) for c in color))
            self.config['accent_color'] = color

    def toggle_dark(self):
        apply_dark_start_menu(True)
        self.config['dark_start_menu'] = True

    def choose_boot_image(self):
        path = filedialog.askopenfilename(filetypes=[('JPEG', '*.jpg;*.jpeg'), ('PNG', '*.png')])
        if path:
            set_boot_background(path)
            self.config['boot_image'] = path

    def open_draw_pad(self):
        DrawPad(self, self.save_cursor)

    def save_cursor(self, path):
        set_cursor(path)
        self.config['cursor'] = path

    def on_close(self):
        save_config(self.config)
        self.destroy()


def main():
    app = CustomizerApp()
    app.mainloop()


if __name__ == '__main__':
    main()
