#include <stdio.h>

#include "Data.h"

int main (void)
{
    List_t lst = {};

    FILE * fp = fopen("log.htm", "w");
    if (fp == NULL)
    {
        printf("%s:%d: File opening error\n", __FILE__, __LINE__);
        return -1;
    }

    if (ListCtor(&lst, fp) != NoError)
    {
        printf("%s:%d: Error in ListCtor\n", __FILE__, __LINE__);
        ListDtor(&lst);
        fclose(fp);
        return -1;
    }

    InsertAfter(&lst, 0, 1);
    GraphDump(&lst, NoArg, __FUNCTION__, __FILE__, __LINE__);
    InsertAfter(&lst, 1, 2);
    GraphDump(&lst, NoArg, __FUNCTION__, __FILE__, __LINE__);
    InsertBefore(&lst, (lst.next)[0], 3);
    GraphDump(&lst, NoArg, __FUNCTION__, __FILE__, __LINE__);
    InsertBefore(&lst, (lst.next)[0], 4);
    GraphDump(&lst, NoArg, __FUNCTION__, __FILE__, __LINE__);
    InsertToHead(&lst, 5);
    GraphDump(&lst, NoArg, __FUNCTION__, __FILE__, __LINE__);
    InsertToTail(&lst, 6);
    GraphDump(&lst, NoArg, __FUNCTION__, __FILE__, __LINE__);

    fclose(fp);
    ListDtor(&lst);

    return 0;
}
