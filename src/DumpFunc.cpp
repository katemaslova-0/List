#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>

#include "Data.h"


void OutputTitle (FILE * log, const char * place, const char * func_name, const char * file_name, int num_of_line, ListElem_t * addr, int value, int result)
{
    assert(log);
    assert(place);
    assert(func_name);
    assert(file_name);

    if (strcmp(func_name, "ListCtor") == 0)
        fprintf(log, "<pre>\n<h3> DUMP <font color=red>%s</font> %s", place, func_name);
    else
    {
        fprintf(log, "<pre>\n<h3> DUMP <font color=red>%s</font> %s %p", place, func_name, addr);
        fprintf(log, " (element = %d) ", value);
    }

    if (result == SUCCESS)
        fprintf(log, " <font color=green>(SUCCESS)<font color=black></h3>\n");
    else
        fprintf(log, " <font color=red>(ERROR)<font color=black></h3>\n");
    fprintf(log, "Verified at: %s:%d\n\n\n", file_name, num_of_line);
    fprintf(log, "          ");
}


void OutputImage (FILE * log, List_t * lst, char * pic_name)
{
    assert(lst);
    assert(log);
    assert(pic_name);

    fprintf(log, "\nIMAGE:\n\n <img src=%s width=800px>\n\n\n", pic_name);
}


void OutputErrorCode (ListErr_t error)
{
    switch (error)
    {
        case NoError:                                                            break;
        case InvalidElemAddr:    printf("Invalid element adress\n");             break;
        case NullListPtr:        printf("Null list pointer\n");                  break;
        case NullListElemPtr:    printf("Null list element pointer\n");          break;
        case ChangedCanaryVal:   printf("Changed canary value\n");               break;
        case PrevNextConnectErr: printf("Error at next and prev connection\n");  break;
        case MemAllocError:      printf("Memory allocation error\n");            break;
        case FileOpeningError:   printf("File openinf error\n");                 break;
        case SizeOverflow:       printf("Too many connections(maybe cycles)\n"); break;
        case ElemDoesNotExist:   printf("Element does not exist\n");             break;
        default: ;
    }
}
