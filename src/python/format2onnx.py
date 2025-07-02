import os
import tensorflow as tf
import tf2onnx
import onnx
import torch

"""
    .keras -> onnx
"""
os.environ["TF_CPP_MIN_LOG_LEVEL"] = "3"
base_path = os.path.dirname(os.path.dirname(os.path.dirname(__file__)))

chosen = os.path.join(base_path, "models", "chosen.keras")
onnx_h5 = os.path.join(base_path, "models", "chosen.onnx")


model = tf.keras.models.load_model(chosen)

spec = tuple(tf.TensorSpec((None, *inp.shape[1:]), inp.dtype) for inp in model.inputs)
mobile_onnx, _ = tf2onnx.convert.from_keras(model, input_signature=spec, opset=13)

with open(onnx_h5, "wb") as f:
    f.write(mobile_onnx.SerializeToString())


"""
    .pt -> onnx
"""
face_pt = os.path.join(base_path, "models", "face.pt")
face_onnx = os.path.join(base_path, "models", "face.onnx")

model2 = torch.load(face_pt, map_location="cpu")
model2.eval()


dummy_input = torch.randn(1, 3, 224, 224)  # Match your model's input shape

torch.onnx.export(
    model2,
    dummy_input,
    face_onnx,
    export_params=True,
    opset_version=13,
    do_constant_folding=True,
    input_names=["input"],
    output_names=["output"],
    dynamic_axes={"input": {0: "batch_size"}, "output": {0: "batch_size"}},
)
