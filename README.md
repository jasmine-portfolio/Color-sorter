# Color Sorter

The **Color Sorter** is an automatic 4-color ball sorter designed to sort up to 16 balls in just 15 seconds. This system, powered by a **Raspberry Pi**, integrates **computer vision**, **servo motors**, and **LED feedback** to sort objects based on their color. The project features a camera for real-time color detection, GPIO control for servos and LEDs, and a GUI interface for manual operation.

[Watch the demo video](https://github.com/jasmine-portfolio/Color-sorter/blob/v1.0/media/ball-sorter.mp4)

![ball-sorter](https://github.com/user-attachments/assets/dd9f564c-9769-4d5d-84b3-3a66db1ee328)

## 🚀 Features
- **Real-Time Color Detection**: Leverages **OpenCV** for efficient color detection and sorting.
- **GPIO Control**: Interfaces with Raspberry Pi’s GPIO pins to control servos and LEDs for sorting and feedback.
- **Servo Control**: Uses servo motors to move sorted objects into designated bins.
- **LED Feedback**: Provides visual indicators with LEDs for each color detected.
- **User Interface**: A simple GUI that allows for manual control and real-time monitoring of the sorting process.


## 🖱️Hardware Requirements
- **Raspberry Pi** (any model with GPIO support)
- **Camera Module** (for capturing video and detecting object colors)
- **Servo Motors** (to physically sort the objects into bins)
- **LEDs** (for color feedback and system status)
- **Optional**: Custom 3D-printed parts to enhance accuracy and improve the sorting mechanism


## 💻 Software Requirements
- **Raspberry Pi OS**: Compatible with any version of Raspberry Pi OS.
- **Libraries**:
    - [OpenCV](https://opencv.org/) (for color detection and processing)
    - pigpio (for controlling GPIO pins)
    - [cvui](https://github.com/dov/-cvui) (for GUI elements)


## 🔧 **Getting Started**

### 1. **Clone the Repository**
Follow these steps to get started with the project:
- **Step 1**: Install Git if you haven't already. You can download it from [here](https://git-scm.com/downloads).
- **Step 2**: Clone the repository by running this command in your terminal:
    
    ```
    bash    git clone https://github.com/yourusername/color-sorting-system.git  
    ```
- **Step 3**: Connect the components to your Raspberry Pi according to the **Hardware Requirements** section.
- **Step 4**: Run the program and enjoy!

### 2. **Alternative: Download as ZIP**
1. **Step 1**: Download the repository as a ZIP file.
2. **Step 2**: Extract the folder to your local machine.
3. **Step 3**: Connect the components to your Raspberry Pi as per the hardware setup.
4. **Step 4**: Load and run the code.


## 🏗️ How It Works
1. Balls are placed in a spiral tube and pass through a camera housed in a well-lit box for accurate color detection.
2. The camera captures video frames, and **OpenCV** processes the frames to detect the color based on **HSV** (Hue, Saturation, Value) values.
3. The system then activates the slider servo motor to move the ball into the appropriate bin, while LEDs light up to provide visual feedback on the sorting process.
4. The **bucket servo motor** then drops the ball into the slider, which slides the ball into the correct bin.
5. The **GUI** allows users to switch between automatic and manual modes, adjust bin settings, and monitor the system in real-time.

## 🕹️ Control Modes
The system features both **automatic** and **manual** control modes, accessible via the GUI:
- **Automatic Mode**: The system automatically detects colors and sorts balls into the corresponding bins.
- **Manual Mode**: You can manually control the sorting process through the GUI. You can:
    - Switch between **automatic** and **manual** sorting modes.
    - Adjust the **bin numbers** and their angles.
    - **Exit** the program.

## 🛠️ Troubleshooting
Here are some common issues and solutions:
- **Camera Feed Not Displaying**:
    - Navigate to `boot/config.txt` on your Raspberry Pi and uncomment the line: `hdmi_force_hotplug=1`.
    - Restart the Raspberry Pi.
    - Enable the legacy camera interface by running `sudo raspi-config` from the command line. 
    *Note*: If you get a "Permission Denied" error when editing the config file, use `sudo -s` to edit it via the command line.   
- **Servos Not Moving**:
    - Check the GPIO pin connections and ensure that the servos are properly powered.
- **Color Detection Issues**:
    - Adjust the **HSV** color thresholds in the code based on the current lighting conditions and the specific colors of the balls.


## 📄 **License**
This project is licensed under the MIT License. Feel free to fork, modify, and use it in your personal or commercial projects.
