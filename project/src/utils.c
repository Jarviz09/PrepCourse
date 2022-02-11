#include "main.h"
#include "utils.h"

#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <stdio.h>
#include <ctype.h>
#include <sys/mman.h>

extern size_t max_len;
extern char success_bin;

extern rule_t syntax[STATE_COUNT][LEXEM_COUNT];
extern char *boundary;

lexem_t get_lexem(const char *key) {
    switch (get_key_comparison(key)) {
        case 1:
            return LEXEM_FROM;
        case 2:
            return LEXEM_TO;
        case 3:
            return LEXEM_DATE;
        case 4:
            return LEXEM_CONTENT_TYPE;
        default:
            return LEXEM_NOTHING;
    }
}

int get_key_comparison(const char *key) {
    if (strcmp(key, "From:") == 0) {
        return 1;
    }
    if (strcmp(key, "To:") == 0) {
        return 2;
    }
    if (strcmp(key, "Date:") == 0) {
        return 3;
    }
    if (strcmp(key, "Content-Type:") == 0) {
        return 4;
    }
    return 0;
}

char *get_first_word(const char *str, unsigned offset) {
    double power = 1;
    int i = 0;
    char *fword = (char *)malloc(SIZE);

    if (fword == NULL) {
        return NULL;
    }

    while (*(str + offset) != ' ' && *(str + offset) != '\n'
           && *(str + offset) != '\r' && *(str + offset) != '"' && offset <= max_len) {
        fword[i++] = *(str + offset);

        if (i % (int)(SIZE * pow(2., power - 1)) == 0) {
            char *save_fword = fword;

            fword = (char *)realloc((void *)fword, SIZE * pow(2., power++));
            if (fword == NULL) {
                free(save_fword);
                return NULL;
            }
        }
        ++offset;
    }

    fword[i] = '\0';

    return fword;
}

void get_offset(const char *str, unsigned *offset) {
    unsigned result_offset = *offset;
    while (*(str + *offset) != '\n' && *(str + *offset) != '\r' && *offset <= max_len) {
        ++result_offset;
        ++(*offset);
    }

    *offset = result_offset + 1;
}

data_t *create_data() {
    data_t *data = (data_t *)malloc(sizeof(data_t));
    if (data == NULL) {
        return NULL;
    }

    data->from = (char *)malloc(SIZE);
    if (data->from == NULL) {
        free(data);
        return NULL;
    }
    data->to = (char *)malloc(SIZE);
    if (data->to == NULL) {
        free(data->from);
        free(data);
        return NULL;
    }
    data->date = (char *)malloc(SIZE);
    if (data->date == NULL) {
        free(data->to);
        free(data->from);
        free(data);
        return NULL;
    }

    return data;
}

void free_data(data_t *data) {
    free(data->from);
    free(data->to);
    free(data->date);

    free(data);
}

int find_word(const char *str, const unsigned offset, const char* word) {
    int length = strlen(word);
    while (str[offset] != '\n' && str[offset] != '\r') {
        for (int i = 0; tolower(str[offset + i]) == word[i]; ++i) {
            if (i == length - 1) {
                return 0;
            }
        }
        ++str;
    }

    return -1;
}

int force_find_word(const char *str, unsigned offset, const char* word) {
    unsigned length = strlen(word);
    while (offset <= max_len) {
        for (unsigned i = 0; tolower(str[offset + i]) == word[i]; ++i) {
            if (i == length - 1) {
                if (str[offset - 1] != ' ' && str[offset - 1] != ';' && str[offset - 1] != '\t') {
                    return 1;
                }
                return (offset + i + 1);
            }
        }
        ++offset;
    }
    return -1;
}

char *get_boundary(const char *str, unsigned offset) {
    char *key = (char *)malloc(SIZE);
    if (key == NULL) {
        return NULL;
    }
    int i = 0;
    int power = 1;
    if (str[offset] == '=') {
        ++offset;
    }

    if (str[offset] == ' ') {
        ++offset;
    }

    if (str[offset] == '"') {
        ++offset;
    }
    while (str[offset] != '\n' && str[offset] != '\r') {
        if (str[offset] == '"' || str[offset] == ' ') {
            break;
        }
        key[i++] = str[offset++];

        if (i % (int)(SIZE * pow(2., power - 1)) == 0) {
            char *save_key = key;
            key = (char *)realloc((void *)key, SIZE * pow(2., power++));

            if (key == NULL) {
                free(save_key);
                return NULL;
            }
        }
    }

    key[i] = '\0';

    return key;
}

