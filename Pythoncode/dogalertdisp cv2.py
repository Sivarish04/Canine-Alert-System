import cv2
from yolov5 import YOLOv5
import socket

# Initialize the YOLOv5 model
model = YOLOv5('/home/sivarish04/E:Folder/project/Pythoncodecv/yolov5s.pt')  # Update the path if needed

# ESP32 server details
ESP32_IP = "10.42.0.27"  # Replace with your ESP32 IP address
ESP32_PORT = 80           # Port should match the port defined in ESP32 code

# Create a socket connection to the ESP32 server
client_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
client_socket.connect((ESP32_IP, ESP32_PORT))

# Open a video capture from the default camera
cap = cv2.VideoCapture("http://10.42.0.197/stream")

while True:
    ret, frame = cap.read()
    if not ret:
        break

    # Run YOLOv5 inference on the frame
    results = model.predict(frame)

    # Convert results to pandas DataFrame for easier processing
    df = results.pandas().xyxy[0]  # Get bounding boxes, labels, and confidence

    dog_detected = False  # Initialize a flag for dog detection

    # Process and visualize results
    for index, row in df.iterrows():
        label = row['name']  # Detected class label

        # Check if the detected label is 'dog'
        if label == 'dog':
            dog_detected = True

            # Get bounding box coordinates
            x1, y1, x2, y2 = int(row['xmin']), int(row['ymin']), int(row['xmax']), int(row['ymax'])
            confidence = row['confidence']  # Confidence score

            # Draw the bounding box for the dog
            cv2.rectangle(frame, (x1, y1), (x2, y2), (0, 255, 0), 2)
            # Display the label and confidence score
            cv2.putText(frame, f'{label} {confidence:.2f}', (x1, y1 - 10), cv2.FONT_HERSHEY_SIMPLEX, 0.9, (0, 255, 0), 2)

    # Send message to ESP32
    if dog_detected:
        print("Dog Alert!")
        message = "Dog Alert!\n"  # Send "Dog Alert!!!" when a dog is detected
        
    else:
        message = "\n"  # Send a blank message (newline) when no dog is detected
    
    client_socket.send(message.encode())  # Send the message to ESP32

    # Display the resulting frame
    cv2.imshow('YOLOv5 Dog Detection', frame)

    # Break the loop on 'q' key press
    if cv2.waitKey(1) & 0xFF == ord('q'):
        break

# Close resources
cap.release()
cv2.destroyAllWindows()
client_socket.close()  # Close the socket connection
