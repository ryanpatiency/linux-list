#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>

#include "../include/list.h"
#include "../private/common.h"
#include "insert-sort.c"
#include "merge-sort.c"
#include "quick-sort.c"
#include "counting-sort.c"

#define SIZE 23
long listsort_time(int *a,
                   struct listitem *item,
                   int size,
                   void (*fn)(struct list_head *))
{
    for (int i = 1; i < size; i++) {
        item[i].i = a[i];
    }
    struct timespec ts1, ts2;
    long diff;
    clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &ts1);
    fn(&item->list);
    clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &ts2);
    diff = 1000000000 * (ts2.tv_sec - ts1.tv_sec) + (ts2.tv_nsec - ts1.tv_nsec);
    return diff;
}


int main()
{
    int *a;
    struct listitem *items;

    printf("input \tmerge_sort \tcounting_sort \n");
    for (int i = 0; i < SIZE; i++) {
        int array_size = 1 << i;
        a = malloc(sizeof(int) * array_size);
        items = malloc(sizeof(struct listitem) * array_size);

        INIT_LIST_HEAD(&(items->list));
        for (int i = 1; i < array_size; i++) {
            a[i] = rand() % 366;
            list_add(&(items[i].list), &(items->list));
        }

        printf("%d\t", array_size);
        printf("%ld\t", listsort_time(a, items, array_size, merge_sort));
        printf("%ld\n", listsort_time(a, items, array_size, counting_sort));

        free(items);
        free(a);
    }
}
