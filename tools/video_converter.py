import cv2
import numpy as np
import struct
import os
import argparse
from tqdm import tqdm

def convert_to_lite_bin(video_path, output_bin, width=480, height=320, output_fps=10):
    if not os.path.exists(video_path):
        print(f"Error: {video_path} not found.")
        return

    cap = cv2.VideoCapture(video_path)
    input_fps = cap.get(cv2.CAP_PROP_FPS)
    total_frames = int(cap.get(cv2.CAP_PROP_FRAME_COUNT))
    skip_interval = max(1, round(input_fps / output_fps))
    expected_output = total_frames // skip_interval

    print(f"Processing: {video_path} -> {output_bin}")
    
    with open(output_bin, 'wb') as out:
        input_count = 0
        output_count = 0
        
        pbar = tqdm(total=expected_output, desc="Converting Video")
        while True:
            ret, frame = cap.read()
            if not ret: break

            if input_count % skip_interval == 0:
                img = cv2.resize(frame, (width, height), interpolation=cv2.INTER_AREA)
                img_rgb = cv2.cvtColor(img, cv2.COLOR_BGR2RGB)
                
                z = img_rgb.reshape((-1, 3)).astype(np.float32)
                criteria = (cv2.TERM_CRITERIA_EPS + cv2.TERM_CRITERIA_MAX_ITER, 10, 1.0)
                _, labels, centers = cv2.kmeans(z, 256, None, criteria, 10, cv2.KMEANS_RANDOM_CENTERS)
                
                centers = centers.astype(np.uint16)
                r = (centers[:, 0] >> 3) << 11
                g = (centers[:, 1] >> 2) << 5
                b = (centers[:, 2] >> 3)
                palette_565 = (r | g | b).astype(np.uint16)

                out.write(palette_565.tobytes()) 
                out.write(labels.astype(np.uint8).tobytes())
                out.write(struct.pack('<H', 0xAA55)) 
                
                output_count += 1
                pbar.update(1)
            
            input_count += 1
        pbar.close()

    cap.release()
    print(f"Success! Final Size: {os.path.getsize(output_bin)/1024/1024:.2f} MB")

if __name__ == "__main__":
    parser = argparse.ArgumentParser(description="TFT35 V1 Video Converter")
    parser.add_argument("input", help="Input video file")
    parser.add_argument("-o", "--output", default="anim_lite.bin", help="Output .bin file")
    parser.add_argument("--fps", type=int, default=10, help="Target FPS (default: 10)")
    args = parser.parse_args()
    convert_to_lite_bin(args.input, args.output, output_fps=args.fps)