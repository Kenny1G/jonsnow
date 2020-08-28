#include <assert.h>
#include <ctype.h>
#include <limits.h>
#include <math.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char* readline();
char* ltrim(char*);
char* rtrim(char*);
char** split_string(char*);

int parse_int(char*);



/*
 * Complete the 'findBeforeMatrix' function below.
 *
 * The function is expected to return a 2D_INTEGER_ARRAY.
 * The function accepts 2D_INTEGER_ARRAY after as parameter.
 */

/*
 * To return the 2d integer array from the function, you should:
 *     - Store the number of rows of the array to be returned in the result_rows variable
 *     - Store the number of columns of the array to be returned in the result_columns variable
 *     - Allocate the array dynamically
 *
 * For example,
 * int** return_2d_integer_array(int* result_rows, int* result_columns) {
 *     *result_rows = 2;
 *     *result_columns = 3;
 *
 *     int** a = malloc(2 * sizeof(int*));
 *
 *     for (int i = 0; i < 2; i++) {
 *         *(a + i) = malloc(3 * sizeof(int));
 *
 *         for (int j = 0; j < 3; j++) {
 *             *(*(a + i) + j) = 3 * i + j + 1;
 *         }
 *     }
 *
 *     return a;
 * }
 *
 */
int** findBeforeMatrix(int after_rows, int after_columns, int** after, int* result_rows, int* result_columns) {
   int** before = malloc(after_rows * sizeof(int*));
   int s;

   for (int x = 0; x < after_rows; ++x) {
      *(before + x) = malloc(after_columns * (sizeof(int)));
      for (int y = 0; y < after_columns; ++y) {
         s = 0;
         for (int i = 0; i <=x; ++i) {
            for (int j = 0; j <= y; ++j) {
               s = before[i][j] +  s;
            }
         }
         before[x][y] = after[x][y] - s;
      }
   }
   *result_rows = after_rows;
   *result_columns = after_columns;


   return before;
}

int main()
{
    //FILE* fptr = fopen(getenv("OUTPUT_PATH"), "w");

    int after_rows = parse_int(ltrim(rtrim(readline())));

    int after_columns = parse_int(ltrim(rtrim(readline())));

    int** after = malloc(after_rows * sizeof(int*));

    for (int i = 0; i < after_rows; i++) {
        *(after + i) = malloc(after_columns * (sizeof(int)));

        char** after_item_temp = split_string(rtrim(readline()));

        for (int j = 0; j < after_columns; j++) {
            int after_item = parse_int(*(after_item_temp + j));

            *(*(after + i) + j) = after_item;
        }
    }

    int result_rows;
    int result_columns;
    int** result = findBeforeMatrix(after_rows, after_columns, after, &result_rows, &result_columns);

    for (int i = 0; i < result_rows; i++) {
        for (int j = 0; j < result_columns; j++) {
            printf( "%d", *(*(result + i) + j));

            if (j != result_columns - 1) {
                printf( " ");
            }
        }

        if (i != result_rows - 1) {
            printf( "\n");
        }
    }

    printf( "\n");

    //fclose(fptr);

    return 0;
}

char* readline() {
    size_t alloc_length = 1024;
    size_t data_length = 0;

    char* data = malloc(alloc_length);

    while (true) {
        char* cursor = data + data_length;
        char* line = fgets(cursor, alloc_length - data_length, stdin);

        if (!line) {
            break;
        }

        data_length += strlen(cursor);

        if (data_length < alloc_length - 1 || data[data_length - 1] == '\n') {
            break;
        }

        alloc_length <<= 1;

        data = realloc(data, alloc_length);

        if (!data) {
            data = '\0';

            break;
        }
    }

    if (data[data_length - 1] == '\n') {
        data[data_length - 1] = '\0';

        data = realloc(data, data_length);

        if (!data) {
            data = '\0';
        }
    } else {
        data = realloc(data, data_length + 1);

        if (!data) {
            data = '\0';
        } else {
            data[data_length] = '\0';
        }
    }

    return data;
}

char* ltrim(char* str) {
    if (!str) {
        return '\0';
    }

    if (!*str) {
        return str;
    }

    while (*str != '\0' && isspace(*str)) {
        str++;
    }

    return str;
}

char* rtrim(char* str) {
    if (!str) {
        return '\0';
    }

    if (!*str) {
        return str;
    }

    char* end = str + strlen(str) - 1;

    while (end >= str && isspace(*end)) {
        end--;
    }

    *(end + 1) = '\0';

    return str;
}

char** split_string(char* str) {
    char** splits = NULL;
    char* token = strtok(str, " ");

    int spaces = 0;

    while (token) {
        splits = realloc(splits, sizeof(char*) * ++spaces);

        if (!splits) {
            return splits;
        }

        splits[spaces - 1] = token;

        token = strtok(NULL, " ");
    }

    return splits;
}

int parse_int(char* str) {
    char* endptr;
    int value = strtol(str, &endptr, 10);

    if (endptr == str || *endptr != '\0') {
        exit(EXIT_FAILURE);
    }

    return value;
}

