#ifndef PROJECT_INCLUDE_CHANGEFILE_H_
#define PROJECT_INCLUDE_CHANGEFILE_H_

typedef struct master_record {
int number;

char name[20];
char surname[20];
char addres[30];
char tel_number[15];

double indebtedness;
double credit_limit;
double cash_payments;
} Data;

int client_scanf(Data *client);
void client_printf_to_file(const Data *client, FILE *file);
int client_scanf_from_file(Data *client, FILE *file);
int transfer_scanf(Data *transfer);
void transfer_printf_to_file(const Data *transfer, FILE *file);
int transfer_scanf_from_file(Data *transfer, FILE *file);

#endif  // PROJECT_INCLUDE_CHANGEFILE_H_
