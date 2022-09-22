#include "lib/beaver.h"

#define FLAGS "-g -Wall -Werror -Og -pipe"
#define FAST_FLAGS "-O2 -march=native -pipe"

module_t modules[] = {
    { .name = "main", .src = "main.c", .extra_flags = "-lcurl" },
    { .name = "main", .src = "config.c" },
};

void clean(void)
{
    rm("$(find build -type f)");
    rm(" -rf test/*");
    rm("out");
}

uint32_t modules_len = sizeof(modules) / sizeof(*modules);

char* program[] = { "main", NULL };

int main(int argc, char** argv)
{
    auto_update(argv);
    if (argc == 1) {
        compile(program, FLAGS);
    } else if (strcmp(argv[1], "clean") == 0) {
        clean();
    } else if (strcmp(argv[1], "install") == 0) {
        call_or_panic("mv out /usr/bin/comrade");
    } else if (strcmp(argv[1], "fast") == 0) {
        compile(program, FAST_FLAGS);
    }
    return 0;
}
