import os
import sys
import json
import tkinter as tk
from tkinter import filedialog, messagebox, font

if sys.platform != 'win32':
    raise OSError('WinJack only runs on Windows.')

try:
    import winreg
    import winsound
except ImportError:
    messagebox.showerror('Error', 'winreg and winsound modules are required on Windows.')
    raise

CONFIG_PATH = os.path.join(os.path.expanduser('~'), '.winjack.json')
CONTEXT_MENU_PATH = r'Software\Classes\Directory\Background\shell'
RUN_KEY = r'Software\Microsoft\Windows\CurrentVersion\Run'


def load_config():
    if os.path.exists(CONFIG_PATH):
        with open(CONFIG_PATH, 'r') as f:
            return json.load(f)
    return {}


def save_config(cfg):
    with open(CONFIG_PATH, 'w') as f:
        json.dump(cfg, f, indent=2)


def add_context_menu_item(name, command):
    with winreg.CreateKey(winreg.HKEY_CURRENT_USER, f"{CONTEXT_MENU_PATH}\\{name}\\command") as key:
        winreg.SetValue(key, '', winreg.REG_SZ, command)


def remove_context_menu_item(name):
    try:
        winreg.DeleteKey(winreg.HKEY_CURRENT_USER, f"{CONTEXT_MENU_PATH}\\{name}\\command")
        winreg.DeleteKey(winreg.HKEY_CURRENT_USER, f"{CONTEXT_MENU_PATH}\\{name}")
    except FileNotFoundError:
        pass


def list_context_menu_items():
    items = []
    try:
        with winreg.OpenKey(winreg.HKEY_CURRENT_USER, CONTEXT_MENU_PATH) as key:
            i = 0
            while True:
                try:
                    items.append(winreg.EnumKey(key, i))
                    i += 1
                except OSError:
                    break
    except FileNotFoundError:
        pass
    return items


def get_startup_items():
    items = {}
    try:
        with winreg.OpenKey(winreg.HKEY_CURRENT_USER, RUN_KEY) as key:
            i = 0
            while True:
                try:
                    name, value, _ = winreg.EnumValue(key, i)
                    items[name] = value
                    i += 1
                except OSError:
                    break
    except FileNotFoundError:
        pass
    return items


def add_startup_item(name, path):
    with winreg.CreateKey(winreg.HKEY_CURRENT_USER, RUN_KEY) as key:
        winreg.SetValueEx(key, name, 0, winreg.REG_SZ, path)


def remove_startup_item(name):
    try:
        with winreg.OpenKey(winreg.HKEY_CURRENT_USER, RUN_KEY, 0, winreg.KEY_SET_VALUE) as key:
            winreg.DeleteValue(key, name)
    except FileNotFoundError:
        pass


def toggle_privacy(block):
    # Placeholder privacy protection (camera/mic) using registry
    value = 0 if block else 1
    path = r'Software\Microsoft\Windows\CurrentVersion\CapabilityAccessManager\ConsentStore'
    for dev in ['microphone', 'webcam']:
        try:
            with winreg.CreateKey(winreg.HKEY_CURRENT_USER, f"{path}\\{dev}") as key:
                winreg.SetValueEx(key, 'Value', 0, winreg.REG_SZ, 'Deny' if block else 'Allow')
        except PermissionError:
            pass


class WinJack(tk.Tk):
    def __init__(self):
        super().__init__()
        self.title('WinJack')
        winsound.MessageBeep()
        self.config = load_config()
        self.script_buttons = self.config.get('scripts', [])

        tk.Button(self, text='Add Context Item', command=self.add_context).pack(fill='x')
        tk.Button(self, text='Remove Context Item', command=self.remove_context).pack(fill='x')
        tk.Button(self, text='Manage Startup', command=self.manage_startup).pack(fill='x')
        tk.Button(self, text='Change System Font', command=self.change_font).pack(fill='x')
        tk.Button(self, text='Toggle Camera/Mic Lock', command=self.toggle_privacy).pack(fill='x')
        tk.Button(self, text='Exit', command=self.on_close).pack(fill='x')

        self.protocol('WM_DELETE_WINDOW', self.on_close)

    def add_context(self):
        if len(self.script_buttons) >= 10:
            messagebox.showerror('Error', 'Maximum of 10 script buttons reached.')
            return
        name = tk.simpledialog.askstring('Name', 'Menu item name:')
        if not name:
            return
        path = filedialog.askopenfilename(title='Select script')
        if not path:
            return
        add_context_menu_item(name, path)
        self.script_buttons.append({'name': name, 'path': path})
        winsound.MessageBeep()

    def remove_context(self):
        items = list_context_menu_items()
        if not items:
            messagebox.showinfo('Info', 'No custom items found.')
            return
        name = tk.simpledialog.askstring('Remove Item', f'Enter item name:\nAvailable: {", ".join(items)}')
        if name:
            remove_context_menu_item(name)
            self.script_buttons = [s for s in self.script_buttons if s['name'] != name]
            winsound.MessageBeep()

    def manage_startup(self):
        items = get_startup_items()
        choice = tk.simpledialog.askstring('Startup Items', f'Current items:\n{items}\nEnter name to toggle or new name:')
        if not choice:
            return
        if choice in items:
            remove_startup_item(choice)
        else:
            path = filedialog.askopenfilename(title='Select executable')
            if path:
                add_startup_item(choice, path)
        winsound.MessageBeep()

    def change_font(self):
        fonts = sorted(font.families())
        top = tk.Toplevel(self)
        top.title('Select Font')
        listbox = tk.Listbox(top)
        for f in fonts:
            listbox.insert('end', f)
        listbox.pack(fill='both', expand=True)

        def apply():
            sel = listbox.get('active')
            if sel:
                self.config['font'] = sel
                # Example registry modification for system font
                for name in ['MS Shell Dlg', 'MS Shell Dlg 2']:
                    try:
                        with winreg.OpenKey(winreg.HKEY_LOCAL_MACHINE, r'SOFTWARE\Microsoft\Windows NT\CurrentVersion\FontSubstitutes', 0, winreg.KEY_SET_VALUE) as key:
                            winreg.SetValueEx(key, name, 0, winreg.REG_SZ, sel)
                    except PermissionError:
                        messagebox.showwarning('Warning', 'Permission denied setting system font.')
                winsound.MessageBeep()
                top.destroy()

        tk.Button(top, text='Apply', command=apply).pack()

    def toggle_privacy(self):
        block = not self.config.get('privacy_block', False)
        toggle_privacy(block)
        self.config['privacy_block'] = block
        status = 'blocked' if block else 'allowed'
        messagebox.showinfo('Privacy', f'Camera and microphone {status}.')
        winsound.MessageBeep()

    def on_close(self):
        save_config({'scripts': self.script_buttons, 'font': self.config.get('font'), 'privacy_block': self.config.get('privacy_block', False)})
        winsound.MessageBeep()
        self.destroy()


def main():
    app = WinJack()
    app.mainloop()


if __name__ == '__main__':
    main()
