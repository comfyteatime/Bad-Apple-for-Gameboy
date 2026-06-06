
echo "How many frames to convert?"

read frames

for i in $(seq 1 $frames)
do
    ./tilemap-generator-no-outbound.sh tileset.bin ./bad-apple-30fps-pgm/${i}.pgm ./bad-apple-30fps-frames/frame${i}.bin
done
