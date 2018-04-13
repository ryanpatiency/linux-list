#include <assert.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../include/list.h"
#include "../private/common.h"

#define MAX 365

void counting_sort(struct list_head *list)
{
    /*
     *  count the frequency of the value (value <= MAX)
     *  use it as information to create the new list
     *  for each list, store the value in the array of 366
     *  and use it to create the new list
     */
    struct listitem *item;

    int *dayfreq = calloc(sizeof(int), MAX + 1);
    struct list_head *list2 = malloc(sizeof(struct list_head));
    INIT_LIST_HEAD(list2);
    list_splice(list, list2);
    list_for_each_entry (item, list2, list) {
        dayfreq[item->i]++;
    }
    for (int i = 0; i <= MAX; i++) {
        while (dayfreq[i] > 0) {
            item = list_first_entry(list2, struct listitem, list);
            item->i = i;
            list_move_tail(&item->list, list);
            dayfreq[i]--;
        }
    }
    free(list2);
    free(dayfreq);
}
