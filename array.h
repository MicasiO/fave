typedef struct {
    char** items;
    int capacity;
    int size;

} str_arr;

int init_str_arr(str_arr* arr);
int push_str_arr(str_arr* arr, const char* str);
int pop_str_arr(str_arr* arr, const char* str);
void free_str_arr(str_arr* arr);
