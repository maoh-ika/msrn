import qrcode
from PIL import Image
import io

# 画像ファイルパス
image_path = "C:/work/retro-game-tcg/resource/puzzle/border.png"

# 画像をバイナリデータに変換する
with open(image_path, "rb") as img_file:
    img_data = img_file.read()

# バイナリデータをQRコードに変換する
qr = qrcode.QRCode(
    version=1,
    error_correction=qrcode.constants.ERROR_CORRECT_L,
    box_size=10,
    border=4,
)
qr.add_data(image_path)
qr.make(fit=True)

# QRコードのイメージを取得
qr_img = qr.make_image(fill_color="black", back_color="white")

# QRコードを表示または保存
output_path = "./output_qr.png"
qr_img.save(output_path)