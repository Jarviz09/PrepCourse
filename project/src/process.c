#include <stdio.h>
#include <stdlib.h>
#include "changefile.h"
#include "process.h"

#define CLIENT_INFO "1 Number account:\n"\
                   "2 Client name:\n"\
                   "3 Surname:\n"\
                   "4 Addres client:\n"\
                   "5 Client Telnum:\n"\
                   "6 Client indebtedness:\n"\
                   "7 Client credit limit:\n"\
                   "8 Client cash payments:\n"

#define TRANSACTION_INFO "1 Number account:\n"\
                        "2 Client cash payments:\n"

int black_record() {
    FILE *record = fopen(FILE_RECORD, "rt");
    if (record == NULL) {
        return ERR_OPEN_FILE;
    }
    FILE *transaction = fopen(FILE_TRANSACTION, "rt");
    if (transaction == NULL) {
        fclose(record);
        return ERR_OPEN_FILE;
    }
    FILE *black_record = fopen(FILE_BLACK_RECORD, "wt");
    if (black_record == NULL) {
        fclose(record);
        fclose(transaction);
        return ERR_OPEN_FILE;
    }
    client_write(record, transaction, black_record);
    fclose(record);
    fclose(transaction);
    fclose(black_record);
    return SUCCESS;
}

void client_write(FILE *record, FILE *transaction, FILE *black_record) {
    Data client_data = {0};
    Data transfer = {0};

    while (client_scanf_from_file(&client_data, record) == ENTRY_NUMBER) {
        while (transfer_scanf_from_file(&transfer, transaction) == TRANSFER_NUMBER) {
            if (client_data.number == transfer.number && transfer.cash_payments != 0) {
                client_data.credit_limit += transfer.cash_payments;
            }
        }

        client_printf_to_file(&client_data, black_record);
        rewind(transaction);
    }
}

int master_write() {
    Data client = {0};
    FILE *record = fopen(FILE_RECORD, "wt");
    if (record == NULL) {
        puts("Not acess");
        return ERR_OPEN_FILE;
    }
    printf("%s", CLIENT_INFO);
    while (client_scanf(&client) == ENTRY_NUMBER) {
        client_printf_to_file(&client, record);
        printf("%s", CLIENT_INFO);
    }
    fclose(record);
    return SUCCESS;
}

int transaction_write() {
    Data transfer = {0};
    FILE *transaction = fopen(FILE_TRANSACTION, "wt");
    if (transaction == NULL) {
        puts("Not acess");
        return ERR_OPEN_FILE;
    } else {
        printf("%s", TRANSACTION_INFO);
        while (transfer_scanf(&transfer) == TRANSFER_NUMBER) {
            transfer_printf_to_file(&transfer, transaction);
            printf("%s", TRANSACTION_INFO);
        }
    }
    fclose(transaction);
    return SUCCESS;
}

