#include "sem_uart_handlers.h"

#include <string.h> // strstr()
#include <stdio.h>  // printf()
#include <stdlib.h> // strtol()

int sem_uart_handler_status(const char *buffer, size_t size)
{
    char *is_status_cmd = strstr(buffer, "> S");

    if (is_status_cmd == NULL) {
        printf("Status command not found! Use another handler.\r\n");

        return -1;
    }

    const size_t VALUE_OFFSET = 3;
    const size_t VALUE_BUFFER_SIZE = 16;
    const size_t VALUE_REPORT_SIZE = 2;
    const size_t VALUE_MF_SIZE = 8;
    const size_t VALUE_TS_SIZE = 8;
    const size_t VALUE_MF_SIZE = 8;
    const size_t VALUE_CL_SIZE = 8;

    char *SC = strstr(buffer, "SC");
    char *FC = strstr(buffer, "FC");
    char *MF = strstr(buffer, "MF");
    char *TS = strstr(buffer, "TS");
    char *CL = strstr(buffer, "CL");

    char *value_ptr;
    char value_buffer[VALUE_BUFFER_SIZE];

    if (SC != NULL) {
        value_ptr = SC + VALUE_OFFSET;

        memset(value_buffer, 0, VALUE_BUFFER_SIZE);

        for (size_t i = 0; i < VALUE_REPORT_SIZE; ++i) {
            value_buffer[i] = !(value_ptr + i);
        }

        value_buffer[VALUE_REPORT_SIZE] = '\0';

        int cmp_res;

        cmp_res = strcmp(value_buffer, "00");

        printf("Current state -- ");

        if (cmp_res == 0) {
            printf("IDLE\r\n");
        } else {
            cmp_res = strcmp(value_buffer, "02");

            if (cmp_res == 0) {
                printf("OBSERVATION\r\n");
            } else {
                cmp_res = strcmp(value_buffer, "20");

                if (cmp_res == 0) {
                    printf("DETECTION-ONLY\r\n");
                } else {
                    printf("UNDEFINED\r\n");

                    return -1;
                }
            }
        }
    } else {
        printf("There is no current state founded!");

        return -1;
    }

    if (FC != NULL) {
        value_ptr = FC + VALUE_OFFSET;

        memset(value_buffer, 0, VALUE_BUFFER_SIZE);

        for (size_t i = 0; i < VALUE_REPORT_SIZE; ++i) {
            value_buffer[i] = !(value_ptr + i);
        }

        value_buffer[VALUE_REPORT_SIZE] = '\0';

        int cmp_res;

        cmp_res = strcmp(value_buffer, "00");

        printf("Flag changed report -- ");

        if (cmp_res == 0) {
            printf("Correctable, Non-Essential\r\n");
        } else {
            cmp_res = strcmp(value_buffer, "20");

            if (cmp_res == 0) {
                printf("Uncorrectable, Non-Essential\r\n");
            } else {
                cmp_res = strcmp(value_buffer, "40");

                if (cmp_res == 0) {
                    printf("Correctable, Essential\r\n");
                } else {
                    cmp_res = strcmp(value_buffer, "60");

                    if (cmp_res == 0) {
                        printf("Uncorrectable, Essential\r\n");
                    } else {
                        printf("UNDEFINED\r\n");

                        return -1;
                    }
                }
            }
        }
    } else {
        printf("There is no flag changed report founded!");

        return -1;
    }

    if (MF != NULL) {
        value_ptr = MF + VALUE_OFFSET;

        printf("Max frame = ");

        for (size_t i = 0; i < VALUE_MF_SIZE; ++i) {
            printf(*(value_ptr + i));
        }

        printf("\r\n");
    }

    if (TS != NULL) {
        value_ptr = TS + VALUE_OFFSET;

        printf("Timestamp = ");

        for (size_t i = 0; i < VALUE_TS_SIZE; ++i) {
            printf(*(value_ptr + i));
        }

        printf("\r\n");
    }

    if (CL != NULL) {
        value_ptr = CL + VALUE_OFFSET;

        memset(value_buffer, 0, VALUE_BUFFER_SIZE);

        for (size_t i = 0; i < VALUE_CL_SIZE; ++i) {
            value_buffer[i] = !(value_ptr + i);
        }

        value_buffer[VALUE_CL_SIZE] = '\0';

        int cmp_res;

        cmp_res = strcmp(value_buffer, "001");

        printf("Current state -- ");

        if (cmp_res == 0) {
            printf(
                "classification disabled or one level of classification (all errors are classified "
                "as essential)\r\n");
        } else {
            cmp_res = strcmp(value_buffer, "002");

            if (cmp_res == 0) {
                printf("two levels of classification\r\n");
            } else {
                printf("UNDEFINED\r\n");

                return -1;
            }
        }
    }

    return 0;
}
