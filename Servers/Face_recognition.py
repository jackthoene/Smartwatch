import cv2
import os
import uuid

# Load the pre-trained Haar Cascade face detector
face_cascade = cv2.CascadeClassifier(
    cv2.data.haarcascades + "haarcascade_frontalface_default.xml"
)

# Load the image
image_path = "image.jpg"
image = cv2.imread(image_path)

if image is not None:
    # Convert the image to grayscale
    gray = cv2.cvtColor(image, cv2.COLOR_BGR2GRAY)

    # Detect faces in the image
    faces = face_cascade.detectMultiScale(
        gray, scaleFactor=1.1, minNeighbors=5, minSize=(30, 30)
    )

    if len(faces) > 0:
        # If faces are detected, save the image with a unique name
        unique_filename = f"image_{uuid.uuid4().hex}.jpg"
        cv2.imwrite(unique_filename, image)
        print(f"Face detected. Image saved as {unique_filename}")
    else:
        # If no faces are detected, delete the image
        os.remove(image_path)
        print("No face detected. Image deleted.")
else:
    print("Image not found.")
