#!/bin/bash
RESSORCE_DIR="../src/res/"
convert -density 200 calculator.svg -resize 32x32 -transparent white "${RESSORCE_DIR}/calculator.png" 
convert -density 200 egcas.svg -resize 32x32 -transparent white "${RESSORCE_DIR}/egcas.png"
convert -density 200 insert_graphic.svg -resize 32x32 -transparent white "${RESSORCE_DIR}/insert_graphic.png"
convert -density 200 insert_text.svg -resize 32x32 -transparent white "${RESSORCE_DIR}/insert_text.png"
convert -density 200 egcas.svg -resize 2560x2560 -transparent white "${RESSORCE_DIR}/egcas_tmp.png" 
convert -density 200 italic_text.svg -resize 32x32 -transparent white "${RESSORCE_DIR}/italic_text.png"
convert -density 200 underlined_text.svg -resize 32x32 -transparent white "${RESSORCE_DIR}/underlined_text.png"
convert -density 200 bold_text.svg -resize 32x32 -transparent white "${RESSORCE_DIR}/bold_text.png"
convert -density 200 save.svg -resize 16x16 -transparent white "${RESSORCE_DIR}/save.png"
convert "${RESSORCE_DIR}/egcas_tmp.png"  -bordercolor white -border 0 \( -clone 0 -resize 16x16 \) \( -clone 0 -resize 32x32 \) \( -clone 0 -resize 48x48 \) \( -clone 0 -resize 64x64 \) \( -clone 0 -resize 128x128 \) -delete 0 -alpha off -colors 256 "${RESSORCE_DIR}/egcas.ico"
rm "${RESSORCE_DIR}/egcas_tmp.png"