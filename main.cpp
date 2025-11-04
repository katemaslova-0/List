#include <stdio.h>

#include "Data.h"

#define OUTPUT_ERROR_AND_FINISH {OutputErrorCode(res); ListDtor(&lst); return -1;}

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

    int res = NoError;
    if ((res = InsertAfter(&lst, 0, 10)) < 0)
        OUTPUT_ERROR_AND_FINISH;
    if ((res = InsertAfter(&lst, 1, 20)) < 0)
        OUTPUT_ERROR_AND_FINISH;
    if ((res = InsertBefore(&lst, (lst.next)[0], 30)) < 0)
        OUTPUT_ERROR_AND_FINISH;
    if ((res = InsertBefore(&lst, (lst.next)[0], 40)) < 0)
        OUTPUT_ERROR_AND_FINISH;
    if ((res = DeleteEl(&lst, (lst.prev)[0])) != NoError)
        OUTPUT_ERROR_AND_FINISH;
    (lst.next)[(lst.prev[0])] = 300;
    if ((res = InsertAfter(&lst, 0, 50)) < 0)
        OUTPUT_ERROR_AND_FINISH;

    fclose(fp);
    ListDtor(&lst);

    return 0;
}
