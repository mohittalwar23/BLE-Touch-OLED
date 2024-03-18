import socket
import matplotlib.pyplot as plt
import matplotlib.animation as animation
import threading

# Define the IP address and port to listen for UDP packets
udpAddress = "192.168.218.75"  # IP address of the ESP32
udpPort = 4444  # UDP port

# Define the maximum number of data points to display on the plot
maxDataPoints = 200

# Define the maximum y-axis limit
yMax = 80

# Create empty lists to store touch values and timestamps
touchValues = []
timestamps = []

# Function to receive UDP packets and extract touch values
def receiveData():
    global touchValues, timestamps
    udpSocket = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
    udpSocket.bind((udpAddress, udpPort))
    while True:
        data, _ = udpSocket.recvfrom(1024)
        touchValue = int(data.decode())
        touchValues.append(touchValue)
        timestamps.append(len(touchValues))  # Using length of touchValues as timestamp

# Create a figure and axis for plotting
fig, ax = plt.subplots()
line, = ax.plot([], [])

# Function to update the plot with new touch values
def updatePlot(i):
    # Calculate the start index based on the number of data points
    startIdx = max(0, len(touchValues) - maxDataPoints)
    endIdx = len(touchValues)
    line.set_data(timestamps[startIdx:endIdx], touchValues[startIdx:endIdx])
    
    # Update x-axis limits for scrolling effect
    ax.set_xlim(timestamps[startIdx], timestamps[endIdx - 1])
    
    # Set y-axis limits
    ax.set_ylim(0, yMax)
    
    ax.relim()
    ax.autoscale_view()

# Create a thread for receiving UDP packets
receiveThread = threading.Thread(target=receiveData)
receiveThread.daemon = True
receiveThread.start()

# Animate the plot with received touch values
ani = animation.FuncAnimation(fig, updatePlot, frames=None, interval=10, blit=False, repeat=False)

# Show the plot
plt.show()
