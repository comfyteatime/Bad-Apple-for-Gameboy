# Bad-Apple-for-Gameboy
Yet another Bad Apple!! port, this time for gameboy.

This project ports the popular Bad Apple!! music video to the original Gameboy.
Here is a demo of the final product running Emulicious, in glorious 40x36 resolution at 30fps:


This is achieved by parsing the video into its individual frames using ffmpeg.
We then resize all frames to fit the gameboy screen's resolution by converting them to 160x144 pgm images using ImageMagick.
Pgm images are stored as plain text, allowing us to process them without making use of external libraries.
