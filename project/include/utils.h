#ifndef PROJECT_INCLUDE_UTILS_H_
#define PROJECT_INCLUDE_UTILS_H_

#include <string.h>

#define SIZE 256

#define LENGTH_FROM 5
#define LENGTH_DATE 5
#define LENGTH_TO 3
#define LENGTH_CONTENT_TYPE 13

#define FROM_TO_DATE_COUNT 15
#define ONLY_FROM 1
#define ONLY_TO 2
#define ONLY_DATE 4
#define ONLY_COUNT 8
#define FROM_TO 3
#define FROM_DATE 5
#define TO_DATE 6
#define FROM_TO_DATE 7
#define FROM_COUNT 9
#define TO_COUNT 10
#define FROM_TO_COUNT 11
#define DATE_COUNT 12
#define FROM_DATE_COUNT 13
#define TO_DATE_COUNT 14

#define FILE_OPEN_ERR -1
#define MMAP_INIT_ERR -2
#define FSTAT_ERR -3
#define CREATE_ERR -4
#define STATE_ERR -5
#define QUANTITY_ARG_ERR -8


lexem_t get_lexem(const char *key);
int get_key_comparison(const char *key);

char *get_first_word(const char *str, unsigned offset);
void get_offset(const char *str, unsigned *offset);
int find_word(const char *str, const unsigned offset, const char* word);
int force_find_word(const char *str, const unsigned offset, const char* word);
char *get_boundary(const char *str, unsigned offset);
int get_count_part(const char *str, unsigned offset, size_t max_length);
const char *line_break(const char *str, const unsigned *offset, const unsigned len);
data_t* pars(void *src);
int handler(data_t *data);
void *mmap_foo(int fdin);

data_t *create_data();
void free_data(data_t *data);

#endif  // PROJECT_INCLUDE_UTILS_H_

