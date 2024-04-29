#ifndef L_SORT_H
#define L_SORT_H

#include <linux/types.h>
void swap_words_64(void *a, void *b, size_t n);
typedef int (*cmp_func_t)(const void *, const void *);
typedef void (*swap_func_t)(void *, void *, int);

void l_sort(void *base,
            size_t num,
            size_t size,
            cmp_func_t cmp,
            swap_func_t swap);

#endif /* L_SORT_H */
