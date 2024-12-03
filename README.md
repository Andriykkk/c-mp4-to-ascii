# ASCII Video Converter

This program converts a video file into an ASCII representation of each frame. It processes the video frame by frame and outputs the ASCII art in the terminal.

## Requirements
This program uses **FFmpeg** libraries to decode video files, so you must have FFmpeg installed on your system. It requires the following libraries:
- `libavcodec`
- `libavformat`
- `libavutil`
- `libswscale`

### Dependencies
To install FFmpeg and the necessary libraries, you can use the following commands on a Debian-based system:

```bash
sudo apt-get update
sudo apt-get install libavcodec-dev libavformat-dev libavutil-dev libswscale-dev
sudo apt-get install ffmpeg
```

## Requirements
To compile the program, use the following gcc command:

```bash
gcc -o main main.c -lavcodec -lavformat -lavutil -lswscale
```
## Usage
To run the program, simply provide the path to a video file (e.g., .mp4) as a command-line argument. The program will process the video and display ASCII art in the terminal.

```bash
./main video.mp4
```

## Example
![Ascii image](./image.png)