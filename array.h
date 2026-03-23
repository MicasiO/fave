typedef struct {
    char** items;
    int capacity;
    int size;

} StrArr;

int init_str_arr(StrArr* arr);
int push_str_arr(StrArr* arr, const char* str);
int pop_str_arr(StrArr* arr, const char* str);
void free_str_arr(StrArr* arr);
