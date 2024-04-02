from PIL import Image

def run_length_encode(data):
    encoded_data = []
    count = 1
    for i in range(1, len(data)):
        if data[i] != data[i - 1] or i == len(data) - 1:
            encoded_data.append(count)
            encoded_data.append(data[i - 1])
            count = 1
        else:
            count += 1
    return encoded_data

def run_length_decode(encoded_data):
    decoded_data = []
    for i in range(0, len(encoded_data), 2):
        count = encoded_data[i]
        value = encoded_data[i + 1]
        decoded_data.extend([value] * count)
    return decoded_data

def create_binary_image(binary_file_path, width, height, output_image_path):
    # バイナリファイルをバイトデータとして読み込む
    with open(binary_file_path, "rb") as file:
        binary_data = file.read()

    compressed_data = run_length_encode(binary_data)

    # 2値画像を生成するためのデータ
    image_data = []
   # バイトデータを解釈して2値画像データを作成
    for byte in compressed_data:
        # 各バイトをビット単位で処理して、0または255のピクセル値に変換
        for i in range(8):
            bit = (byte >> i) & 1
            pixel_value = 0 if bit == 0 else 255
            image_data.append(pixel_value)

    # 2値画像をPillow Imageオブジェクトとして生成
    image = Image.new("L", (width, height))
    image.putdata(image_data)

    # 画像を保存
    image.save(output_image_path)

# バイナリファイルのパス
binary_file_path = "C:/work/retro-game-tcg/rom/SAGA2.sav"
# 画像の幅と高さ
width = 100
height = 200
# 出力画像のパス
output_image_path = "binary_image.png"

# 2値画像を生成して保存
create_binary_image(binary_file_path, width, height, output_image_path)