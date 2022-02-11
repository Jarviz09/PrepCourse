#ifndef PROJECT_INCLUDE_ACTION_H_
#define PROJECT_INCLUDE_ACTION_H_

#define SIZE_IN_DATA 256

int set_from(const char *str, const unsigned *offset, data_t *data);
int set_date(const char *str, const unsigned *offset, data_t *data);
int set_to(const char *str, const unsigned *offset, data_t *data);
int set_content_type(const char *str, const unsigned *offset, data_t *data);
char* get_alloc(data_t *data, int power, char *val);

#endif  // PROJECT_INCLUDE_ACTION_H_


