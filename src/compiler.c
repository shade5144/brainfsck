#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include "ops.h"

#define MAX_LOOP 10000

typedef struct
{
    int ls_data[MAX_LOOP];
    int ls_top;
} Loop_Stack;

int main(int argc, char **argv)
{
    if (argc < 3)
    {
        printf("USAGE: ./bfcomp source dest");
        exit(0);
    }

    FILE *bf_source = fopen(argv[1], "rb");

    struct stat file_stat;

    if (fstat(bf_source->_fileno, &file_stat) == -1)
    {
        perror("fstat");
        return 0;
    }

    char *bf_string = malloc(file_stat.st_size);

    if (bf_string == NULL)
    {
        perror("malloc");
        return 0;
    }

    fread(bf_string, 1, file_stat.st_size, bf_source);

    fclose(bf_source);

    printf("FILE SIZE:%lu\n%s\n", file_stat.st_size, bf_string);

    FILE *bf_comp = fopen(argv[2], "wb");

    char c;
    int i = 0;

    int cur_i;
    int *int_buf = malloc(sizeof(int) * file_stat.st_size * 4);
    int instruction_count = 0;

    int nest_level = 0;

    Loop_Stack ls;
    ls.ls_top = -1;

    BF_Opcode cur = NULL_OP;

    while ((c = bf_string[i]) != '\0')
    {
        switch (c)
        {
        case '>': // opcode 1
        {
            cur = INC_DPTR;

            cur_i = i;

            while (bf_string[i] == '>')
            {
                i++;
            }

            break;
        }

        case '<': // opcode 2
        {
            cur = DEC_DPTR;

            cur_i = i;

            while (bf_string[i] == '<')
            {
                i++;
            }

            break;
        }

        case '+': // opcode 3
        {
            cur = INC_VAL;

            cur_i = i;

            while (bf_string[i] == '+')
            {
                i++;
            }

            break;
        }

        case '-': // opcode 4
        {
            cur = DEC_VAL;

            cur_i = i;

            while (bf_string[i] == '-')
            {
                i++;
            }

            break;
        }

        case '.': // opcode 5
        {
            cur = WRITE_OUT;
            break;
        }

        case ',': // opcode 6
        {
            cur = READ_IN;
            break;
        }

        case '[': // opcode 7
        {
            cur = JMP_IF_ZERO;
            nest_level++;
            break;
        }

        case ']': // opcode 8
        {
            cur = JMP_IF_NOT_ZERO;
            nest_level--;

            if (nest_level < 0)
            {
                printf("ERROR: Unmatched ]\n");
            }

            break;
        }

        default:
        {
            cur = NULL_OP;
        }
        }

        if (cur != NULL_OP)
        {
            if (cur < WRITE_OUT)
            {
                int_buf[instruction_count] = ((0xffffff00) & ((i - cur_i) << 8)) | cur;
                i--;
            }
            else
            {
                int_buf[instruction_count] = cur;
            }

            instruction_count++;

            // printf("%08x\n", int_buf[instruction_count]);
        }

        i++;
    }

    BF_Opcode opcode;

    int start_ind;
    ls.ls_top = -1;

    for (i = 0; i < instruction_count; i++)
    {
        opcode = int_buf[i] & 0xff;

        if (opcode == JMP_IF_ZERO)
        {
            ls.ls_top++; // Push to the stack
            ls.ls_data[ls.ls_top] = i;
        }

        if (opcode == JMP_IF_NOT_ZERO)
        {
            start_ind = ls.ls_data[ls.ls_top];

            ls.ls_top--;

            int_buf[i] = (0xffffff00 & (start_ind << 8)) | 8;
            int_buf[start_ind] = (0xffffff00 & (i << 8)) | 7;
        }
    }

    fwrite(int_buf, sizeof(int), instruction_count, bf_comp);

    free(bf_string);
    free(int_buf);
}
