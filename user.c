#include <fcntl.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

#define KSORT_DEV "/dev/sort"

struct timespec start, end;
unsigned long long duration;


int main()
{
    int fd = open(KSORT_DEV, O_RDWR);
    if (fd < 0) {
        perror("Failed to open character device");
        goto error;
    }

    size_t n_elements = 1000;
    size_t size = n_elements * sizeof(int);
    int *inbuf = malloc(size);
    if (!inbuf)
        goto error;

    for (size_t i = 0; i < n_elements; i++)
        inbuf[i] = rand() % n_elements;

    clock_gettime(CLOCK_MONOTONIC, &start);
    ssize_t r_sz = read(fd, inbuf, size);
    clock_gettime(CLOCK_MONOTONIC, &end);

    if (r_sz != size) {
        perror("Failed to write character device");
        goto error;
    }

    duration = (end.tv_sec - start.tv_sec) * 1000000000LL +
               (end.tv_nsec - start.tv_nsec);
    printf("Sorting took %llu nanoseconds.\n", duration);

    bool pass = true;
    int ret = 0;
    /* Verify the result of sorting */
    for (size_t i = 1; i < n_elements; i++) {
        if (inbuf[i] < inbuf[i - 1]) {
            pass = false;
            break;
        }
    }

    printf("Sorting %s!\n", pass ? "succeeded" : "failed");

error:
    free(inbuf);
    if (fd > 0)
        close(fd);
    return ret;
}
