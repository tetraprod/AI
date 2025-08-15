import cv2
import tkinter as tk

# --------------------------- GUI setup --------------------------- #
root = tk.Tk()
root.title("Webcam Magnifier")

# control variables bound to GUI widgets
region_size = tk.IntVar(value=100)    # size of region to zoom
zoom_factor = tk.DoubleVar(value=2.0) # zoom level
magnify_on = tk.BooleanVar(value=False)

# slider to change the area of magnification
size_label = tk.Label(root, text="Region Size")
size_label.pack()
size_scale = tk.Scale(root, from_=40, to=400, orient=tk.HORIZONTAL,
                      variable=region_size, length=300)
size_scale.pack()

# slider for the zoom level
zoom_label = tk.Label(root, text="Zoom Factor")
zoom_label.pack()
zoom_scale = tk.Scale(root, from_=1, to=5, resolution=0.1, orient=tk.HORIZONTAL,
                      variable=zoom_factor, length=300)
zoom_scale.pack()

# checkbutton to toggle magnification overlay
magnify_check = tk.Checkbutton(root, text="Magnification On/Off",
                               variable=magnify_on)
magnify_check.pack(pady=5)

# ------------------------- Video capture ------------------------- #
cap = cv2.VideoCapture(0)


def update_frame():
    """Read frame, apply magnification if enabled, display it and reschedule."""
    ret, frame = cap.read()
    if not ret:
        # reschedule quickly if frame not read to keep loop alive
        root.after(15, update_frame)
        return

    if magnify_on.get():
        size = region_size.get()
        zoom = zoom_factor.get()

        h, w = frame.shape[:2]
        cx, cy = w // 2, h // 2  # center of the frame
        half = size // 2
        x1, y1 = max(0, cx - half), max(0, cy - half)
        x2, y2 = min(w, cx + half), min(h, cy + half)

        # draw rectangle showing area to be magnified
        cv2.rectangle(frame, (x1, y1), (x2, y2), (0, 255, 0), 2)

        roi = frame[y1:y2, x1:x2]
        if roi.size > 0:
            # resize the region to create a zoom effect
            zoomed = cv2.resize(roi, None, fx=zoom, fy=zoom,
                                interpolation=cv2.INTER_LINEAR)
            zh, zw = zoomed.shape[:2]
            zh = min(zh, h)
            zw = min(zw, w)
            # overlay zoomed window in top-left corner
            frame[0:zh, 0:zw] = zoomed[0:zh, 0:zw]

    # display frame
    cv2.imshow("Webcam", frame)
    cv2.waitKey(1)
    # schedule next frame update
    root.after(15, update_frame)


def on_close():
    """Cleanup when the window is closed."""
    cap.release()
    cv2.destroyAllWindows()
    root.destroy()

root.protocol("WM_DELETE_WINDOW", on_close)

# start the loop
root.after(0, update_frame)
root.mainloop()
