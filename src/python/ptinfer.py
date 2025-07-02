import torch
from torchvision import transforms
from PIL import Image

model = torch.load("/path/to/model.pt")
model.eval()

image_path = "/path/to/image.jpg"
img = Image.open(image_path)

transform = transforms.Compose(
    [
        transforms.Resize((224, 224)),
        transforms.ToTensor(),
        transforms.Normalize(mean=[0.485, 0.456, 0.406], std=[0.229, 0.224, 0.225]),
    ]
)

img_tensor = transform(img)
img_tensor = img_tensor.unsqueeze(0)
print(f"Shape: {img_tensor.size()}")

model.to("cpu")
img_tensor.to("cpu")

with torch.no_grad():
    output = model(img_tensor)

output_probs = torch.nn.functional.softmax(output, dim=1)
predicted_class = torch.argmax(output_probs, dim=1).item()
print(f"Predicted Class: {predicted_class}")
