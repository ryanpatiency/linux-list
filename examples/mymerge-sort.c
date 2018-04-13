#include <stdio.h>
#include <assert.h>
/*
 *  merge sort, use the following
 */
#define FALSE 0
#define TRUE 1

typedef struct element element;
struct element {
    element *next, *prev;
    int i;
};
int cmp(element *a, element *b)
{
    return a->i - b->i;
}
/*
 * break the list into N/2 parts, sort these part each, and then rebreak
 * the list into N/4 part, sort these part each, in general, we  rebreak
 * the list into N/2K part , where 2K = 2^k, and each part has K element,
 * we assume 1 to K is sorted and K+1 to 2K is sorted, we just need to
 * combine it in to a bigger list.
 *
 * To sort these part each, we first handle the first 2K element
 * and move to the next 2K element, and there will be N % 2K element for
 * the last part, which we know it is time to K *= 2, every time we
 * handle, we increase handle_time, when we handle the last part, we
 * increase the handle_time also, so we can assert the the whole list is
 * sorted when handle_time == 1, which means there is only the % part, and
 * N < 2K
 *
 * To handle the N < 2K part, we divide it into two kind of problem:
 * 1. N <= K, then it is sorted, K *= 2 and break;
 * 2. N > K , then it is not sorted, we still need to combine, but have to
 * remember to break
 *
 *
 *
 * To know it is condition B, we notice that next_head will be invalid
 *
 * To do the step 3, we need to create a templist, and a tail, use
 * tail->next to record the element, and set tail to its next
 *
 * To do the step 4, we need to create a prev_pstart, to record where to
 * copy back, and we need to use the second pointer as the tail, so in case
 * A, it will be valid, in case B, it will be invalid
 *
 * To connect the list, we have to give the handle2K the 2K's head, handle2K
 * have to give us the p_nexthead, and we use it
 *
 * in the end, we return the orig_list
 */
int handle2K(element **list, int k, int is_circular, int is_double)
{
    // have a static variable to store next-head
    /*
     * To handle both the N = 2K (A) part and N < 2K (B) part together, we
     * need
     * to:
     * 1. set the pointer location to [1, K+1] (A, B2) or [1,invalid] (B1)
     * 2. set the remain of element to [K, K] (A, B2) or [N, 0] (B1)
     * 3. while there the first pointer has remaining element or the second
     * pointer has remaining element  AND is valid:
     * compare them, put them in the templist, inc info
     * 4. copy templist back to its location when we go through all the N/2K
     * part
     */
    static element *nexthead = (element *) 0xff;

    if (!nexthead || (is_circular && nexthead == *list)) {
        return is_double - is_double;
    }
    if (nexthead == (element *) 0xff) {
        nexthead = *list;
    }

    element *p, *q;
    p = q = nexthead;
    int psize = 0;
    int qsize = k;
    for (int i = 0; i < k; i++) {
        q = q->next;
        psize++;
        if (!q || q == *list) {
            qsize = 0;
            break;
        }
    }
    element *tail = NULL;
    element *e = NULL;
    while (1) {
        if (psize > 0 && qsize > 0 && q && q != *list) {
            if (q->i > p->i) {
                p = p->next;
                e = p;
                psize--;
            } else {
                e = q;
                q = q->next;
                qsize--;
            }
        } else if (psize > 0) {
            p = p->next;
            e = p;
            psize--;
        } else if (qsize > 0 && q && q != *list) {
            e = q;
            q = q->next;
            qsize--;
        } else {
            nexthead = q;
            return 1;
        }

        if (!tail) {
            tail = nexthead = e;
        } else {
            tail->next = e;
            tail = tail->next;
        }
    }
}

element *listsort(element *list, int is_circular, int is_double)
{
    if (!list)
        return NULL;
    int merge_time = 10;  // randome number > 1
    int nextheadvalid;
    for (int k = 1; merge_time != 1; k *= 2) {
        merge_time = 0;
        do {
            nextheadvalid = handle2K(&list, k, is_circular, is_double);
            merge_time++;
        } while (nextheadvalid);
    }
    return list;
}
int main(void)
{
#define n 3
    element k[n], *head, *p;
    int is_circular, is_double;

    int order[][n] = {{0, 1, 2}, {2, 1, 0}, {2, 0, 1}};

    //    int order[][n] = {
    //        {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12},
    //        {6, 2, 8, 4, 11, 1, 12, 7, 3, 9, 5, 0, 10},
    //        {12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0},
    //    };
    int i, j;

    for (j = 0; j < n; j++)
        k[j].i = j;

    listsort(NULL, 0, 0);

    for (is_circular = 0; is_circular < 1; is_circular++) {
        for (is_double = 0; is_double < 1; is_double++) {
            for (i = 0; i < (int) (sizeof(order) / sizeof(*order)); i++) {
                int *ord = order[i];
                head = &k[ord[0]];
                for (j = 0; j < n; j++) {
                    if (j == n - 1)
                        k[ord[j]].next = (is_circular ? &k[ord[0]] : NULL);
                    else
                        k[ord[j]].next = &k[ord[j + 1]];
                    if (is_double) {
                        if (j == 0)
                            k[ord[j]].prev =
                                (is_circular ? &k[ord[n - 1]] : NULL);
                        else
                            k[ord[j]].prev = &k[ord[j - 1]];
                    }
                }

                printf("before:");
                p = head;
                do {
                    printf(" %d", p->i);
                    if (is_double) {
                        if (p->next && p->next->prev != p)
                            printf(" [REVERSE LINK ERROR!]");
                    }
                    p = p->next;
                } while (is_circular ? (p != head) : (p != NULL));
                printf("\n");
                head = listsort(head, is_circular, is_double);
                printf(" after:");
                p = head;
                do {
                    printf(" %d", p->i);
                    if (is_double) {
                        if (p->next && p->next->prev != p)
                            printf(" [REVERSE LINK ERROR!]");
                    }
                    p = p->next;
                } while (is_circular ? (p != head) : (p != NULL));
                printf("\n");
            }
        }
    }
    return 0;
}
