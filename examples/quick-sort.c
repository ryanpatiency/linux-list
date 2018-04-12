#include <assert.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "../include/list.h"
#include "../private/common.h"

static uint16_t values[256];

/*
 *  my qsort,
 *  use the first element as pivot, compare with all the other, and combine
 *
 *  so if there is no first element, quit
 */
void myqsort(struct list_head *p_head)
{
    if (list_empty(p_head) || list_is_singular(p_head)) {
        return;
    }
    struct listitem *p_item = list_first_entry(p_head, struct listitem, list);
    uint16_t pivot = p_item->i;

    struct list_head less_equal_head;
    struct list_head larger_head;
    struct listitem *p_safe;
    INIT_LIST_HEAD(&less_equal_head);
    INIT_LIST_HEAD(&larger_head);
    list_for_each_entry_safe (p_item, p_safe, p_head, list) {
        if (p_item->i > pivot) {
            list_move(&p_item->list, &larger_head);
        } else {
            list_move(&p_item->list, &less_equal_head);
        }
    }
    myqsort(&less_equal_head);
    myqsort(&larger_head);
    list_splice(&larger_head, p_head);
    list_splice(&less_equal_head, p_head);
}
int main(void)
{
    struct list_head testlist;
    struct listitem *item, *is = NULL;
    size_t i;

    random_shuffle_array(values, (uint16_t) ARRAY_SIZE(values));

    INIT_LIST_HEAD(&testlist);

    assert(list_empty(&testlist));

    for (i = 0; i < ARRAY_SIZE(values); i++) {
        item = (struct listitem *) malloc(sizeof(*item));
        assert(item);
        item->i = values[i];
        list_add_tail(&item->list, &testlist);
    }

    assert(!list_empty(&testlist));

    qsort(values, ARRAY_SIZE(values), sizeof(values[0]), cmpint);
    myqsort(&testlist);

    i = 0;
    list_for_each_entry_safe (item, is, &testlist, list) {
        assert(item->i == values[i]);
        list_del(&item->list);
        free(item);
        i++;
    }

    assert(i == ARRAY_SIZE(values));
    assert(list_empty(&testlist));
    printf("Verified\n");
    return 0;
}
