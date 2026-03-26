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

    if ((ListCtor(&lst, fp)).error != NoError)
    {
        printf("Ошибка выделения памяти в ListCtor\n");
        return -1;
    }

    ListElem_t * ptr = (InsertAfter(&lst, (lst.root)->next, 10)).ptr;
    ListElem_t * ptr1 = (InsertAfter(&lst, ptr, 20)).ptr;
    ListElem_t * ptr2 = (InsertAfter(&lst, ptr1, 30)).ptr;
    DeleteEl(&lst, ptr1);
    InsertBefore(&lst, ptr2, 8);
    DeleteEl(&lst, ptr2);

    GraphDump(&lst, NoArg, __FUNCTION__, __FILE__, __LINE__, NULL, 0, SUCCESS); // FIXME

    fclose(fp);
    ListDtor(&lst);

    return 0;
}
