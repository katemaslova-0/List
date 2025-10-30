#ifndef DATA_H
#define DATA_H

typedef int ListEl;
const int START_SIZE = 10;

struct List_t
{
    ListEl * data;
    int * next;
    int * prev;
    int free;
    int capacity;
    FILE * log;
};

enum ListErr_t
{
    NoError             = (1 << 0),
    ListError           = (1 << 1),
    NullListPtr         = (1 << 2),
    NullDataPtr         = (1 << 3),
    NullNextPtr         = (1 << 4),
    NullPrevPtr         = (1 << 5),
    ChangedCanaryVal    = (1 << 6),
    PrevNextConnectErr  = (1 << 7),
    MemAllocError       = (1 << 8),
    FileOpeningError    = (1 << 9),
    InvalidNextValue    = (1 << 10),
    InvalidPrevValue    = (1 << 11),
    CapacityOverflow    = (1 << 12)
};

enum Place_t
{
    Before = 0,
    After  = 1,
    NoArg  = 2
};

ListErr_t ListCtor       (List_t * lst, FILE * fp);
int ListVerify           (List_t * lst);
void ListDtor            (List_t * lst);
char * MakeGraphCodeFile (List_t * lst);
ListErr_t FileDump       (List_t * lst);
ListErr_t ConsoleDump    (List_t * lst);
ListErr_t GraphDump      (List_t * lst, Place_t place, const char * func_name, const char * file_name, int num_of_line);

ListErr_t InsertAfter    (List_t * lst, int addr_of_el_before, int elem);
ListErr_t InsertBefore   (List_t * lst, int addr_of_el_after, int elem);
ListErr_t InsertToHead   (List_t * lst, int elem);
ListErr_t InsertToTail   (List_t * lst, int elem);
ListErr_t DeleteEl       (List_t * lst, int addr_of_el);

ListErr_t ListRealloc    (List_t * lst);
int GetCapacity          (List_t * lst);

void FillDataWithPoison       (List_t * lst);
void FillNextWithFreeAddresses (List_t * lst);
void FillPrevWithStartValue   (List_t * lst);

FILE * GetLog                 (List_t * lst);
char * GetPicName             (int pic_count);
char * GetCommand             (char * pic_name);
const char * GetPlace         (Place_t place);

#endif // DATA_H
