from PIL import Image

# Step 1: Open the image
img = Image.open("C.png")  # Replace "C.png" with the actual path
if img.mode != "RGB":
    img = img.convert("RGB")  # Convert to RGB mode if not already

pixels = list(img.getdata())  # Get pixel values as a list of (r, g, b) tuples

# Scale RGB values to the range 0-31
scaled_pixels = [(int(r * 31 / 255), int(g * 31 / 255), int(b * 31 / 255)) for r, g, b in pixels]

# Step 2: Generate C header file
with open("Colour.h", "w") as header_file:
    header_file.write("#pragma once\n")
    header_file.write("#include <nds.h>\n\n")

    header_file.write("const unsigned short imageData[] = {\n")
    for pixel_value in scaled_pixels:
        header_file.write(f"    RGB15({pixel_value[0]}, {pixel_value[1]}, {pixel_value[2]}),\n")
    header_file.write("};\n\n")
