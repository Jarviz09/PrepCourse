#ifndef PROJECT_INCLUDE_PROCESS_H_
#define PROJECT_INCLUDE_PROCESS_H_

#define FILE_RECORD "record.dat"
#define FILE_TRANSACTION "transaction.dat"
#define FILE_BLACK_RECORD "blackrecord.dat"

#define ENTRY_NUMBER    8
#define TRANSFER_NUMBER 2
#define ERR_OPEN_FILE  -1
#define SUCCESS         0

int master_write();
int transaction_write();
int black_record();
void client_write(FILE *record, FILE *transaction, FILE *black_record);

#endif  // PROJECT_INCLUDE_PROCESS_H_
