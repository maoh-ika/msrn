
from PIL import Image, ImageOps

def add_border(image_path, border_size, output_path):
    # 画像を開く
    image = Image.open(image_path)
    
    # 画像のサイズを取得
    width, height = image.size
    
    # ボーダーサイズを加えた新しいサイズを計算
    new_width = width + 2 * border_size
    new_height = height + 2 * border_size
    
    # 新しいサイズの画像を作成し、背景を白で塗りつぶす
    new_image = Image.new("RGB", (new_width, new_height), "white")
    
    # 元の画像を新しい画像の中央に配置
    new_image.paste(image, (border_size, border_size))
    
    # 新しい画像を保存
    new_image.save(output_path)

# 画像のパス
input_image_path = 'resource/org/mashi.png'
# ボーダーサイズ（ピクセル単位）
border_size = 16
# 出力画像のパス
output_image_path = 'resource/org/mashi_border.png'

# ボーダーを追加して画像を保存
add_border(input_image_path, border_size, output_image_path)