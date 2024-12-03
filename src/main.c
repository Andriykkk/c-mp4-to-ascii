#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <inttypes.h>
#include <math.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libavutil/avutil.h>
#include <libavutil/imgutils.h>
#include <libswscale/swscale.h>

char *change_extension(char *filename, char *extension)
{
	char *dot = strrchr(filename, '.');
	char *asci_filename = malloc(strlen(filename) + strlen(extension) + 1);
	char length;

	if (dot == NULL)
	{
		length = strlen(filename);
	}
	else
	{
		length = dot - filename;
	}

	strncpy(asci_filename, filename, length);

	strcat(asci_filename, extension);

	return asci_filename;
}

const char *ascii_chars = "@B#8&WM0Q%O0QLCJUYXzcvuxnqfjoai!_+-.   ";

void convert_frame_to_ascii(AVFrame *frame, int width, int height)
{
	uint8_t *y_channel = frame->data[0];
	int pixel_size = frame->linesize[0];
	int frame_width = frame->width;
	int frame_height = frame->height;
	int ascii_length = strlen(ascii_chars);

	int y_divider = 4;
	int x_divider = 2;

	printf("\033[H\033[J");

	for (int y = 0; y < frame_height; y += y_divider)
	{
		for (int x = 0; x < frame_width; x += x_divider)
		{
			uint8_t gray = y_channel[y * pixel_size + x];

			int ascii_index = (gray * (ascii_length - 1)) / 255;
			printf("%c", ascii_chars[ascii_index]);
		}
		printf("\n");
	}

	fflush(stdout);
}

void create_ascii_file(char *filename)
{
	AVFormatContext *fmt_ctx = NULL;
	AVCodecContext *codec_ctx = NULL;
	const AVCodec *codec = NULL;
	AVStream *video_stream = NULL;
	AVFrame *frame = NULL;
	AVPacket packet;
	int video_stream_index = -1;
	int ret;

	if (avformat_open_input(&fmt_ctx, filename, NULL, NULL) < 0)
	{
		fprintf(stderr, "Could not open input file '%s'\n", filename);
		return;
	}

	if (avformat_find_stream_info(fmt_ctx, NULL) < 0)
	{
		fprintf(stderr, "Could not find stream information\n");
		return;
	}

	for (int i = 0; i < fmt_ctx->nb_streams; i++)
	{
		if (fmt_ctx->streams[i]->codecpar->codec_type == AVMEDIA_TYPE_VIDEO)
		{
			video_stream_index = i;
			video_stream = fmt_ctx->streams[i];
			break;
		}
	}

	if (video_stream_index == -1)
	{
		fprintf(stderr, "No video stream found\n");
		return;
	}

	codec = avcodec_find_decoder(video_stream->codecpar->codec_id);
	if (!codec)
	{
		fprintf(stderr, "Codec not found\n");
		return;
	}

	codec_ctx = avcodec_alloc_context3(codec);
	if (!codec_ctx)
	{
		fprintf(stderr, "Could not allocate codec context\n");
		return;
	}

	if (avcodec_parameters_to_context(codec_ctx, video_stream->codecpar) < 0)
	{
		fprintf(stderr, "Could not copy codec parameters to codec context\n");
		return;
	}

	if (avcodec_open2(codec_ctx, codec, NULL) < 0)
	{
		fprintf(stderr, "Could not open codec\n");
		return;
	}

	frame = av_frame_alloc();
	if (!frame)
	{
		fprintf(stderr, "Could not allocate frame\n");
		return;
	}

	int x = 0;

	while (av_read_frame(fmt_ctx, &packet) >= 0)
	{
		if (packet.stream_index == video_stream_index)
		{
			ret = avcodec_send_packet(codec_ctx, &packet);
			if (ret < 0)
			{
				printf("Error sending packet to decoder (stream index: %d, codec id: %d)\n", packet.stream_index, codec_ctx->codec_id);
				continue;
			}

			while (avcodec_receive_frame(codec_ctx, frame) == 0)
			{
				convert_frame_to_ascii(frame, codec_ctx->width, codec_ctx->height);
				usleep(41700);
			}
		}
	}
}

void render_ascii(char *filename)
{
	printf("Hello, World!\n");
}

int main(int argc, char *argv[])
{
	if (argc < 2)
	{
		fprintf(stderr, "Usage: %s <filename>\n", argv[0]);
		return 1;
	}

	char *filename_with_extension = change_extension(argv[1], "asci");

	if (access(filename_with_extension, F_OK) != 0)
	{
		create_ascii_file(argv[1]);
	}

	render_ascii(argv[1]);
}