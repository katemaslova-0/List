#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>

#include "Data.h"

const ListEl CANARY = 5498; // better value?
const ListEl POISON = 333;  // better value?
const int SIZE_OF_PIC_NAME = 80;
const int SIZE_OF_COMMAND = 80;
const ListEl FREE_ADDRESS_INDICATOR = -1;

#define LIST_VERIFY(lst, place) if (ListVerify(lst) != NoError) \
                                {printf("ListDump at %s:%d\n", __FILE__, __LINE__); \
                                GraphDump(lst, place, __FUNCTION__, __FILE__, __LINE__); \
                                return ListError;}

ListErr_t ListCtor (List_t * lst, FILE * fp)
{
    assert(lst);
    assert(fp);

    lst->data = (ListEl *) calloc (START_SIZE, sizeof(ListEl)); //func
    lst->next = (int *)    calloc (START_SIZE, sizeof(int));
    lst->prev = (int *)    calloc (START_SIZE, sizeof(int));

    lst->log = fp;                  // logfile

    (lst->data)[0] = CANARY;
    (lst->next)[0] = 0;             // head
    (lst->prev)[0] = 0;             // tail

    lst->free = 1;                  // the first free element of array
    lst->capacity = START_SIZE;

    FillDataWithPoison(lst);
    FillNextWithFreeAddresses(lst);
    FillPrevWithStartValue(lst);

    LIST_VERIFY(lst, After);

    return NoError;
}


void FillDataWithPoison (List_t * lst)
{
    assert(lst);

    for (int count = 1; count < START_SIZE; count++) // [0] is for CANARY
    {
        (lst->data)[count] = POISON;
    }
}


void FillNextWithFreeAddresses (List_t * lst)
{
    assert(lst);

    for (int count = 1; count < START_SIZE - 1; count++) // [0] is for head, next for [START_SIZE - 1] is 0
    {
        (lst->next)[count] = count + 1; // address of the next free cell
    }

    (lst->next)[START_SIZE - 1] = 0;
}


void FillPrevWithStartValue (List_t * lst)
{
    assert(lst);

    for (int count = 1; count < START_SIZE; count++) // starting from el connected with [1] el of data
    {
        (lst->prev)[count] = FREE_ADDRESS_INDICATOR;
    }
}


ListErr_t InsertAfter (List_t * lst, int addr_of_el_before, int elem)
{
    assert(lst);

    LIST_VERIFY(lst, Before); // is assert needed?

    if (addr_of_el_before < 0 || addr_of_el_before >= lst->capacity)
    {
        printf("Error at %s: Invalid element address\n", __FUNCTION__);
        return ListError;
    }

    if ((lst->prev)[addr_of_el_before] == FREE_ADDRESS_INDICATOR)
    {
        printf("Error at %s: Element before does not exist\n", __FUNCTION__);
        return ListError;
    }

    if (lst->free == 0)
    {
        if (ListRealloc(lst) != NoError) // message about where and what went wrong
            return MemAllocError;
    }

    int data_pos = lst->free;
    (lst->data)[data_pos] = elem; // put elem to the next free cell in data
    lst->free = (lst->next)[lst->free]; // change free value to the address of the next free cell

    (lst->next)[data_pos] = (lst->next)[addr_of_el_before]; // setting new next's and prev's
    (lst->prev)[(lst->next)[addr_of_el_before]] = data_pos;
    (lst->next)[addr_of_el_before] = data_pos;
    (lst->prev)[data_pos] = addr_of_el_before;

    LIST_VERIFY(lst, After);

    return NoError; // change!
}


ListErr_t InsertBefore (List_t * lst, int addr_of_el_after, int elem)
{
    assert(lst);

    LIST_VERIFY(lst, Before);

    if (addr_of_el_after < 1 || addr_of_el_after > lst->capacity)
    {
        printf("Error at %s: Invalid element address\n", __FUNCTION__);
        return ListError;
    }

    if (InsertAfter(lst, (lst->prev)[addr_of_el_after], elem) != NoError)
        return ListError;

    LIST_VERIFY(lst, After);

    return NoError;
}


ListErr_t InsertToHead (List_t * lst, int elem)
{
    assert(lst);

    LIST_VERIFY(lst, Before);

    if (InsertBefore(lst, (lst->next)[0], elem) != NoError)
        return ListError; // message

    LIST_VERIFY(lst, After);

    return NoError;
}


