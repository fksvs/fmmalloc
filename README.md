# fmmalloc

fmmalloc is a custom implementation of the dynamic memory allocation functions. This implementation includes the functions `malloc()`, `free()`, `calloc()`, `realloc()` and `reallocarray()`.

## Installation

Clone the git repository from [GitHub][] or [GitLab][]:

```console
git clone https://github.com/fksvs/fmmalloc
git clone https://gitlab.com/fksvs/fmmalloc
```

Build the source code and produce shared object, this will also compile the test programs:

```console
make
```

## Usage

To use fmmalloc in your programs, you need to link the `malloc.so` shared library to your target program. This can be done using the `LD_PRELOAD` environment variable.

### Running Test Programs

You can run the provided test programs to verify the functionality of the custom memory allocation functions:

```console
LD_PRELOAD=./malloc.so ./basic_func
LD_PRELOAD=./malloc.so ./error_handling
LD_PRELOAD=./malloc.so ./stress_test
```

### Using with Other Programs

You can also use fmmalloc with existing programs to replace the default memory allocation functions with fmmalloc. For example, to use fmmalloc with the `ls` command, you can run:

```console
LD_PRELOAD=./malloc.so /bin/ls
```

or use with your own program:

```console
LD_PRELOAD=./malloc.so ./<your program>
```

## License

This project is free software; you can redistribute it and/or modify it under the terms of the GPLv3 license. See [LICENSE][] for details.

[GitHub]: https://github.com/fksvs/fmmalloc
[GitLab]: https://gitlab.com/fksvs/fmmalloc
[LICENSE]: https://www.gnu.org/licenses/gpl-3.0.en.html
