from PIL import Image
import struct
import argparse

def convert_image_to_raw(input_path, output_path):
    try:
        img = Image.open(input_path).convert('RGB')
    except Exception as e:
        print(f"Error opening image: {e}")
        return

    img = img.resize((480, 320), Image.Resampling.LANCZOS)
    
    with open(output_path, 'wb') as f:
        for y in range(320):
            for x in range(480):
                r, g, b = img.getpixel((x, y))
                rgb565 = ((r & 0xF8) << 8) | ((g & 0xFC) << 3) | (b >> 3)
                f.write(struct.pack('<H', rgb565))

    print(f"Success! Saved to {output_path}")

if __name__ == "__main__":
    parser = argparse.ArgumentParser(description="TFT35 V1 Image Converter")
    parser.add_argument("input", help="Input image file")
    parser.add_argument("-o", "--output", default="image.raw", help="Output .raw file")
    args = parser.parse_args()
    convert_image_to_raw(args.input, args.output)