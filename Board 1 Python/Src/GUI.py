import tkinter as tk
from tkinter import ttk
import time
import serial
import threading
import pygame
from PIL import Image, ImageTk

# Define baud rate and the serial port number
SERIAL_PORT = 'COM5'
BAUD_RATE = 115200

# Initialise pygame mixer
pygame.mixer.init()

# Creating the timer application pop-up GUI
class TimerApp:
    def __init__(self, root):
        self.root = root
        self.root.title("The a-MAZE-ing Race!")

        # Set up the background image
        self.background_image = Image.open("game-background.jpg")
        self.background_photo = ImageTk.PhotoImage(self.background_image)
        self.background_label = tk.Label(root, image=self.background_photo)
        self.background_label.place(relwidth=1, relheight=1)

        # Set up styles
        self.style = ttk.Style()
        self.style.configure('TLabel', font=('Helvetica', 24), background='lightgrey')
        self.style.configure('TButton', font=('Helvetica', 16), background='white')

        # Title label
        self.title_label = ttk.Label(root, text="The a-MAZE-ing Race!", font=("Helvetica", 36), background='lightgrey')
        self.title_label.pack(pady=20)

        # Timer label
        self.timer_label = ttk.Label(root, text="0.0", font=("Helvetica", 48), background='lightgrey')
        self.timer_label.pack(pady=20)

        # Message labels
        self.checkpoint1_label = ttk.Label(root, text="", font=("Helvetica", 24), background='lightgrey')
        self.checkpoint1_label.pack(pady=5)
        self.checkpoint2_label = ttk.Label(root, text="", font=("Helvetica", 24), background='lightgrey')
        self.checkpoint2_label.pack(pady=5)
        self.checkpoint3_label = ttk.Label(root, text="", font=("Helvetica", 24), background='lightgrey')
        self.checkpoint3_label.pack(pady=5)

        # Leaderboard label
        self.leaderboard_label = ttk.Label(root, text="Leaderboard", font=("Helvetica", 24), background='lightgrey')
        self.leaderboard_label.pack(pady=10)

        # Leaderboard listbox with scrollbar
        self.leaderboard_frame = ttk.Frame(root)
        self.leaderboard_frame.pack(pady=10)
        self.leaderboard_scrollbar = ttk.Scrollbar(self.leaderboard_frame)
        self.leaderboard_scrollbar.pack(side=tk.RIGHT, fill=tk.Y)
        self.leaderboard = tk.Listbox(self.leaderboard_frame, font=("Helvetica", 16), yscrollcommand=self.leaderboard_scrollbar.set, width=30, height=10)
        self.leaderboard.pack(side=tk.LEFT, fill=tk.BOTH)
        self.leaderboard_scrollbar.config(command=self.leaderboard.yview)

        self.start_time = None
        self.running = False
        self.previous_values = [0, 0, 0]
        self.leaderboard_entries = []

        # Start serial communication thread
        self.serial_thread = threading.Thread(target=self.read_serial)
        self.serial_thread.daemon = True
        self.serial_thread.start()
    
    # Function to start the GUI timer and sound
    def start_timer(self):
        self.start_time = time.time()
        self.running = True
        self.update_timer()
        # Play music when the timer starts
        pygame.mixer.music.load("Gusty_Garden.mp3")
        pygame.mixer.music.play(-1)  # Play the music in a loop

    # Function to stop the GUI timer and sound
    def stop_timer(self):
        self.running = False
        end_time = time.time() - self.start_time
        self.prompt_for_name(end_time)
        # Stop the music and play the completion sound
        pygame.mixer.music.stop()
        pygame.mixer.Sound("mixkit-completion-of-a-level-2063.wav").play()

    # Function to update the time display
    def update_timer(self):
        if self.running:
            elapsed_time = time.time() - self.start_time
            self.timer_label.config(text=f"{elapsed_time:.1f}")
            self.root.after(100, self.update_timer)

    # Function to ask for the users name after the game is complete to be aded to the leaderboard submission
    def prompt_for_name(self, time):
        def submit_name():
            name = name_entry.get()
            self.leaderboard_entries.append((time, name))
            self.leaderboard_entries.sort()
            self.update_leaderboard()
            name_window.destroy()

        name_window = tk.Toplevel(self.root)
        name_window.title("Enter Your Name")
        tk.Label(name_window, text="Enter your name:").pack(pady=10)
        name_entry = tk.Entry(name_window)
        name_entry.pack(pady=10)
        submit_button = tk.Button(name_window, text="Submit", command=submit_name)
        submit_button.pack(pady=10)

    # Updates the leaderboard when the name is inputted
    def update_leaderboard(self):
        self.leaderboard.delete(0, tk.END)
        for idx, (time, name) in enumerate(self.leaderboard_entries[:10], start=1):  # Show top 10 times
            self.leaderboard.insert(tk.END, f"{idx}. {name}: {time:.1f} sec")

    # Reading the serial data sent from the STM32CubeIDE code to determine the game state
    def read_serial(self):
        ser = serial.Serial(SERIAL_PORT, BAUD_RATE)
        while True:
            if ser.in_waiting > 0:
                line = ser.readline().decode('utf-8').strip()
                values = list(map(int, line.split(',')))

                # Check if the first number is 2 to start the timer
                if values[0] == 2 and not self.running:
                    self.start_timer()
                    self.previous_values = [0, 0, 0]
                    self.checkpoint1_label.config(text="")
                    self.checkpoint2_label.config(text="")
                    self.checkpoint3_label.config(text="")
                
                # Check if the first number is 3 to stop the timer
                elif values[0] == 3 and self.running:
                    self.stop_timer()
                
                # Check for checkpoint changes from 0 to 1
                for i in range(1, 4):
                    if self.previous_values[i-1] == 0 and values[i] == 1:
                        if i == 1:
                            self.checkpoint1_label.config(text="Checkpoint 1 reached")
                        elif i == 2:
                            self.checkpoint2_label.config(text="Checkpoint 2 reached")
                        elif i == 3:
                            self.checkpoint3_label.config(text="Checkpoint 3 reached")
                        pygame.mixer.Sound("mixkit-arcade-video-game-bonus-2044.wav").play()
                
                self.previous_values = values[1:4]

# Main function to run the GUI
if __name__ == "__main__":
    root = tk.Tk()
    app = TimerApp(root)
    root.mainloop()

