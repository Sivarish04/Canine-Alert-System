import cv2
from yolov5 import YOLOv5

# Initialize the YOLOv5 model
model = YOLOv5('/home/sivarish04/E:Folder/project/Pythoncodecv/yolov5s.pt')  # Ensure yolov5s.pt path is correct

# Open a video capture from the default camera or replace '0' with your IP camera URL
cap = cv2.VideoCapture(0)

# Loop for continuous capture and detection
while True:
    # Capture frame-by-frame
    ret, frame = cap.read()
    if not ret:
        break

    # Run inference on the frame
    results = model.predict(frame)  # Perform inference on the frame

    # Convert results to pandas DataFrame for easier processing
    df = results.pandas().xyxy[0]  # Get bounding boxes, labels, and confidence

    # Flag to track if a dog is detected
    dog_detected = False

    # Process and visualize results
    for index, row in df.iterrows():
        label = row['name']  # Detected class label

        # Only process if the detected object is a dog
        if label == 'dog':
            dog_detected = True  # Set flag if a dog is detected

            # Get bounding box coordinates
            x1, y1, x2, y2 = int(row['xmin']), int(row['ymin']), int(row['xmax']), int(row['ymax'])
            confidence = row['confidence']  # Confidence score

            # Draw bounding box around the dog
            cv2.rectangle(frame, (x1, y1), (x2, y2), (0, 255, 0), 2)
            # Display the label and confidence score
            cv2.putText(frame, f'{label} {confidence:.2f}', (x1, y1 - 10), cv2.FONT_HERSHEY_SIMPLEX, 0.9, (0, 255, 0), 2)

    # Check if a dog was detected and print an alert
    if dog_detected:
        print("Dog Alert!!!")

    # Display the resulting frame
    cv2.imshow('YOLOv5 Inference - Dog Detection', frame)

    # Break the loop on 'q' key press
    if cv2.waitKey(1) & 0xFF == ord('q'):
        break

# When everything is done, release the capture and close windows
cap.release()
cv2.destroyAllWindows()
