
This repository hosts code for plotting touch sensor data using BLE (Bluetooth Low Energy).

The firmware is designed to be uploaded onto an ESP32 microcontroller using the Arduino IDE, an open-source platform with syntax similar to C programming.

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


CREDITS: Detailed credits will be included shortly.
