#include "main.h"
#include "utils.h"
#include "action.h"

#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>

char success_bin = 0;
char *boundary;
size_t max_len;

rule_t syntax[STATE_COUNT][LEXEM_COUNT] = {
       /*FROM:*/           /*TO*/          /*DATE*/          /*CONTENT_TYPE*/            /*NOTHING*/
/*BEGIN*/       {{STATE_FROM, set_from},   {STATE_TO, set_to},     {STATE_DATE, set_date},
{STATE_NOTHING, set_content_type},  {STATE_NOTHING, NULL}},
/*FROM*/           {{STATE_ERROR, set_from},  {STATE_TO, set_to},     {STATE_DATE, set_date},
{STATE_NOTHING, set_content_type},  {STATE_NOTHING, NULL}},
/*TO*/             {{STATE_FROM, set_from},   {STATE_ERROR, set_to},  {STATE_DATE, set_date},
{STATE_NOTHING, set_content_type},  {STATE_NOTHING, NULL}},
/*DATE*/           {{STATE_FROM, set_from},   {STATE_TO, set_to},     {STATE_ERROR, set_date},
{STATE_NOTHING, set_content_type},  {STATE_NOTHING, NULL}},
/*NOTHING*/        {{STATE_FROM, set_from},   {STATE_TO, set_to},     {STATE_DATE, set_date},
{STATE_NOTHING, set_content_type},  {STATE_NOTHING, NULL}},
};

int main(int argc, const char **argv) {
    if (argc != 2) {
        return QUANTITY_ARG_ERR;
    }

    struct stat statbuf = {0};

    const char *path_to_eml = argv[1];
    int fdin = open(path_to_eml, O_RDONLY);

    if (fdin < 0) {
        return FILE_OPEN_ERR;
    }

    if (fstat(fdin, &statbuf) < 0) {
        close(fdin);
        return FSTAT_ERR;
    }

    max_len = statbuf.st_size;
    void *src = mmap_foo(fdin);
    if (src == 0) {
        close(fdin);
        return MMAP_INIT_ERR;
    }

    data_t *data = pars(src);
    if (data == NULL) {
        munmap(src, statbuf.st_size);
        return CREATE_ERR;
    }

    return 0;
}


