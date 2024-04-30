#include <fcntl.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <time.h>
#include <unistd.h>

#define KSORT_DEV "/dev/sort"
#define SORT_METHOD_KSORT 0
#define SORT_METHOD_L_SORT 1

struct timespec start, end;
unsigned long long duration;

int main(int argc, char **argv)
{
    unsigned int sort_method = SORT_METHOD_KSORT;  // 預設使用 ksort
    if (argc < 3) {
        fprintf(stderr, "Usage: %s [ksort|l_sort] [n_elements]\n", argv[0]);
        return -1;
    }


    if (argc > 1) {
        if (strcmp(argv[1], "ksort") == 0) {
            sort_method = SORT_METHOD_KSORT;
        } else if (strcmp(argv[1], "l_sort") == 0) {
            sort_method = SORT_METHOD_L_SORT;
        } else {
            fprintf(stderr, "Invalid sort method. Use ./user [ksort|l_sort]\n");
            return -1;
        }
    }

    int fd = open(KSORT_DEV, O_RDWR);
    if (fd < 0) {
        perror("Failed to open character device");
        goto error;
    }

    ioctl(fd, sort_method);

    size_t n_elements = (size_t) atoi(argv[2]);
    size_t size = n_elements * sizeof(int);
    int *inbuf = malloc(size);
    if (!inbuf)
        goto error;

    for (size_t i = 0; i < n_elements; i++)
        inbuf[i] = rand() % n_elements;

    clock_gettime(CLOCK_MONOTONIC, &start);
    size_t r_sz = read(fd, inbuf, size);
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
