# shear-panel-stress
Library used to calculate stress in skin reinforced stringer assemblies.

This library also features crippling/buckling stress calculations with compressive loads (using the Euler-Johnson equation, the effective column length and slenderness ratio to do so). It was programmed with the intent of compiling it to .dll so that it could be either used with Microsoft Excel (by exporting the functions to it) or with Python (because these calculations executed much faster when ran in C).

# Pre-requisites 
This code should be able to be compiled with most versions of gcc (v12.2.0 was used here). More recent versions of those libraries should be compatible, but were not tested. This code also makes uses the C Standard C99 (previous versions should be compatible but again, weren't tested; some people had better luck compiling with C11 or C17).

gcc will need to be added to PATH.

# Compiling #
## Using cmd ##
* Navigate to the project directory,
* Enter sequentially to compile: 
```
gcc -m64 -O3 -c <filename>.c
gcc -m64 -O3 -shared -o <filename>.dll <filename>.o -Wl,--out-implib,<filename>_lib.a
```
where <filename>.c is the name of the file you want to compile (in this case, it would be: 'solve.c' for <filename>.c and 'solve_lib.a' for <filename>_lib.a).

# Usage #
* You can now run the 'dll_import_test.py' file to test if the .dll was correctly built.
* The function returns the Margin of Safety of the assembly to the applied load. Margins below 0.0 indicate material failure.
