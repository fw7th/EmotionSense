import os
import tensorflow as tf
from tensorflow.keras import Model
from tensorflow.keras.utils import plot_model
from tensorflow.keras.applications import ResNet50
from tensorflow.keras.optimizers import SGD
from tensorflow.keras.layers import Flatten, Dense

os.environ["CUDA_VISIBLE_DEVICES"] = "3"


def create_model(architecture, parameters):
    model = architecture(
        input_shape=parameters["shape"] + [3],
        weights="imagenet",
        include_top=False,
        classes=parameters["nbr_classes"],
    )

    # Freeze existing VGG already trained weights
    for layer in model.layers[: parameters["number_of_last_layers_trainable"]]:
        layer.trainable = False

    # get the VGG output
    out = model.output

    # Add new dense layer at the end
    x = Flatten()(out)
    x = Dense(parameters["nbr_classes"], activation="softmax")(x)

    model = Model(inputs=model.input, outputs=x)

    opti = SGD(
        learning_rate=parameters["learning_rate"],
        momentum=parameters["momentum"],
        nesterov=parameters["nesterov"],
    )

    model.compile(loss="categorical_crossentropy", optimizer=opti, metrics=["accuracy"])

    return model


parameters = {
    "shape": [80, 80],
    "nbr_classes": 7,
    "batch_size": 8,
    "epochs": 50,
    "number_of_last_layers_trainable": 10,
    "learning_rate": 0.001,
    "nesterov": True,
    "momentum": 0.9,
}
model = create_model(architecture=ResNet50, parameters=parameters)
model.load_weights("/path/to/model.h5")
model.summary()

# model.save("/path/to/model/save.keras")

print("Inputs:")
for i in model.inputs:
    print(f"  Shape: {i.shape}, Dtype: {i.dtype}")

print("Outputs:")
for o in model.outputs:
    print(f"  Shape: {o.shape}, Dtype: {o.dtype}")

plot_model(model, to_file="model.png", show_shapes=True, show_layer_names=True)
