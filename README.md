# Double ended queue (VecDeque)

## Code structure

All `.h` files are in the `include/` directory, and `.c` files are in `src/`.

After compiling, generated objects go to the `build/` directory, and the final executable goes to `bin/`.

## Compilation and execution

The `makefile` has the following commands:

- `all` (default): compiles everything that was modified since last compilation;
- `clean`: cleans all build objects and binaries;
- `run`: compiles everything and runs the results executable binary.

After compiling, you can use the path `/bin/main` to execute as well.
