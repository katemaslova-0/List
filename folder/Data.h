#ifndef DATA_H
#define DATA_H

const int CANARY = 4387;

#define LIST_VERIFY(lst, place, addr, value) if (ListVerify(lst) != NoError) \
                                            {printf("ListDump at %s:%d\n", __FILE__, __LINE__); \
                                            ListErr_t error = ListVerify(lst); \
                                            OutputErrorCode(error); \
                                            GraphDump(lst, place, __FUNCTION__, __FILE__, __LINE__, addr, value, ERROR); \
                                            PtrAndErr_t result = {}; \
                                            result.ptr = NULL; \
                                            result.error = error; \
                                            return result;} \
                                            GraphDump(lst, place, __FUNCTION__, __FILE__, __LINE__, addr, value, SUCCESS);

struct ListElem_t
{
    int data;
    ListElem_t * next;
    ListElem_t * prev;
};

struct List_t
{
    ListElem_t * root;
    int size;
    FILE * log;
};

enum ListErr_t
{
    NoError             = 0,
    InvalidElemAddr     = 1,
    NullListPtr         = 2,
    NullListElemPtr     = 3,
    ChangedCanaryVal    = 4,
    PrevNextConnectErr  = 5,
    MemAllocError       = 6,
    FileOpeningError    = 7,
    SizeOverflow        = 8,
    ElemDoesNotExist    = 9
};

struct PtrAndErr_t
{
    ListElem_t * ptr;
    ListErr_t error;
};

enum Dump_t
{
    SUCCESS = 0,
    ERROR   = 1
};

enum Place_t
{
    Before = 0,
    After  = 1,
    NoArg  = 2
};

/*List functions*/
PtrAndErr_t ListCtor (List_t * lst, FILE * fp);
ListErr_t ListVerify (List_t * lst);
void MakeGraphCodeFile (List_t * lst);
void ListDtor (List_t * lst);
ListErr_t GraphDump (List_t * lst, Place_t place, const char * func_name, const char * file_name, int num_of_line, ListElem_t * addr, int value, int result);

/*Insert, Delete*/
PtrAndErr_t InsertAfter (List_t * lst, ListElem_t * lst_elem_before, int value);
PtrAndErr_t InsertBefore (List_t * lst, ListElem_t * lst_elem_after, int value);
PtrAndErr_t DeleteEl (List_t * lst, ListElem_t * lst_elem);

/*Get smth*/
FILE * GetLog         (List_t * lst);
int    GetCapacity    (List_t * lst);
char * GetCommand     (char * pic_name);
char * GetPicName     (void);
const char * GetPlace (Place_t place);

/*Inside functions for ListVerify*/
ListErr_t CheckNullPointers           (List_t * lst);
ListErr_t CheckCanaryValue            (List_t * lst);
ListErr_t CheckNextAndPrevConnections (List_t * lst);
ListErr_t CheckIfCycles               (List_t * lst);

/*Output functions for MakeGraphCode*/
void OutputNullCell         (FILE * fp, List_t * lst);
void OutputArrayCellsNodes  (FILE * fp, List_t * lst);
void OutputHeadAndTailNodes (FILE * fp);
void OutputAllEdges         (FILE * fp, List_t * lst);
void OutputArrayCellsEdges  (FILE * fp, List_t * lst);
void OutputHeadAndTailEdges (FILE * fp, List_t * lst);

/*Output functions for ListDump*/
void OutputTitle   (FILE * log, const char * place, const char * func_name, const char * file_name, int num_of_line, ListElem_t * addr, int elem, int result);
void OutputImage   (FILE * log, List_t * lst, char * pic_name);

/*Error messages*/
void OutputErrorCode (ListErr_t error);

#endif // DATA_H
