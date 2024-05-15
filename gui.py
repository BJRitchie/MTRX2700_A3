import tkinter as tk
from tkinter import messagebox
import threading
import time

class TimerApp:
    def __init__(self, master):
        self.master = master
        self.master.title("Countdown Timer")
        
        # Variables for timer control
        self.start_flag = 0
        self.stop_flag = 0
        self.timer_active = False
        self.leaderboard = []
        
        # Timer display
        self.time_label = tk.Label(self.master, text="00:00", font=("Helvetica", 48))
        self.time_label.pack(pady=20)
        
        # Start and stop buttons
        start_button = tk.Button(self.master, text="Start", command=self.start_timer, font=("Helvetica", 14))
        start_button.pack(side=tk.LEFT, padx=20)
        
        stop_button = tk.Button(self.master, text="Stop", command=self.stop_timer, font=("Helvetica", 14))
        stop_button.pack(side=tk.RIGHT, padx=20)
        
        # Leaderboard
        self.leaderboard_label = tk.Label(self.master, text="Leaderboard:\n", font=("Helvetica", 14))
        self.leaderboard_label.pack(pady=20)
        
        self.update_leaderboard()

    def start_timer(self):
        if not self.timer_active:
            self.start_flag = 1
            self.stop_flag = 0
            self.start_time = time.time()
            self.timer_active = True
            threading.Thread(target=self.run_timer).start()

    def stop_timer(self):
        if self.timer_active:
            self.stop_flag = 1
            self.timer_active = False
            elapsed_time = time.time() - self.start_time
            self.leaderboard.append(elapsed_time)
            self.leaderboard.sort()
            self.update_leaderboard()

    def run_timer(self):
        while self.start_flag and not self.stop_flag:
            elapsed_time = time.time() - self.start_time
            mins, secs = divmod(int(elapsed_time), 60)
            time_format = '{:02}:{:02}'.format(mins, secs)
            self.time_label.config(text=time_format)
            time.sleep(1)
        self.start_flag = 0

    def update_leaderboard(self):
        leaderboard_text = "Leaderboard:\n"
        for idx, score in enumerate(self.leaderboard[:5]):
            leaderboard_text += f"{idx + 1}. {score:.2f} seconds\n"
        self.leaderboard_label.config(text=leaderboard_text)

if __name__ == "__main__":
    root = tk.Tk()
    app = TimerApp(root)
    root.mainloop()
