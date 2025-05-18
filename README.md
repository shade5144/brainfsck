### Description

This is a non-optimizing bytecode interpreter although that may change in the future. Given below are descriptions of implementation/usage.

#### Files

- src/compiler.c: Compiles brainfsck code into a simple bytecode format.

- src/vm.c: VM to run the code.

- src/readbf.c: Displays the bytecode with the symbols given in the enum type `BF_Opcode` in `include/ops.h` converted to strings.

#### Note on the bytecode format

- All instructions are 4 bytes in length with the format of:

    `DATA(if available) for 3 bytes | opcode_number(1-8)`
- The rationale behind storing `opcode_number` in the LSB is that when the VM reads the bytecode, the opcode is read in first, which allows more flexible decoding. For example, if in the future the program is made to use variable length instructions, this would be convenient.

### Running

Any of the files given in `tests` can be compiled and run with the VM. Usage instructions will be provided by the program.

### Acknowledgements

- Specification used: https://github.com/sunjay/brainfuck/blob/master/brainfuck.md

- Tests acquired from: https://github.com/rdebath/Brainfuck