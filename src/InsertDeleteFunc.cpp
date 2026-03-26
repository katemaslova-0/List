#include <stdio.h>
#include <assert.h>
#include <stdlib.h>

#include "Data.h"


PtrAndErr_t InsertAfter (List_t * lst, ListElem_t * lst_elem_before, int value)
{
    assert(lst);
    assert(lst_elem_before);
    assert(lst_elem_before->next); // FIXME: are asserts needed?

    PtrAndErr_t res = {};
    res.ptr = NULL;
    res.error = NoError;

    LIST_VERIFY(lst, Before, lst_elem_before, value);

    ListElem_t * lst_elem = (ListElem_t *) calloc (1, sizeof(ListElem_t));
    if (!lst_elem)
    {
        res.error = MemAllocError;
        return res;
    }

    lst_elem->data = value;
    lst_elem->prev = lst_elem_before;
    lst_elem->next = lst_elem_before->next;

    (lst_elem_before->next)->prev = lst_elem;
    lst_elem_before->next = lst_elem;

    lst->size++;

    LIST_VERIFY(lst, After, lst_elem_before, value);

    res.ptr = lst_elem;

    return res;
}


PtrAndErr_t InsertBefore (List_t * lst, ListElem_t * lst_elem_after, int value)
{
    assert(lst);
    assert(lst_elem_after);

    LIST_VERIFY(lst, Before, lst_elem_after, value);

    PtrAndErr_t res = InsertAfter(lst, (lst_elem_after)->prev, value);

    LIST_VERIFY(lst, After, lst_elem_after, value);

    return res;
}


PtrAndErr_t DeleteEl (List_t * lst, ListElem_t * lst_elem)
{
    assert(lst);
    assert(lst_elem);
    assert(lst_elem->next);
    assert(lst_elem->prev); // FIXME: are asserts needed?

    LIST_VERIFY(lst, Before, lst_elem, lst_elem->data);

    (lst_elem->prev)->next = lst_elem->next;
    (lst_elem->next)->prev = lst_elem->prev;

    int data = lst_elem->data;

    free(lst_elem);

    lst->size--;

    LIST_VERIFY(lst, After, NULL, data);

    PtrAndErr_t res = {};
    res.ptr = NULL;
    res.error = NoError;

    return res;
}
