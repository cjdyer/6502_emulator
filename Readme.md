# Byte Code Interpreter

This project is a simple byte code interpreter for a set of custom instructions. It reads assembly-like instructions from a file, interprets them into opcodes, and then executes them using a virtual processor.

## How To Build

1. Clone the repo: 
```bash
git clone https://github.com/cjdyer/6502_emulator.git
cd 6502_emulator
```

2. Compile the code:
```bash
mkdir build
cd build
cmake ..
make
```

3. Run:
```bash
./emulator
```

## Usage

- Make sure the assembly file (`program.asm` by default) is present in the directory above the interpreter.
- Run the emulator.