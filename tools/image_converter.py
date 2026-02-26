from PIL import Image
import struct
import argparse

def convert_image_to_raw(input_path, output_path, new_size=None):
    try:
        img = Image.open(input_path).convert('RGB')
    except Exception as e:
        print(f"Error opening image: {e}")
        return

    # Resize the image if the user requested it!
    if new_size:
        print(f"Resizing image to: {new_size[0]}x{new_size[1]}")
        img = img.resize((new_size[0], new_size[1]), Image.Resampling.LANCZOS)
    else:
        print(f"Keeping original image size: {img.width}x{img.height}")

    width, height = img.size
    
    with open(output_path, 'wb') as f:
        for y in range(height):
            for x in range(width):
                r, g, b = img.getpixel((x, y))
                # Compress 24-bit RGB into 16-bit RGB565 format
                rgb565 = ((r & 0xF8) << 8) | ((g & 0xFC) << 3) | (b >> 3)
                f.write(struct.pack('<H', rgb565))

    print(f"Success! Saved to {output_path}")

if __name__ == "__main__":
    parser = argparse.ArgumentParser(description="TFT35 V1 Custom Image Converter")
    parser.add_argument("input", help="Input image file (e.g., button.png)")
    parser.add_argument("-o", "--output", default="image.raw", help="Output .raw file")
    
    # Custom resizing arguments
    parser.add_argument("--fullscreen", action="store_true", help="Force resize to 480x320")
    parser.add_argument("--size", type=int, nargs=2, metavar=('WIDTH', 'HEIGHT'), help="Force resize to specific width and height")
    
    args = parser.parse_args()
    
    # Determine the target size based on the user's commands
    target_size = None
    if args.fullscreen:
        target_size = (480, 320)
    elif args.size:
        target_size = (args.size[0], args.size[1])
        
    convert_image_to_raw(args.input, args.output, target_size)