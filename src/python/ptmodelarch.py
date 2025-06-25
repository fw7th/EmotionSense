import torch
from torchviz import make_dot
from torchsummary import summary

model = torch.load("/home/fw7th/emotions/models/face.pt")

print(model.default_cfg)
print(model)

"""
model = model.to("cpu")  # or "cuda" if you're using GPU
summary(model, input_size=(3, 224, 224))  # change to your model's input shape


for name, module in model.named_modules():
    print(name, "->", module)
    """
