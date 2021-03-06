/*
 * merge sort for linux list
 * for interval = 1 to 2^k (say 2K)
 *      count = 0
 *      for 2K in N/2K
 *          count ++
 *          compare [1~K] with [K+1~2K], move them to tmp list
 *      splice tmp list back
 *
 * when to stop increase k? when N <= 2K, so count will be 1
 * when to stop loop in N/2K? when we meet the group N%2K, so q is now point at
 * origin head
 */

#include <assert.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "../include/list.h"
#include "../private/common.h"

void plist(char *name, struct list_head *testlist)
{
    static int x = 0;
    struct listitem *item, *is;
    printf("%d: the %s is :", x++, name);
    list_for_each_entry_safe (item, is, testlist, list) {
        printf("%d, ", item->i);
    }
    printf("\n");
}

void merge_sort(struct list_head *list)
{
    if (list_empty(list) || list_is_singular(list)) {
        return;
    }

    struct list_head *tmplist = malloc(sizeof(struct list_head));

    int count = 2;
    for (int k = 1; count > 1; k *= 2) {
        count = 0;
        struct list_head *p, *q = list->next;

        INIT_LIST_HEAD(tmplist);

        while (1) {
            count++;
#ifdef DEBUG
            plist("list", list);
            plist("tmplist", tmplist);
#endif
            // move p to the correct location
            p = q;
            // move q to the correct location
            for (int i = 0; i < k; i++) {
                if (q == list) {
                    break;
                }
                q = q->next;
            }
            if (q == list) {
                break;
            }
            // use qsize and psize to record remain element
            int psize = k, qsize = k;

            while (psize > 0 && qsize > 0 && q != list) {
                struct listitem *pitem = list_entry(p, struct listitem, list);
                struct listitem *qitem = list_entry(q, struct listitem, list);
                if (pitem->i <= qitem->i) {
                    p = p->next;
                    psize--;
                    list_move_tail(p->prev, tmplist);
                } else {
                    q = q->next;
                    qsize--;
                    list_move_tail(q->prev, tmplist);
                }
            }
            while (psize > 0) {
                p = p->next;
                psize--;
                list_move_tail(p->prev, tmplist);
            }
            while (qsize > 0 && q != list) {
                q = q->next;
                qsize--;
                list_move_tail(q->prev, tmplist);
            }
        }
        list_splice(tmplist, list);
    }
    free(tmplist);
}
