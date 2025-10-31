#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>

#include "Data.h"


void FillDataWithPoison (List_t * lst, int start)
{
    assert(lst);

    int capacity = GetCapacity(lst);

    for (int count = start; count < capacity; count++) // [0] is for CANARY
    {
        (lst->data)[count] = POISON;
    }
}


void FillNextWithFreeAddresses (List_t * lst, int start)
{
    assert(lst);

    int capacity = GetCapacity(lst);

    if (start != 1)
        lst->free = start;

    for (int count = start; count < capacity - 1; count++) // [0] is for head, next for [START_SIZE - 1] is 0
    {

        (lst->next)[count] = count + 1; // address of the next free cell
    }
    (lst->next)[capacity - 1] = 0;

}


void FillPrevWithStartValue (List_t * lst, int start)
{
    assert(lst);

    int capacity = GetCapacity(lst);

    for (int count = start; count < capacity; count++) // starting from el connected with [1] el of data
    {
        (lst->prev)[count] = FREE_ADDRESS_INDICATOR;
    }
}


void ChangeNextsAndPrevsAfterDelete (List_t * lst, int addr_of_el)
{
    assert(lst);

    (lst->next)[(lst->prev)[addr_of_el]] = (lst->next)[addr_of_el]; // changing next value for the prev el
    (lst->prev)[(lst->next)[addr_of_el]] = (lst->prev)[addr_of_el]; // changing prev value for the next el

    (lst->next)[addr_of_el] = lst->free; // writing the next free address (= value of free) in
    lst->free = addr_of_el;              // changing free value (to current data pos)

    (lst->prev)[addr_of_el] = FREE_ADDRESS_INDICATOR;
}
