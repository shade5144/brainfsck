#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ops.h"

int main(int argc, char **argv)
{
    if (argc < 2)
    {
        printf("USAGE: ./readbf bf_file_to_read\n");
        exit(0);
    }

    char *bf_func_names[9] = {"NULL_OP", "INC_DPTR", "DEC_DPTR", "INC_VAL", "DEC_VAL", "WRITE_OUT", "READ_IN", "JMP_IF_ZERO", "JMP_IF_NOT_ZERO"};

    FILE *to_read = fopen(argv[1], "rb");

    size_t read_items;

    int i = 1;
    int j;

    unsigned char opcode;
    int int_buf;
    int num_loops = 0;
    int buf;

    while ((read_items = fread(&buf, sizeof(unsigned int), 1, to_read)) != 0)
    {
        printf("%d ", i);
        for (j = 0; j < num_loops; j++)
        {
            printf("\t");
        }

        opcode = buf & 0xff;
        printf("%s ", bf_func_names[opcode]);

        if (opcode == JMP_IF_ZERO)
        {
            num_loops++;
        }
        if (opcode == JMP_IF_NOT_ZERO)
        {
            num_loops--;
        }

        if (opcode != WRITE_OUT && opcode != READ_IN)
        {
            printf("0x%06x", (buf & 0xffffff00) >> 8);
        }
        printf("\n");

        i++;
    }

    fclose(to_read);
}
