import os
import sys
import tkinter as tk
from tkinter import filedialog, messagebox

if sys.platform != 'win32':
    raise OSError('This application only runs on Windows.')

STARTUP_DIR = os.path.join(os.environ['APPDATA'], r'Microsoft\Windows\Start Menu\Programs\Startup')


def install_startup_video(video_path):
    os.makedirs(STARTUP_DIR, exist_ok=True)
    bat_path = os.path.join(STARTUP_DIR, 'play_boot_video.bat')
    with open(bat_path, 'w') as f:
        f.write(f'@echo off\nstart "" "{video_path}"\n')
    return bat_path


class BootVideoApp(tk.Tk):
    def __init__(self):
        super().__init__()
        self.title('HP Boot Video')
        tk.Label(self, text='Select a video to play when Windows starts.').pack(padx=10, pady=10)
        tk.Button(self, text='Choose Video', command=self.choose_video).pack(padx=10, pady=5)
        tk.Button(self, text='Exit', command=self.destroy).pack(padx=10, pady=5)

    def choose_video(self):
        path = filedialog.askopenfilename(filetypes=[('Video Files', '*.mp4;*.avi;*.mkv;*.mov')])
        if not path:
            return
        try:
            script = install_startup_video(path)
            messagebox.showinfo('Done', f'A startup script was created:\n{script}\nThis will play the video after login.\nChanging the actual HP boot screen is not supported.')
        except Exception as e:
            messagebox.showerror('Error', str(e))


def main():
    app = BootVideoApp()
    app.mainloop()


if __name__ == '__main__':
    main()

