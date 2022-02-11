#include <stdio.h>
#include <string.h>
#include "changefile.h"

int client_scanf(Data *client) {
    return scanf("%12d%11s%11s%16s%20s%12lf%12lf%12lf",
                 &client->number,
                 client->name,
                 client->surname,
                 client->addres,
                 client->tel_number,
                 &client->indebtedness,
                 &client->credit_limit,
                 &client->cash_payments);
}

void client_printf_to_file(const Data *client, FILE *file) {
    fprintf(file, "%-12d%-11s%-11s%-16s%20s%12.2f%12.2f%12.2f\n",
            client->number,
            client->name,
            client->surname,
            client->addres,
            client->tel_number,
            client->indebtedness,
            client->credit_limit,
            client->cash_payments);
}

int client_scanf_from_file(Data *client, FILE *file) {
    return fscanf(file, "%12d%11s%11s%16s%20s%12lf%12lf%12lf",
                  &client->number,
                  client->name,
                  client->surname,
                  client->addres,
                  client->tel_number,
                  &client->indebtedness,
                  &client->credit_limit,
                  &client->cash_payments);
}

int transfer_scanf(Data *transfer) {
    return scanf("%d %lf",
                 &transfer->number,
                 &transfer->cash_payments);
}

void transfer_printf_to_file(const Data *transfer, FILE *file) {
    fprintf(file, "%-3d%-6.2f\n",
            transfer->number,
            transfer->cash_payments);
}

int transfer_scanf_from_file(Data *transfer, FILE *file) {
    return fscanf(file, "%d %lf",
                  &transfer->number,
                  &transfer->cash_payments);
}

int is_equality(const Data *client_one, const Data *client_two) {
    return client_one->number == client_two->number &&
        !strncmp(client_one->name, client_two->name, strlen(client_one->name)) &&
        !strncmp(client_one->surname, client_two->surname, strlen(client_one->surname)) &&
        !strncmp(client_one->addres, client_two->addres, strlen(client_one->addres)) &&
        !strncmp(client_one->tel_number, client_two->tel_number, strlen(client_one->tel_number)) &&
        client_one->indebtedness == client_two->indebtedness &&
        client_one->credit_limit == client_two->credit_limit &&
        client_one->cash_payments == client_two->cash_payments;
}
