#include <stdio.h>
#include <assert.h>
#include <stdlib.h>

#include "Data.h"


PtrAndErr_t ListCtor (List_t * lst, FILE * fp)
{
    assert(lst);

    PtrAndErr_t res = {};
    res.ptr = NULL;

    lst->root = (ListElem_t *) calloc (1, sizeof(ListElem_t));
    if (!lst->root)
    {
        res.error = MemAllocError;
        return res;
    }

    (lst->root)->data = CANARY;
    (lst->root)->next = lst->root;
    (lst->root)->prev = lst->root;

    lst->size = 0;
    lst->log = fp;

    LIST_VERIFY(lst, After, NULL, 0); // the last two argument are not used

    res.error = NoError;

    return res;
}


ListErr_t GraphDump (List_t * lst, Place_t place, const char * func_name, const char * file_name, int num_of_line, ListElem_t * addr, int value, int result)
{
    assert(lst);
    assert(func_name);
    assert(file_name);

    MakeGraphCodeFile(lst);

    char * pic_name = GetPicName();
    char * command  = GetCommand(pic_name);
    const char * place_of_call = GetPlace(place);
    FILE * log = GetLog(lst);

    system(command); // FIXME: check return value
    free(command);

    OutputTitle(log, place_of_call, func_name, file_name, num_of_line, addr, value, result);
    OutputImage(log, lst, pic_name);

    free(pic_name);
    return NoError;
}


void MakeGraphCodeFile (List_t * lst)
{
    assert(lst);

    FILE * fp = fopen("graphdump.txt", "w");
    assert(fp); //FIXME: check

    OutputNullCell         (fp, lst);
    OutputArrayCellsNodes  (fp, lst);
    OutputHeadAndTailNodes (fp);
    OutputAllEdges         (fp, lst);
    OutputArrayCellsEdges  (fp, lst);
    OutputHeadAndTailEdges (fp, lst);

    fclose(fp);
}


ListErr_t ListVerify (List_t * lst)
{
    assert(lst);

    ListErr_t error = NoError;

    if ((error = CheckNullPointers(lst)) != NoError)
        return error;
    if ((error = CheckCanaryValue(lst)) != NoError)
        return error;
    if ((error = CheckNextAndPrevConnections(lst)) != NoError)
        return error;
    if ((error = CheckIfCycles(lst)) != NoError)
        return error;

    return error;
}


void ListDtor (List_t * lst)
{
    assert(lst);

    ListElem_t * ptr = (lst->root)->next;
    ListElem_t * next = NULL;

    while (ptr != lst->root)
    {
        next = ptr->next;
        free(ptr);
        ptr = next;
    }

    free(lst->root);
}



