import tensorflow as tf
import numpy as np
import cv2 as cv

model_path = "/path/to/model.keras"
model = tf.keras.models.load_model(model_path)

image_path = "/path/to/image.jpeg"  # Replace with your image path
img = tf.keras.utils.load_img(image_path, target_size=(80, 80))
img_array = tf.keras.utils.img_to_array(img)
img_array = tf.expand_dims(img_array, 0)  # Batch dimension

predictions = model.predict(img_array)
print(predictions)
predicted_class_index = np.argmax(predictions)

emotions = {
    0: "Anger",
    1: "Disgust",
    2: "Fear",
    3: "Happiness",
    4: "Neutral",
    5: "Sadness",
    6: "Suprise",
}

emo = emotions.get(predicted_class_index)
print(f"Predicted class index: {emotions.get(predicted_class_index)}")
frame = cv.imread(image_path)
frame = cv.resize(frame, (640, 360))
cv.putText(
    frame,
    emo,
    (240, 240),
    cv.FONT_HERSHEY_PLAIN,
    1,
    (244, 0, 0),
)
cv.imshow("Frame", frame)
cv.waitKey(0)
