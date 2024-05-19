import tkinter as tk
import time
import serial
import threading

SERIAL_PORT = 'COM6'  
BAUD_RATE = 115200

class TimerApp:
    def __init__(self, root):
        self.root = root
        self.root.title("Timer with Leaderboard")
        
        self.timer_label = tk.Label(root, text="0.0", font=("Helvetica", 48))
        self.timer_label.pack()

        self.message_label = tk.Label(root, text="", font=("Helvetica", 24))
        self.message_label.pack()
        
        self.leaderboard_label = tk.Label(root, text="Leaderboard", font=("Helvetica", 24))
        self.leaderboard_label.pack()
        
        self.leaderboard = tk.Listbox(root)
        self.leaderboard.pack()

        self.start_time = None
        self.running = False
        self.checkpoints = [0, 0, 0]
        self.leaderboard_times = []

        # Start serial communication thread
        self.serial_thread = threading.Thread(target=self.read_serial)
        self.serial_thread.daemon = True
        self.serial_thread.start()
        
    def start_timer(self):
        self.start_time = time.time()
        self.running = True
        self.update_timer()

    def stop_timer(self):
        self.running = False
        end_time = time.time() - self.start_time
        self.leaderboard_times.append(end_time)
        self.leaderboard_times.sort()
        self.update_leaderboard()

    def update_timer(self):
        if self.running:
            elapsed_time = time.time() - self.start_time
            self.timer_label.config(text=f"{elapsed_time:.1f}")
            self.root.after(100, self.update_timer)
            
    def update_leaderboard(self):
        self.leaderboard.delete(0, tk.END)
        for idx, time in enumerate(self.leaderboard_times[:10], start=1):  # Show top 10 times
            self.leaderboard.insert(tk.END, f"{idx}. {time:.1f} sec")

    def read_serial(self):
        ser = serial.Serial(SERIAL_PORT, BAUD_RATE)
        while True:
            if ser.in_waiting > 0:
                line = ser.readline().decode('utf-8').strip()
                values = list(map(int, line.split(',')))

                # Check if the first number is 2 to start the timer
                if values[0] == 2 and not self.running:
                    self.start_timer()
                    self.checkpoints = [0, 0, 0]
                    self.message_label.config(text="")
                
                # Check if the first number is 3 to stop the timer
                elif values[0] == 3 and self.running:
                    self.stop_timer()
                
                # Check if all middle three numbers are 1 for the checkpoint
                if self.running and values[1:4] == [1, 1, 1]:
                    self.message_label.config(text="Checkpoint reached")

if __name__ == "__main__":
    root = tk.Tk()
    app = TimerApp(root)
    root.mainloop()
