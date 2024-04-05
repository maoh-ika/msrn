
from PIL import Image, ImageOps

def add_border(image_path, border_size, output_path):
    # 画像を開く
    image = Image.open(image_path)
    
    # 画像のサイズを取得
    width, height = image.size
    
    # ボーダーサイズを加えた新しいサイズを計算
    new_width = width + 40
    new_height = height + 40 
    
    # 新しいサイズの画像を作成し、背景を白で塗りつぶす
    new_image = Image.new("RGB", (new_width, new_height), "white")
    
    # 元の画像を新しい画像の中央に配置
    new_image.paste(image, (8, 20))
    
    # 新しい画像を保存
    new_image.save(output_path)

# 画像のパス
input_image_path = 'resource/title/8x.png'
# ボーダーサイズ（ピクセル単位）
border_size = 160
# 出力画像のパス
output_image_path = 'resource/title/msrn6_border.png'

# ボーダーを追加して画像を保存
add_border(input_image_path, border_size, output_image_path)