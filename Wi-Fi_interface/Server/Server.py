import socket

# Set up server socket
server_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
server_socket.bind(('0.0.0.0', 3388))  # Use your PC's IP and choose a port
server_socket.listen(1)

print("Waiting for connection...")

# Accept incoming connection
client_socket, client_address = server_socket.accept()
print(f"Connection from {client_address} established.")

while True:
    # Receive data from BeagleBone
    data = client_socket.recv(1024)
    if not data:
        break
    print(f"Received data: {data.decode()}")

# Close sockets
client_socket.close()
server_socket.close()
