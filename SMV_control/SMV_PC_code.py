import tkinter as tk
import asyncio
import websockets
from PIL import Image, ImageTk
import io
import threading

class RobotControlApp:
    def __init__(self, root):
        self.root = root
        self.root.title("Robot Control GUI")
        self.websocket = None
        self.loop = asyncio.new_event_loop()
        threading.Thread(target=self.start_event_loop, daemon=True).start()

        # Main-Layout
        self.root.rowconfigure(0, weight=1)
        self.root.columnconfigure(0, weight=1)
        self.root.columnconfigure(1, weight=2)

        # Left site
        self.left_frame = tk.Frame(root, bg="lightgray", padx=10, pady=10)
        self.left_frame.grid(row=0, column=0, sticky="nsew")

        self.status_label = tk.Label(self.left_frame, text="Status: Not Connected", bg="lightgray", font=("Arial", 12), anchor="w")
        self.status_label.pack(fill="x", pady=5)

        self.command_log = tk.Text(self.left_frame, wrap="word", height=20, state="disabled", bg="white", font=("Arial", 10))
        self.command_log.pack(fill="both", expand=True, pady=10)

        # Right site
        self.right_frame = tk.Frame(root, bg="white")
        self.right_frame.grid(row=0, column=1, sticky="nsew")
        self.right_frame.rowconfigure(0, weight=3)
        self.right_frame.rowconfigure(1, weight=1)
        self.right_frame.columnconfigure(0, weight=1)

        # Videostream
        self.video_label = tk.Label(self.right_frame, text="Video Stream", bg="black", fg="white")
        self.video_label.grid(row=0, column=0, sticky="nsew", padx=5, pady=5)

        # Map
        self.map_canvas = tk.Canvas(self.right_frame, bg="white", width=400, height=300)
        self.map_canvas.grid(row=1, column=0, sticky="nsew", padx=5, pady=5)
        self.robot_position = [200, 150]
        self.robot_direction = "N"

        # Control
        self.active_keys = set()
        self.last_command = None 
        self.root.bind("<KeyPress>", self.on_key_press)
        self.root.bind("<KeyRelease>", self.on_key_release)
        self.servo_angle = 180

    def start_event_loop(self):
        asyncio.set_event_loop(self.loop)
        self.loop.run_forever()

    def connect_to_robot(self):
        self.status_label.config(text="Status: Connecting...")
        uri = "ws://192.168.4.1:81"  # Replace with your ESP32's IP if different
        asyncio.run_coroutine_threadsafe(self.establish_connection(uri), self.loop)

    async def establish_connection(self, uri):
        try:
            self.websocket = await websockets.connect(uri)
            self.status_label.config(text="Status: Connected")
            self.log_command("Connected to robot.")
            asyncio.create_task(self.receive_data())
        except Exception as e:
            self.status_label.config(text="Status: Connection Failed")
            self.log_command(f"Connection error: {e}")

    async def receive_data(self):
        while True:
            try:
                data = await self.websocket.recv()
                if isinstance(data, bytes):
                    self.update_video_stream(data)
                elif isinstance(data, str):
                    self.update_status(data)
            except Exception as e:
                self.log_command(f"Error receiving data: {e}")
                break

    def update_video_stream(self, frame_data):
        try:
            image = Image.open(io.BytesIO(frame_data))
            image = image.rotate(270)  # rotate videostream
            video_label_width = self.video_label.winfo_width()
            video_label_height = self.video_label.winfo_height()
            image = image.resize((video_label_width, video_label_height), Image.Resampling.LANCZOS)
            photo = ImageTk.PhotoImage(image)
            self.video_label.configure(image=photo)
            self.video_label.image = photo
        except Exception as e:
            self.log_command(f"Error processing video frame: {e}")

    def update_status(self, status_message):
        self.log_command(f"Status Update: {status_message}")

    def send_command(self, command):
        if self.websocket:
            asyncio.run_coroutine_threadsafe(self.websocket.send(command), self.loop)
            self.log_command(f"Command sent: {command}")
        else:
            self.log_command("Not connected to the robot.")

        if command in {"Vor", "Rueck", "Links", "Rechts"}: # forwards, backwards, left, right
            self.update_map(command)

    def log_command(self, message):
        self.command_log.config(state="normal")
        self.command_log.insert("end", message + "\n")
        self.command_log.see("end")
        self.command_log.config(state="disabled")

    def on_key_press(self, event):
        key = event.keysym.lower()
        if key in {"w", "a", "s", "d", "up", "down"} and key not in self.active_keys:
            self.active_keys.add(key)

            command = None
            if key == "w":
                command = "Vor"
            elif key == "a":
                command = "Links"
            elif key == "s":
                command = "Rueck"
            elif key == "d":
                command = "Rechts"
            elif key == "up":
                self.servo_angle = max(1, self.servo_angle - 10)  # reduce angle
                command = str(self.servo_angle)
            elif key == "down":
                self.servo_angle = min(180, self.servo_angle + 10)  # increase angle
                command = str(self.servo_angle)

            if command and command != self.last_command:
                self.send_command(command)
                self.last_command = command

    def on_key_release(self, event):
        key = event.keysym.lower()
        if key in self.active_keys:
            self.active_keys.remove(key)

            if not self.active_keys:
                self.send_command("Stop")
                self.last_command = "Stop"

    def update_map(self, command):
        x, y = self.robot_position

        if command == "Vor":
            if self.robot_direction == "N":
                y -= 20
            elif self.robot_direction == "S":
                y += 20
            elif self.robot_direction == "E":
                x += 20
            elif self.robot_direction == "W":
                x -= 20
        elif command == "Rueck":
            if self.robot_direction == "N":
                y += 20
            elif self.robot_direction == "S":
                y -= 20
            elif self.robot_direction == "E":
                x -= 20
            elif self.robot_direction == "W":
                x += 20
        elif command == "Links":
            self.robot_direction = {"N": "W", "W": "S", "S": "E", "E": "N"}[self.robot_direction]
        elif command == "Rechts":
            self.robot_direction = {"N": "E", "E": "S", "S": "W", "W": "N"}[self.robot_direction]

        if command in {"Vor", "Rueck"}:
            self.map_canvas.create_line(self.robot_position[0], self.robot_position[1], x, y, fill="blue", width=2)
            self.robot_position = [x, y]

root = tk.Tk()
app = RobotControlApp(root)
root.after(1000, app.connect_to_robot)
root.mainloop()
