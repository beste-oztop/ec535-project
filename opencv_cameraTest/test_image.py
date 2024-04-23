import cv2
import time

# Initialize the camera
cap = cv2.VideoCapture(0)  # 0 corresponds to the default camera

# Check if the camera is opened successfully
if not cap.isOpened():
    print("Error: Failed to open camera")
    exit()

            # Capture a frame from the camera
ret, frame = cap.read()
time.sleep(1)
ret, frame = cap.read()


            # Check if the frame is captured successfully
if not ret:
    print("Error: Failed to capture frame")
    cap.release()  # Release the camera
    exit()

                            # Define the filename for the JPEG file
filename = "captured_image.jpg"

                            # Save the frame as a JPEG file
cv2.imwrite(filename, frame, [int(cv2.IMWRITE_JPEG_QUALITY), 90])

                            # Release the camera
cap.release()

print(f"Image saved as {filename}")
