import sys
from PIL import Image
import numpy as np

def snake_case_convert(str):
    return '_'.join(word.lower() for word in str.split(' '))

def convert_image_to_c_array(image_path, width, height, output_name, threshold=128):
    # Load and process the image
    image_smaller = Image.open(image_path)
    width_padding = width - 4
    height_padding = height - 4
    image_smaller = image_smaller.resize((width_padding, height_padding)).convert('L')  # Resize and convert to grayscale
    # Pad the image with a small margin, centered
    image = Image.new('L', (width, height), color=255)
    image.paste(image_smaller, (2, 2))

    # Add a 1px black square border around the image on all sides
    for x in range(0, width):
        image.putpixel((x, 0), 0)
        image.putpixel((x, height-1), 0)
    for y in range(0, height):
        image.putpixel((0, y), 0)
        image.putpixel((width-1, y), 0)

    # Apply threshold to convert image to monochrome
    image_array = np.array(image)
    image_monochrome = np.where(image_array > threshold, 255, 0)

    # Save the image for debugging to temp.png
    image.save('temp.png')

    # Convert the image to a numpy array and process it
    encoded_image = []

    for y in range(height):
      for x in range(0, width, 2):
          # Combine two pixels into one byte
          left_pixel = 0 if image_monochrome[y, x] == 255 else 0x0F
          right_pixel = 0 if image_monochrome[y, x+1] == 255 else 0x0F if x+1 < width else 0x00
          byte = (left_pixel << 4) | right_pixel
          encoded_image.append(byte)

    # Format the array into a C-style array
    output_name_snake = snake_case_convert(output_name)
    variable_name = f'ImageResource_{output_name_snake}_{width}x{height}'
    formatted_array = ', '.join(f'0x{byte:02X}' for byte in encoded_image)
    c_array = f'const unsigned char {variable_name}[{len(encoded_image)}] = {{\n    {formatted_array}\n}};'
    return c_array

if __name__ == "__main__":
    if len(sys.argv) != 5:
        print("Usage: python script.py path_to_image width height output_name")
        sys.exit(1)

    image_path = sys.argv[1]
    width = int(sys.argv[2])
    height = int(sys.argv[3])
    output_name = sys.argv[4]

    c_array = convert_image_to_c_array(image_path, width, height, output_name)
    print(c_array)
