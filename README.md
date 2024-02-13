# This repository hosts code for plotting touch sensor data using BLE (Bluetooth Low Energy)

Select a touch pin on ESP32, display the real-time touch value as a  continuous graph on OLED screen. 

At the same time send it using bluetooth to computer or phone and display the graph on PC in real-time.

# INSTRUCTIONS/INSTALLATION

- Install Arduino IDE if not present
- Have the board manager for ESP32 Installed
- Have the necessary libraries installed
- Sketch > Include Libraries > Add .zip Libraries
- Install zip file from https://github.com/elC0mpa/OLED_SSD1306_Chart/tree/master
- Compile and UPLOAD TOUCH_WITH_oled_BLE.ino
- Voila OLED works , now open linux if not already
- In Terminal add these commands to install bluepy
  ```plaintext
  sudo apt-get install python-pip libglib2.0-dev
  sudo pip install bluepy
  ```
- Now run the file by this command
  ```plaintext
  python3 app.py
  ```
  
# GENERAL

The firmware is designed to be uploaded onto an ESP32 microcontroller using the Arduino IDE, an open-source platform with syntax similar to C programming.

Install Arduino IDE if not installed using Arduino's official website, add boards manager for ESP32

To upload the firmware file onto the ESP32, no external libraries are necessary. 

When selecting the boards manager in the Arduino IDE and choosing ESP32, all relevant examples are accessible under FILE > EXAMPLES > ESP32 > BLE.

The Python script relies on two primary libraries:
- Matplotlib: for real-time plotting of sensor data with auto-scaling (Analog Reading vs. Time).
- BluePy: facilitates the connection between Python and the Bluetooth server, allowing for retrieval of touch sensor readings.

It's important to note that BluePy exclusively functions on Linux-based systems, making it incompatible with Windows machines. Similarly, alternative libraries like PyBluez also face issues, as their wheels fail to build.

For Windows users, a workaround involves running a virtual machine, such as Ubuntu on VirtualBox. Occasionally, VirtualBox may fail to detect the Bluetooth driver internally. To resolve this:

Disable Bluetooth for Windows in Device Manager.
- Restart the virtual machine.
- In the Virtual Machine, navigate to Devices > USB Device, and checkmark the Bluetooth driver.
- Refer to resources like Stack Overflow for further assistance if needed.

# DEMO BLE

![DEMO](https://github.com/mohittalwar23/BLE-Touch-OLED/blob/main/demogif.gif)

# OLED

The oled implementation is based on an opensource library https://github.com/elC0mpa/OLED_SSD1306_Chart/tree/master 

We are measuring the touch sensor value ( minimum - 0 when touched and maximum 70ish when no touch)
Similarly multiple graphs can be implemented

![DEMO2](https://github.com/mohittalwar23/BLE-Touch-OLED/blob/main/demogif2.gif)

CREDITS:  https://github.com/elC0mpa for the OLED PART


AUTHOR: https://github.com/mohittalwar23
