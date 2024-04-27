import socket

# Set up server socket
server_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
server_socket.bind(('10.239.121.109', 8080))  # Use your PC's IP and choose a port
server_socket.listen(1)
f = open("received_data.txt", "a")


print("Waiting for connection...")

# Receive data from BeagleBone
while True:
    # Accept incoming connection
    client_socket, client_address = server_socket.accept()
    print(f"Connection from {client_address} established.")

    try:
        while True:
            # Receive data from client
            data = client_socket.recv(1024)
            if not data:
                print("Connection closed by client.")
                break
            print(f"Received data: {data.decode()}")
            f.write(data.decode())

    finally:
        # Close client socket
        client_socket.close()

# Close server socket (this will never be reached in this implementation)
server_socket.close()