ListErr_t InsertToTail (List_t * lst, int elem)
{
    assert(lst);

    LIST_VERIFY(lst, Before);

    if (InsertAfter(lst, (lst->prev)[0], elem) != NoError)
        return ListError;

    LIST_VERIFY(lst, After);

    return NoError;
}


ListErr_t DeleteEl (List_t * lst, int addr_of_el)
{
    assert(lst);

    LIST_VERIFY(lst, Before); // is assert needed?

    if ((lst->prev)[addr_of_el] == FREE_ADDRESS_INDICATOR)
    {
        printf("Error at %s: Element does not exist\n", __FUNCTION__);
        return ListError;
    }

    (lst->data)[addr_of_el] = POISON;
    (lst->next)[(lst->prev)[addr_of_el]] = (lst->next)[addr_of_el]; // changing next value for the prev el
    (lst->prev)[(lst->next)[addr_of_el]] = (lst->prev)[addr_of_el]; // changing prev value for the next el

    (lst->next)[addr_of_el] = lst->free; // writing the next free address (= value of free) in
    lst->free = addr_of_el;              // changing free value (to current data pos)

    (lst->prev)[addr_of_el] = FREE_ADDRESS_INDICATOR;

    LIST_VERIFY(lst, After);

    return NoError;
}


ListErr_t ListRealloc (List_t * lst)
{
    assert(lst);

    lst->capacity *= 2;
    lst->data = (ListEl *) realloc (lst->data, (size_t) lst->capacity);
    lst->next = (int    *) realloc (lst->next, (size_t) lst->capacity);
    lst->prev = (int    *) realloc (lst->next, (size_t) lst->capacity);

    if (lst->data == NULL)
        return MemAllocError;

    return NoError;
}


int ListVerify (List_t * lst) // получение элементов структуры через функции!
{
    assert(lst);

    int sum_of_err = NoError;

    if (lst == NULL)
    {
        sum_of_err |= NullListPtr;
        return sum_of_err;
    }
    if (lst->next == NULL)
    {
        sum_of_err |= NullNextPtr;
        return sum_of_err;
    }
    if (lst->prev == NULL)
    {
        sum_of_err |= NullPrevPtr;
        return sum_of_err;
    }
    if (lst->data == NULL)
    {
        sum_of_err |= NullDataPtr;
        return sum_of_err;
    }
    if ((lst->data)[0] != CANARY)
        sum_of_err |= ChangedCanaryVal;

    for (int count = 0; count < lst->capacity; count++) // checking if next and prev indexes are valid
    {
        if ((lst->next)[count] >= lst->capacity)
        {
            sum_of_err |= InvalidNextValue;
            return sum_of_err;
        }
        if ((lst->prev)[count] >= lst->capacity)
        {
            sum_of_err |= InvalidPrevValue;
            return sum_of_err;
        }
    }
    for (int count = 0; count < lst->capacity; count++) // checking connections between next and prev elements
    {
        if ((lst->data)[count] != POISON)
        {
            if ((lst->prev)[(lst->next)[count]] != count)
            {
                sum_of_err |= PrevNextConnectErr;
                return sum_of_err;
            }
        }
    }

    int count = 0;
    for (count = (lst->next)[0]; count < lst->capacity && (lst->next)[count] != 0; count = (lst->next)[count])
        count++;
    if (count > lst->capacity)
        sum_of_err |= CapacityOverflow;

    return sum_of_err;
}


ListErr_t GraphDump(List_t * lst, Place_t place, const char * func_name, const char * file_name, int num_of_line)
{
    assert(lst);
    assert(func_name);
    assert(file_name);

    char * pic_name = MakeGraphCodeFile(lst);
    const char * place_of_call = GetPlace(place);
    FILE * log = GetLog(lst);

    fprintf(log, "<pre>\n<h3> DUMP <font color=red>%s</font> %s </h3>\n", place_of_call, func_name);
    fprintf(log, "List {%s:%d}\n\n     ", file_name, num_of_line);

    int capacity = GetCapacity(lst);

    for (int count = 0; count < capacity; count++)
        fprintf(log, "%5d", count);
    fprintf(log, "\n\ndata:");
    for (int count = 0; count < capacity; count++)
        fprintf(log, "%5d", (lst->data)[count]);
    fprintf(log, "\n\nnext:");
    for (int count = 0; count < capacity; count++)
        fprintf(log, "%5d", (lst->next)[count]);
    fprintf(log, "\n\nprev:");
    for (int count = 0; count < capacity; count++)
        fprintf(log, "%5d", (lst->prev)[count]);
    fprintf(log, "\n\nfree: %d\n", lst->free);

    fprintf(log, "\n<h3></font>IMAGE:<h3> <img src=%s width=1600px>\n\n\n", pic_name); // choose file

    free(pic_name);
    return NoError;
}


