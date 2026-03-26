#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>

#include "Data.h"

/* These functions are only called inside ListVerify so there's no assert's or other null pointers check's
inside */

ListErr_t CheckNullPointers (List_t * lst)
{
    if (lst == NULL)
        return NullListPtr;
    for (ListElem_t * ptr = lst->root; ptr != (lst->root)->prev; ptr = ptr->next)
    {
        if (ptr == NULL)
            return NullListElemPtr;
    }

    return NoError;
}


ListErr_t CheckCanaryValue (List_t * lst)
{
    if ((lst->root)->data != CANARY)
        return ChangedCanaryVal;

    return NoError;
}


ListErr_t CheckNextAndPrevConnections (List_t * lst)
{
    for (ListElem_t * ptr = lst->root; ptr != (lst->root)->prev; ptr = ptr->next)
    {
        if ((ptr->next)->prev != ptr)
            return PrevNextConnectErr;
    }

    return NoError;
}


ListErr_t CheckIfCycles (List_t * lst)
{
    int count = 0;

    for (ListElem_t * ptr = lst->root; count < lst->size && ptr != (lst->root)->prev; ptr = ptr->next)
        count++;
    if (count > lst->size)
        return SizeOverflow;

    return NoError;
}
