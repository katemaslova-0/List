#ifndef DATA_H
#define DATA_H

const int START_SIZE = 10;
const int CANARY = 5498;
const int POISON = 333;
const int SIZE_OF_PIC_NAME = 80;
const int SIZE_OF_COMMAND = 80;
const int FREE_ADDRESS_INDICATOR = -1;

#define LIST_VERIFY(lst, place, addr, elem) if (ListVerify(lst) != NoError) \
                                      {printf("ListDump at %s:%d\n", __FILE__, __LINE__); \
                                      int error = ListVerify(lst); \
                                      OutputErrorCode(error); \
                                      GraphDump(lst, place, __FUNCTION__, __FILE__, __LINE__, addr, elem, ERROR); \
                                      return error;} \
                                      GraphDump(lst, place, __FUNCTION__, __FILE__, __LINE__, addr, elem, SUCCESS);
struct List_t
{
    int * data;
    int * next;
    int * prev;
    int free;
    int capacity;
    FILE * log;
};

enum Dump_t
{
    SUCCESS = 0,
    ERROR   = 1
};

enum ListErr_t
{
    NoError             = -1,
    InvalidElemAddr     = -2,
    NullListPtr         = -3,
    NullDataPtr         = -4,
    NullNextPtr         = -5,
    NullPrevPtr         = -6,
    ChangedCanaryVal    = -7,
    PrevNextConnectErr  = -8,
    MemAllocError       = -9,
    FileOpeningError    = -10,
    InvalidNextValue    = -11,
    InvalidPrevValue    = -12,
    CapacityOverflow    = -13,
    ElemDoesNotExist    = -14
};

enum Array_t
{
    Data = 0,
    Next = 1,
    Prev = 2
};

enum Place_t
{
    Before = 0,
    After  = 1,
    NoArg  = 2
};

/*List functions*/
int ListCtor             (List_t * lst, FILE * fp);
int ListVerify           (List_t * lst);
void ListDtor            (List_t * lst);
void MakeGraphCodeFile   (List_t * lst);
ListErr_t GraphDump      (List_t * lst, Place_t place, const char * func_name, const char * file_name, int num_of_line, int addr, int elem, int result);
ListErr_t ListRealloc    (List_t * lst);

/*Insert, Delete*/
int InsertAfter          (List_t * lst, int addr_of_el_before, int elem);
int InsertBefore         (List_t * lst, int addr_of_el_after, int elem);
int InsertToHead         (List_t * lst, int elem);
int InsertToTail         (List_t * lst, int elem);
int DeleteEl             (List_t * lst, int addr_of_el);

/*Fill arrays with start values*/
void FillDataWithPoison        (List_t * lst, int start);
void FillNextWithFreeAddresses (List_t * lst, int start);
void FillPrevWithStartValue    (List_t * lst, int start);

/*Get smth*/
FILE * GetLog         (List_t * lst);
int    GetCapacity    (List_t * lst);
char * GetCommand     (char * pic_name);
char * GetPicName     (void);
const char * GetPlace (Place_t place);

/*Inside functions for Insert/Delete*/
int PutElemToData                     (List_t * lst, int elem);
ListErr_t CheckIfAddressValid         (List_t * lst, int address);
ListErr_t CheckIfElemBeforeExist      (List_t * lst, int address);
ListErr_t CallReallocIfNeed           (List_t * lst);

/*Inside functions for ListVerify*/
ListErr_t CheckNullPointers           (List_t * lst);
ListErr_t CheckCanaryValue            (List_t * lst);
ListErr_t CheckNextAndPrevConnections (List_t * lst);
ListErr_t CheckIfIndexesValid         (List_t * lst);
ListErr_t CheckIfCycles               (List_t * lst);

/*Output functions for MakeGraphCode*/
void OutputNullCell         (FILE * fp, List_t * lst);
void OutputArrayCellsNodes  (FILE * fp, List_t * lst);
void OutputHeadAndTailNodes (FILE * fp, List_t * lst);
void OutputAllEdges         (FILE * fp, List_t * lst);
void OutputArrayCellsEdges  (FILE * fp, List_t * lst);
void OutputHeadAndTailEdges (FILE * fp, List_t * lst);

/*Output functions for ListDump*/
void OutputTitle   (FILE * log, const char * place, const char * func_name, const char * file_name, int num_of_line, int addr, int elem, int result);
void OutputIndexes (FILE * log, List_t * lst);
void OutputData    (FILE * log, List_t * lst);
void OutputNext    (FILE * log, List_t * lst);
void OutputPrev    (FILE * log, List_t * lst);
void OutputFree    (FILE * log, List_t * lst);
void OutputImage   (FILE * log, List_t * lst, char * pic_name);

/*Error messages*/
void OutputErrorCode (int error);

#endif // DATA_H
