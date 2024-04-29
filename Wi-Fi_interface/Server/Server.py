import socket
import time
from pynput.keyboard import Controller # Pressing the key taken from the client side


# Set up server socket
server_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
server_socket.bind(('10.239.204.45', 8080))  # Use your PC's IP and choose a port
server_socket.listen(1)

keyboard = Controller()  # Create the controller


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
            print(f"{data.decode()}")
            keyboard.type(f"{data.decode()}")  # Type the character

    finally:
        # Close client socket
        client_socket.close()

# Close server socket (this will never be reached in this implementation)
server_socket.close()








