from PIL import Image, ImageOps
import struct
import argparse

def convert_image_to_raw(input_path, output_path, new_size=None, fit=False, bg_color=(0, 0, 0)):
    try:
        # 1. Open as RGBA to keep the transparency channel!
        img = Image.open(input_path).convert('RGBA')
        
        # 2. Create a solid background and paste using the alpha mask
        background = Image.new('RGBA', img.size, bg_color + (255,))
        background.paste(img, (0, 0), img)
        
        # 3. Now flatten it to solid RGB
        img = background.convert('RGB')
        
    except Exception as e:
        print(f"Error opening image: {e}")
        return

    # 4. Handle resizing based on user flags
    if new_size:
        if fit:
            print(f"Proportionally fitting image into: {new_size[0]}x{new_size[1]}")
            # This scales proportionally and pads the empty space with black
            img = ImageOps.pad(img, (new_size[0], new_size[1]), color=bg_color)
        else:
            print(f"Force stretching image to: {new_size[0]}x{new_size[1]}")
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
    parser.add_argument("input", help="Input image file")
    parser.add_argument("-o", "--output", default="image.raw", help="Output .raw file")
    
    parser.add_argument("--fullscreen", action="store_true", help="Force resize to 480x320")
    parser.add_argument("--size", type=int, nargs=2, metavar=('WIDTH', 'HEIGHT'), help="Force resize to specific width and height")
    parser.add_argument("--fit", action="store_true", help="Use with --size. Keeps aspect ratio and pads with black.")

    parser.add_argument("--bg", type=int, nargs=3, metavar=('R', 'G', 'B'), default=[0, 0, 0], help="Background RGB color (e.g., 255 255 255 for white)")
    
    args = parser.parse_args()
    
    target_size = None
    if args.fullscreen:
        target_size = (480, 320)
    elif args.size:
        target_size = (args.size[0], args.size[1])
        
    bg_tuple = tuple(args.bg)
    
    convert_image_to_raw(args.input, args.output, target_size, args.fit, bg_color=bg_tuple)
        