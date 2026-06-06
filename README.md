# Bad-Apple-for-Gameboy
Yet another Bad Apple!! port, this time for gameboy.

This project ports the popular Bad Apple!! music video to the original Gameboy. It makes use of a combination run-length encoding and delta encoding to fit the entire video into a single 1 MB Gameboy rom. This project is written entirely in C, gameboy assembly and shell scripts.
Here is a demo of the final product running Emulicious, in glorious 40x36 resolution at 30fps:

Here is the original video for reference:


Explanation:
This is achieved by parsing the video into its individual frames using ffmpeg.
We then resize all frames to fit the gameboy screen's resolution by converting them to 160x144 pgm images using ImageMagick.
Pgm images are stored as plain text, allowing us to process them directly without making use of external libraries.

The gameboy makes use of tile based graphics. This means that the 160x144 pixel area is split into a 20x18 grid of 8x8 pixel tiles.
The pixels are stored at 2bpp, so a pixel can have one of 4 shades of gray.
The gameboy only allows a tileset of 256 different tiles to be shown at one time. 
Since using a unique tileset for each frame would take up an astronomical amount of data for a device like this, we must choose a tileset to fit the full video.
We do this by splitting each tile into 4 4x4 squares, and assigning a color to each. 
The total number of possible tiles we can create this way is 4^4 = 256 tiles, which fits perfectly within the constraints.
This gives the video an effective 40x36 resolution, with 4 shades of gray.

To convert our pgm images to tilemaps that the gameboy can interpret, we divide each image into tile shaped regions. 
We then compare that area with each tile in our tileset, and select the closest fit tile to represent that part of the image.

We then have to encode the tilemaps into our gameboy rom. 
To fit this data into our constraint of 1MB, we make use of run-length encoding and delta encoding, to compress the data down.
This effectively means we only store the changes between frames, and any consecutive changes get encoded in a row of a frame get encoded in a single data packet.

Finally, to actually play the video, we write a video player in RGBDS Gameboy assembly, which decodes each frame and writes the changes to the tilemap in VRAM, once every two VBlank cycles, to achieve a smooth 30fps experience.

Proper documentation for this project may follow in the future, as well as adding music the music, and cleaning up the code, but i currently have no immediate plans to do so.
