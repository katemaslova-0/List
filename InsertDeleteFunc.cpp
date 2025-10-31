#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>

#include "Data.h"


int InsertAfter (List_t * lst, int addr_of_el_before, int elem)
{
    assert(lst);

    LIST_VERIFY(lst, Before); // is assert needed?

    if (CheckIfAddressValid(lst, addr_of_el_before) != NoError)
        return InvalidElemAddr;

    if (CheckIfElemBeforeExist(lst, addr_of_el_before) != NoError)
        return ElemDoesNotExist;

    if (CallReallocIfNeed(lst) != NoError)
    {
        return MemAllocError;
    }

    int data_pos = PutElemToData(lst, elem);
    ChangeNextsAndPrevsAfterInsert(lst, data_pos, addr_of_el_before);

    LIST_VERIFY(lst, After);

    return data_pos;
}


int InsertBefore (List_t * lst, int addr_of_el_after, int elem)
{
    assert(lst);

    LIST_VERIFY(lst, Before);

    int pos = InsertAfter(lst, (lst->prev)[addr_of_el_after], elem);

    LIST_VERIFY(lst, After);

    return pos;
}


int InsertToHead (List_t * lst, int elem)
{
    assert(lst);

    LIST_VERIFY(lst, Before);

    int pos = InsertBefore(lst, (lst->next)[0], elem);

    LIST_VERIFY(lst, After);

    return pos;
}


int InsertToTail (List_t * lst, int elem)
{
    assert(lst);

    LIST_VERIFY(lst, Before);

    int pos = InsertAfter(lst, (lst->prev)[0], elem);

    LIST_VERIFY(lst, After);

    return pos;
}


ListErr_t DeleteEl (List_t * lst, int addr_of_el)
{
    assert(lst);

    LIST_VERIFY(lst, Before);

    if (CheckIfElemBeforeExist(lst, addr_of_el) != NoError)
        return ElemDoesNotExist;

    (lst->data)[addr_of_el] = POISON;
    ChangeNextsAndPrevsAfterDelete(lst, addr_of_el);

    LIST_VERIFY(lst, After);

    return NoError;
}


/*Some inside Insert/Delete functions(not for user)*/

int PutElemToData (List_t * lst, int elem)
{
    assert(lst);

    int data_pos = lst->free;
    (lst->data)[data_pos] = elem;
    lst->free = (lst->next)[lst->free];

    return data_pos;
}


ListErr_t CheckIfElemBeforeExist (List_t * lst, int address)
{
    assert(lst);

    if ((lst->prev)[address] == FREE_ADDRESS_INDICATOR)
        return ElemDoesNotExist;

    return NoError;
}


ListErr_t CheckIfAddressValid (List_t * lst, int address)
{
    assert(lst);

    if (address < 0 || address >= lst->capacity)
        return InvalidElemAddr;

    return NoError;
}


void ChangeNextsAndPrevsAfterInsert (List_t * lst, int data_pos, int addr_of_el_before)
{
    assert(lst);

    (lst->next)[data_pos] = (lst->next)[addr_of_el_before]; // setting new next's and prev's
    (lst->prev)[(lst->next)[addr_of_el_before]] = data_pos;
    (lst->next)[addr_of_el_before] = data_pos;
    (lst->prev)[data_pos] = addr_of_el_before;
}


ListErr_t CallReallocIfNeed (List_t * lst)
{
    assert(lst);

    if (lst->free == 0)
    {
        if (ListRealloc(lst) != NoError)
            return MemAllocError;
    }

    return NoError;
}
