#include <stdio.h>
#include <stdlib.h>
#include "changefile.h"
#include "process.h"

#define MASTER_WRITE 1
#define TRANSACTION_WRITE 2
#define BLACK_RECORD 3

#define MENU "please enter action:\n"\
             "1 enter data client\n"\
             "2 enter data transaction\n"\
             "3 update base\n"

int main(void) {
    int is_error;
    int choice = 0;
    printf("%s", MENU);

    while (scanf("%d", &choice) != 1) {
        switch (choice) {
            case MASTER_WRITE:
                is_error = master_write();
                break;
            case TRANSACTION_WRITE:
                is_error = transaction_write();
                break;
            case BLACK_RECORD:
                is_error = black_record();
                break;
            default:
                puts("error");
                break;
        }
        printf("%s", MENU);
    }
    return is_error;
}
