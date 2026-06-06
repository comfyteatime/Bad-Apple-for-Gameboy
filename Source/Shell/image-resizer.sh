
echo "How many frames to convert?"

read frames

for i in $(seq 1 $frames)
do
    convert "./bad-apple-frames/out${i}.png" -resize 40x36! ./bad-apple-gb-quality/${i}.pgm
done
