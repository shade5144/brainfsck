#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include "ops.h"

typedef struct
{
    int bf_ptr;
    uint8_t bf_mem[30000];
} BF_Machine;

int main(int argc, char **argv)
{
    if (argc < 3)
    {
        printf("USAGE: ./vm -iomode[c/x] bf_file_to_read\n");
        exit(0);
    }

    char *bf_func_names[9] = {"NULL_OP", "INC_DPTR", "DEC_DPTR", "INC_VAL", "DEC_VAL", "WRITE_OUT", "READ_IN", "JMP_IF_ZERO", "JMP_IF_NOT_ZERO"};

    BF_Machine bf_machine;

    bf_machine.bf_ptr = 0;
    memset(bf_machine.bf_mem, 30000, 0);

    FILE *to_read = fopen(argv[2], "rb");

    struct stat file_stat;

    if (fstat(to_read->_fileno, &file_stat) == -1)
    {
        perror("fstat");
        return 0;
    }

    long file_size = file_stat.st_size;

    unsigned int *buf = malloc(file_stat.st_size);

    fread(buf, 4, file_stat.st_size / 4, to_read);

    fclose(to_read);

    uint8_t hex_mode = !strcmp(argv[1], "-x");

    size_t read_bytes;
    int i;

    BF_Opcode opcode;
    uint32_t int_buf;

    for (i = 0; i < file_size / 4; i++)
    {

        opcode = buf[i] & 0xff;
        int_buf = (buf[i] & 0xffffff00) >> 8;

        switch (opcode)
        {
        case INC_DPTR:
        {
            bf_machine.bf_ptr = (bf_machine.bf_ptr + int_buf) % 30000;
            break;
        }

        case DEC_DPTR:
        {
            bf_machine.bf_ptr = (bf_machine.bf_ptr - int_buf) % 30000;
            break;
        }

        case INC_VAL:
        {
            bf_machine.bf_mem[bf_machine.bf_ptr] += int_buf;
            break;
        }

        case DEC_VAL:
        {
            bf_machine.bf_mem[bf_machine.bf_ptr] -= int_buf;
            break;
        }

        case WRITE_OUT:
        {
            if (hex_mode)
            {
                printf("%02x ", bf_machine.bf_mem[bf_machine.bf_ptr]);
            }
            else
            {
                printf("%c", bf_machine.bf_mem[bf_machine.bf_ptr]);
            }

            fflush(stdout);
            break;
        }

        case READ_IN:
        {
            if (hex_mode)
            {
                scanf("%02x", bf_machine.bf_mem + bf_machine.bf_ptr);
            }
            else
            {
                scanf("%c", bf_machine.bf_mem + bf_machine.bf_ptr);
            }

            break;
        }

        case JMP_IF_ZERO:
        {
            if (bf_machine.bf_mem[bf_machine.bf_ptr] == 0)
            {
                i = int_buf - 1;
            }
            break;
        }

        case JMP_IF_NOT_ZERO:
        {
            if (bf_machine.bf_mem[bf_machine.bf_ptr] != 0)
            {
                i = int_buf - 1;
            }
            break;
        }
        }
    }

    printf("\n");
    free(buf);
}
