import cv2
index = 0
arr = []
while index < 10:
    cap = cv2.VideoCapture(index)
    if cap.read()[0]:
        arr.append(index)
        cap.release()
    index += 1
print(arr)
