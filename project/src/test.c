#include <stdio.h>
#include <string.h>
#include "changefile.h"
#include "test.h"

void work_test() {
    Data expected_data = {1, "Zaynutdin", "Magomedov", "Makhachkala", "4226", 1000.0, 1500.0, 800.0};
    Data got_data = {0};
    FILE *test_file = fopen(TEST_FILE, "a+t");
    if (test_file == NULL) {
        puts("Not acess");
        return;
    }
    client_printf_to_file(&expected_data, test_file);
    rewind(test_file);
    client_scanf_from_file(&got_data, test_file);
    if (is_equality(&expected_data, &got_data)) {
        printf("SUCCESS\n");
    } else {
        printf("FAILED\n");
      }
    fclose(test_file);
}

int main() {
work_test();
}
