import time
from bluepy.btle import Peripheral, UUID
import matplotlib.pyplot as plt

# MAC address of the ESP32 BLE server
MAC_ADDRESS = "A0:B7:65:DC:7C:A2"

# UUIDs of the service and characteristic
SERVICE_UUID = UUID("4fafc201-1fb5-459e-8fcc-c5c9c331914b")
CHARACTERISTIC_UUID = UUID("beb5483e-36e1-4688-b7f5-ea07361b26a8")

try:
    # Connect to the ESP32 BLE server
    peripheral = Peripheral(MAC_ADDRESS)

    # Get the service and characteristic
    service = peripheral.getServiceByUUID(SERVICE_UUID)
    characteristic = service.getCharacteristics(CHARACTERISTIC_UUID)[0]

    # Create a figure and axis for plotting
    fig, ax = plt.subplots()
    timestamps = []
    touch_values = []
    start_time = time.time()  # Get the start time

    while True:
        # Read the value of the characteristic
        value = characteristic.read()

        # Convert the value to integer (little-endian)
        touch_value = int.from_bytes(value, byteorder='little', signed=False)

        # Get current time and normalize it to start from zero
        timestamp = time.time() - start_time

        # Append the current timestamp and touch value to the lists
        timestamps.append(timestamp)
        touch_values.append(touch_value)

        # Plot the data as a continuous line
        ax.clear()
        ax.plot(timestamps, touch_values, 'b-')

        # Set labels and title
        ax.set_xlabel('Time (seconds)')
        ax.set_ylabel('Touch Value')
        ax.set_title('Touch Sensor Readings')

        # Draw the plot
        plt.draw()
        plt.pause(0.05)

except Exception as e:
    print("Error:", e)

finally:
    # Disconnect from the BLE server
    if 'peripheral' in locals():
        peripheral.disconnect()