FILE * GetLog (List_t * lst)
{
    assert(lst);

    return lst->log;
}


int GetCapacity (List_t * lst)
{
    assert(lst);

    return lst->capacity;
}


const char * GetPlace (Place_t place)
{
    if (place == Before)
        return "BEFORE";
    else if (place == After)
        return "AFTER"; // fix return
    else
        return "AT";
}


char * MakeGraphCodeFile (List_t * lst) // const List *
{
    assert(lst);

    static int pic_count = 0;

    FILE * fp = fopen("graphdump.txt", "w");
    assert(fp); // change

    //FIXME: define for colors
    fprintf(fp, "digraph G{\nrankdir=LR;\nf0 [shape=Mrecord; style = filled; fillcolor = \"#C0FFC0\"; color = \"#008000\"; label = \" head = %d | tail = %d | free = %d\"];\n", (lst->next)[0], (lst->prev)[0], lst->free);

    for (int count = 1; count < lst->capacity; count++)
    {
        fprintf(fp, "f%d [shape=Mrecord; style = filled; fillcolor = \"#C0FFC0\"; color = \"#008000\"; label = \"index = %d | value =  %d | {prev = %d | next = %d} \"]; \n", count, count, (lst->data)[count], (lst->prev)[count], (lst->next)[count]);
    }
    fprintf(fp, "head [shape=Mrecord; style = filled; fillcolor = \"#ffc0f6ff\"; color = \"#db36b7ff\"; label = \"HEAD\"; weight=\"100000\"]; \n");
    fprintf(fp, "tail [shape=Mrecord; style = filled; fillcolor = \"#ffc0f6ff\"; color = \"#db36b7ff\"; label = \"TAIL\"; weight=\"100000\"]; \n");
    for (int count = 0; count < lst->capacity - 1; count++)
    {
        fprintf(fp, "f%d -> f%d [weight = 100000, style = invis];\n", count, count + 1); // changed! + remove struct el?
    }
    for (int count = 1; count < lst->capacity - 1; count++)
    {
        if ((lst->data)[count] != POISON && (lst->next)[count] != 0)
        {
            if ((lst->next)[count] <= lst->capacity && (lst->prev)[(lst->next)[count]] == count)
            {
                fprintf(fp, "f%d -> f%d [color = \"#f63f6aff\", dir = both];\n", count, (lst->next)[count]); // changed! + remove struct el?
            }
            else
                fprintf(fp, "f%d -> f%d [color = \"#ff0000ff\"];\n", count, (lst->next)[count]);
        }
    }
    fprintf(fp, "head -> f%d [weight = 1; color = \"#ffc0f6ff\"];\n", (lst->next)[0]);
    fprintf(fp, "tail -> f%d [weight = 1; color = \"#ffc0f6ff\"];\n", (lst->prev)[0]);
    fprintf(fp, "}");

    fclose(fp);

    char * pic_name = GetPicName(pic_count);
    char * command  = GetCommand(pic_name);

    pic_count++;
    system(command); // check

    free(command);
    return pic_name;
}


char * GetCommand (char * pic_name) // const char *, return const char *
{
    assert(pic_name);

    char * command = (char *) calloc (SIZE_OF_COMMAND, sizeof(char));
    assert(command); // check

    sprintf(command, "dot graphdump.txt -T png -o %s", pic_name);

    return command;
}


char * GetPicName (int pic_count)
{
    char * pic_name = (char *) calloc (SIZE_OF_PIC_NAME, sizeof(char));
    assert(pic_name); // check

    sprintf(pic_name, "graphdump%d.png", pic_count);

    return pic_name;
}


ListErr_t ConsoleDump (List_t * lst)
{
    assert(lst);

    printf("data:");
    for (int count = 0; count < lst->capacity; count++)
        printf("%5d", (lst->data)[count]);
    printf("\nnext:");
    for (int count = 0; count < lst->capacity; count++)
        printf("%5d", (lst->next)[count]);
    printf("\nprev:");
    for (int count = 0; count < lst->capacity; count++)
        printf("%5d", (lst->prev)[count]);
    printf("\nfree: %d\n", lst->free);

    return NoError;
}


void ListDtor (List_t * lst)
{
    assert(lst);

    free(lst->data);
    free(lst->next);
    free(lst->prev);
}
