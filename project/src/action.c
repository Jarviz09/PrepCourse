#include "main.h"
#include "action.h"
#include "utils.h"

#include <stdlib.h>
#include <string.h>
#include <math.h>

#include<stdio.h>

extern char success_bin;
extern char *boundary;
extern size_t max_len;

char* get_alloc(data_t *data, int power, char *val) {
    val = realloc((void *)val, SIZE_IN_DATA * pow(2., power));
    if (val == NULL) {
        free(data->to);
        free(data->date);
        free(data);
        return NULL;
    }

    return val;
}

int set_from(const char *str, const unsigned *offset, data_t *data) {
    int i = 0;
    int power = 1;

    while (str[*offset + LENGTH_FROM] != '\n' && str[*offset + LENGTH_FROM] != '\r') {
        if (i == 0 && str[*offset + LENGTH_FROM] == ' ') {
            ++str;
        } else {
            data->from[i++] = str[*offset + LENGTH_FROM];
            if (i % (int)(SIZE_IN_DATA * pow(2., power - 1)) == 0) {
                char *save_data_from = data->from;
                data->from = get_alloc(data, power++, data->from);
                if (data->from == NULL) {
                    free(save_data_from);
                    free(data->to);
                    free(data->date);
                    free(data);
                    return -1;
                }
            }
            ++str;
        }
        str = line_break(str, offset, LENGTH_FROM);
    }
    data->from[i] = '\0';

    success_bin |= 1;

    return 0;
}

int set_date(const char *str, const unsigned *offset, data_t *data) {
    int i = 0;
    int power = 1;

    while (str[*offset + LENGTH_DATE] != '\n' && str[*offset + LENGTH_DATE] != '\r') {
        if (i == 0 && str[*offset + LENGTH_DATE] == ' ') {
            ++str;
        } else {
            data->date[i++] = str[*offset + LENGTH_DATE];
            if (i % (int)(SIZE_IN_DATA * pow(2., power - 1)) == 0) {
                char *save_data_date = data->date;
                data->date = get_alloc(data, power++, data->date);
                if (data->date == NULL) {
                    free(data->from);
                    free(data->to);
                    free(save_data_date);
                    free(data);

                    return -1;
                }
            }
            ++str;
        }
        str = line_break(str, offset, LENGTH_DATE);
    }
    data->date[i] = '\0';

    success_bin |= 4;

    return 0;
}

int set_to(const char *str, const unsigned *offset, data_t *data) {
    int i = 0;
    int power = 1;
    while (str[*offset + LENGTH_TO] != '\n' && str[*offset + LENGTH_TO] != '\r') {
        if (i == 0 && str[*offset + LENGTH_TO] == ' ') {
            ++str;
        } else {
            data->to[i++] = str[*offset + LENGTH_TO];
            if (i % (int)(SIZE_IN_DATA * pow(2., power - 1)) == 0) {
                char *save_data_to = data->to;
                data->to = get_alloc(data, power++, data->to);
                if (data->to == NULL) {
                    free(data->from);
                    free(save_data_to);
                    free(data->date);
                    free(data);

                    return -1;
                }
            }
            ++str;
        }
        str = line_break(str, offset, LENGTH_TO);
    }
    data->to[i] = '\0';

    success_bin |= 2;

    return 0;
}

int set_content_type(const char *str, const unsigned *offset, data_t *data) {
    if (success_bin >= 8) {
        return 0;
    }
    if (find_word(str, *offset + LENGTH_CONTENT_TYPE, "multipart") == -1) {
        data->part_count = 1;
        success_bin |= 8;
        return 0;
    }

    unsigned end_word_offset = force_find_word(str, *offset + LENGTH_CONTENT_TYPE, "boundary");
    if (end_word_offset == 1) {
        data->part_count = 1;
        success_bin |= 8;
        return 0;
    }

    boundary = get_boundary(str, end_word_offset + 1);
    if (boundary == NULL) {
        free_data(data);
        return -1;
    }

    data->part_count = get_count_part(str, *offset, max_len);
    success_bin |= 8;
    free(boundary);

    return 0;
}

