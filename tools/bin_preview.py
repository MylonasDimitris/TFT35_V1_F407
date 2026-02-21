import cv2
import numpy as np
import struct
import os
import argparse

def preview_lite_bin(bin_path, width=480, height=320):
    if not os.path.exists(bin_path):
        print("File not found!")
        return

    palette_bytes = 256 * 2
    pixel_bytes = width * height
    
    with open(bin_path, 'rb') as f:
        while True:
            # 1. Read Palette
            pal_raw = f.read(palette_bytes)
            if len(pal_raw) < palette_bytes: break
            palette = np.frombuffer(pal_raw, dtype=np.uint16)
            
            # 2. Read Indices
            indices_raw = f.read(pixel_bytes)
            if len(indices_raw) < pixel_bytes: break
            indices = np.frombuffer(indices_raw, dtype=np.uint8)
            
            # 3. Read and verify Sync
            sync_data = f.read(2)
            if len(sync_data) < 2 or struct.unpack('<H', sync_data)[0] != 0xAA55:
                print("Sync marker lost! File might be corrupted or end reached.")
                break

            # 4. Map indices to palette colors (RGB565)
            data_565 = palette[indices]

            # 5. Convert RGB565 back to RGB888 for PC display
            r = ((data_565 >> 11) & 0x1F) << 3
            g = ((data_565 >> 5) & 0x3F) << 2
            b = (data_565 & 0x1F) << 3
            
            img = np.stack((r, g, b), axis=-1).astype(np.uint8)
            img = img.reshape((height, width, 3))

            cv2.imshow("8-bit Lite Preview (Press 'q' to quit)", cv2.cvtColor(img, cv2.COLOR_RGB2BGR))
            
            # ~15fps playback speed (66ms)
            if cv2.waitKey(66) & 0xFF == ord('q'): 
                break

    cv2.destroyAllWindows()

if __name__ == "__main__":
    parser = argparse.ArgumentParser(description="TFT35 V1 Bin File Previewer")
    parser.add_argument("input", help="Input .bin file to preview")
    args = parser.parse_args()
    preview_lite_bin(args.input)