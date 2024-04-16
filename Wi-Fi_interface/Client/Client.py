import socket

# Set up client socket
client_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
client_socket.connect(('192.168.224.18', 3388))  # Use your PC's IP and the cho$

# Predefined message to send
message = "hello there"

# Send predefined message to PC
client_socket.send(message.encode())

# Close socket
client_socket.close()
