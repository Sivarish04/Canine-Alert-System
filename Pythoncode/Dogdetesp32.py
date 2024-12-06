import cv2
from yolov5 import YOLOv5

# Initialize the YOLOv5 model
model = YOLOv5('/home/sivarish04/E:Folder/project/Pythoncodecv/yolov5s.pt')  # Update the path if needed

# URL of the ESP32-CAM MJPEG stream
esp32cam_url = 'http://10.42.0.197/stream'  # Replace with your ESP32-CAM URL

# Open a video capture from the ESP32-CAM stream
cap = cv2.VideoCapture(esp32cam_url)

# Create a resizable window
cv2.namedWindow('YOLOv5 Inference', cv2.WINDOW_NORMAL)

# Set the initial size of the window (width, height)
cv2.resizeWindow('YOLOv5 Inference', 1280, 720)  # Adjust the size as needed

while True:
    # Capture frame-by-frame
    ret, frame = cap.read()
    if not ret:
        print("Failed to capture image from ESP32-CAM")
        break

    # Run inference on the frame
    results = model.predict(frame)  # Use the predict method for inference

    # Convert results to pandas DataFrame for easier processing
    df = results.pandas().xyxy[0]  # Get bounding boxes, labels, and confidence

    dog_detected = False  # Initialize a flag for dog detection

    # Process and visualize results
    for index, row in df.iterrows():
        # Example: Draw bounding boxes and labels
        x1, y1, x2, y2 = int(row['xmin']), int(row['ymin']), int(row['xmax']), int(row['ymax'])  # Bounding box coordinates
        label = row['name']  # Detected class label
        confidence = row['confidence']  # Confidence score
        
        # Draw the bounding box
        cv2.rectangle(frame, (x1, y1), (x2, y2), (0, 255, 0), 2)
        # Display the label and confidence score
        cv2.putText(frame, f'{label} {confidence:.2f}', (x1, y1 - 10), cv2.FONT_HERSHEY_SIMPLEX, 0.9, (0, 255, 0), 2)

        # Check if the detected label is 'dog'
        if label == 'dog':
            dog_detected = True

    # Print a message if a dog is detected
    if dog_detected:
        print("Dog detected")

    # Display the resulting frame in the resized window
    cv2.imshow('YOLOv5 Inference', frame)

    # Break the loop on 'q' key press
    if cv2.waitKey(1) & 0xFF == ord('q'):
        break

# When everything is done, release the capture and close windows
cap.release()
cv2.destroyAllWindows()
