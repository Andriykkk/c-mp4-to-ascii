#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

int main(int argc, char *argv[])
{
    // const char *command = "gcc main.c -g -o main -Wall -Wextra -lavformat -lavcodec -lavutil -lswscale -lm; ./main";
    const char *command = "gcc main.c -g -o main -lavformat -lavformat -lavcodec -lavutil -lm -lz; ./main vid.mp4";
    int result = system(command);

    if (result == -1)
    {
        fprintf(stderr, "Error executing command: %s\n", strerror(errno));
        return 1;
    }

    return 0;
}
