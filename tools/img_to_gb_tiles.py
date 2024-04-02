import subprocess
import os
import cv2

org_img = 'resource/stage/piece.png'
out_tileset = 'puzzle_piece_tileset'
out_tilemap = 'puzzle_piece_tilemap'
tileset_name = 'PUZZLE_PIT_TILESET'
tilemap_name = 'PUZZLE_PIT_TILEMAP'
out_tiles_x = 2
out_tiles_y = 2

img = cv2.imread(org_img, cv2.IMREAD_GRAYSCALE)
cv2.imwrite('g.png', img)

h, w = img.shape[0], img.shape[1]
w8 = out_tiles_x * 8
h8 = out_tiles_y * 8

img8 = cv2.resize(img, (w8, h8))
scale_img = './8x.png'
cv2.imwrite(scale_img, img8)

subprocess.run(['img2gb',
    'tileset',
    f'--output-c-file={out_tileset}.c',
    f'--output-header-file={out_tileset}.h',
    f'--output-image={out_tileset}.png',
    '--deduplicate',
    f'--name={tileset_name}',
    scale_img
])

subprocess.run(['img2gb',
    'tilemap',
    f'--output-c-file={out_tilemap}.c',
    f'--output-header-file={out_tilemap}.h',
    f'{out_tileset}.png',
    f'--name={tilemap_name}',
    scale_img
])
