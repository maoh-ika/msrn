import cv2

def convert_to_grayscale(input_image_path, output_image_path):
    # 画像をグレースケールで読み込む
    image = cv2.imread(input_image_path, cv2.IMREAD_GRAYSCALE)
    
    # 黒以外の画素を白に変換
    _, binary_image = cv2.threshold(image, 1, 255, cv2.THRESH_BINARY)
    
    # グレースケール画像を保存
    cv2.imwrite(output_image_path, binary_image)

# 入力画像のパス
input_image_path = "C:/work/retro-game-tcg/resource/stage/msrn3.jpg"
# 出力画像のパス
output_image_path = "C:/work/retro-game-tcg/resource/stage/msrn3_black.jpg"

# グレースケールに変換して保存
convert_to_grayscale(input_image_path, output_image_path)