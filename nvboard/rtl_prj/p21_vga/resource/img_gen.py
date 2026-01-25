from PIL import Image
import os

# --- 配置参数 ---
src_img_path = 'Tom.jpg'      
dst_txt_path = 'picture.hex'  

# --- 关键参数修改 ---
# 行优先模式下：
# X (h_addr) 是低位，位宽 10bit (0-1023)，所以内存每一行的跨度是 1024
# Y (v_addr) 是高位
IMAGE_W = 640         # 图片实际宽度
IMAGE_H = 480         # 图片实际高度
MEMORY_STRIDE_X = 1024 # 对应 Verilog 中 h_addr 10bit (2^10)

def generate_hex():
    if not os.path.exists(src_img_path):
        print(f"Error: {src_img_path} not found.")
        return

    img = Image.open(src_img_path)
    print(f"Resizing image to {IMAGE_W}x{IMAGE_H}...")
    img = img.resize((IMAGE_W, IMAGE_H))
    img = img.convert("RGB")

    print(f"Generating {dst_txt_path} (Row-Major)...")
    print(f"Logic: Loop Y then Loop X (Standard Raster Order).")
    print(f"Padding: Each row is padded from {IMAGE_W} to {MEMORY_STRIDE_X}.")

    with open(dst_txt_path, 'w') as f:
        # --- 关键逻辑修改：先遍历 Y (行)，再遍历 X (列) ---
        for y in range(IMAGE_H):
            # 1. 写入当前行的有效像素 (0 - 639)
            for x in range(IMAGE_W):
                r, g, b = img.getpixel((x, y))
                pixel_data = (r << 16) | (g << 8) | b
                f.write(f"{pixel_data:06x}\n")
            
            # 2. 填充当前行的剩余空间 (640 - 1023)
            # 因为 h_addr 是 10bit，必须填满 1024 才能让下一行的地址对齐
            for i in range(MEMORY_STRIDE_X - IMAGE_W):
                f.write("000000\n")

    print(f"Done! File saved to {os.path.abspath(dst_txt_path)}")
    # 总行数应该是 480 * 1024 = 491520
    print(f"Total lines: {IMAGE_H * MEMORY_STRIDE_X} (Should be 491520)")

if __name__ == '__main__':
    generate_hex()