int get_count_part(const char *str, unsigned offset, size_t max_len) {
    int part_count = 0;
    char *res_boundary = (char*)malloc(3 + strlen(boundary));
    snprintf(res_boundary, strlen(boundary) + 3, "%s%s", "--", boundary);
    while (offset < max_len) {
        char *key = get_first_word(str, offset);
        if (key == NULL) {
            free(res_boundary);
            return -1;
        }

        if (strcmp(res_boundary, key) == 0) {
            ++part_count;
        }

        free(key);
        get_offset(str, &offset);
    }
    free(res_boundary);

    return part_count;
}

const char *line_break(const char *str, const unsigned *offset, const unsigned len) {
    if (str[*offset + len] == '\n' || str[*offset + len] == '\r') {
        if (str[*offset + len + 1] == ' ' || str[*offset + len + 1] == '\n') {
            while (str[*offset + len + 1] == ' ' || str[*offset + len + 1] == '\n') {
                str++;
            }
        }
    }
    return str;
}

data_t* pars(void *src) {
    data_t *data = create_data();
    unsigned offset = 0;
    state_t state = STATE_BEGIN;
    while (state != STATE_END && state != STATE_ERROR && success_bin != FROM_TO_DATE_COUNT) {
        char *key_word = get_first_word((char *)src, offset);

        if (key_word == NULL) {
            free_data(data);
            munmap(src, max_len);
            return NULL;
        }

        lexem_t lexem = get_lexem(key_word);
        free(key_word);
        rule_t rule = syntax[state][lexem];

        if (rule.action) {
            int res = rule.action((char *)src, &offset, data);
            if (res == -1) {
                munmap(src, max_len);
                return NULL;
            }
        }

        state = rule.state;

        get_offset((char *)src, &offset);  // Поиск смещения на новую строку

        if (offset >= max_len) {
            state = STATE_END;
        }
    }

    if (success_bin == FROM_TO_DATE) {
       data->part_count = 1;
    }

    if (handler(data) == 0) {
        free_data(data);
        munmap(src, max_len);
        return data;
    }

    if (state == STATE_ERROR) {
        free_data(data);
        munmap(src, max_len);
        return NULL;
    }

    return data;
}

int handler(data_t *data) {
    switch (success_bin) {
        case ONLY_FROM:
            printf("%s|||%d\n", data->from, data->part_count);
            return 0;

        case ONLY_TO:
            printf("|%s||%d\n", data->to, data->part_count);
            return 0;

        case FROM_TO:
            printf("%s|%s||%d\n", data->from, data->to, data->part_count);
            return 0;

        case ONLY_DATE:
            printf("||%s|\n", data->date);
            return 0;

        case FROM_DATE:
            printf("%s||%s|%d\n", data->from, data->date, data->part_count);
            return 0;

        case TO_DATE:
            printf("|%s|%s|%d\n", data->to, data->date, data->part_count);
            return 0;

        case FROM_TO_DATE:
            printf("%s|%s|%s|%d\n", data->from, data->to, data->date, data->part_count);
            return 0;

        case ONLY_COUNT:
            printf("|||%d\n", data->part_count);
            return 0;

        case FROM_COUNT:
            printf("%s|||%d\n", data->from, data->part_count);
            return 0;

        case TO_COUNT:
            printf("|%s||%d\n", data->to, data->part_count);
            return 0;

        case FROM_TO_COUNT:
            printf("%s|%s||%d\n", data->from, data->to, data->part_count);
            return 0;

        case DATE_COUNT:
            printf("||%s|%d\n", data->date, data->part_count);
            return 0;

        case FROM_DATE_COUNT:
            printf("%s||%s|%d\n", data->from, data->date, data->part_count);
            return 0;

        case TO_DATE_COUNT:
            printf("|%s|%s|%d\n", data->to, data->date, data->part_count);
            return 0;

        case FROM_TO_DATE_COUNT:
            printf("%s|%s|%s|%d\n", data->from, data->to, data->date, data->part_count);
            return 0;
    }

    return -1;
}

void *mmap_foo(int fdin) {
    void *src = mmap(0, max_len, PROT_READ, MAP_SHARED, fdin, 0);
    if (src == MAP_FAILED) {
        return 0;
    }

    return src;
}

