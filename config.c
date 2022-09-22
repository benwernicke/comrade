#include "config.h"

static char def_main[] = "#include <stdio.h>\n"
                         "#include <stdint.h>\n"
                         "\n"
                         "int main(void)\n"
                         "{\n"
                         "   printf(\"Hello World!\\n\");\n"
                         "   return 0;\n"
                         "}\n";

static char def_beaver[]
    = "#include \"lib/beaver.h\"\n"
      "\n"
      "#define FLAGS \"-g -Wall -Werror -Og\"\n"
      "\n"
      "module_t modules[] = {\n"
      "    { .name = \"main\", .src = \"main.c\" },\n"
      "};\n"
      "\n"
      "uint32_t modules_len = sizeof(modules) / sizeof(*modules);\n"
      "\n"
      "char* program[] = { \"main\", NULL };\n"
      "\n"
      "int main(int argc, char** argv)\n"
      "{\n"
      "    auto_update(argv);\n"
      "    if (argc == 1) {\n"
      "        compile(program, FLAGS);\n"
      "    } else if (strcmp(argv[1], \"clean\") == 0) {\n"
      "        rm(\"$(find build -type f)\");\n"
      "        rm(\"out\");\n"
      "    }\n"
      "    return 0;\n"
      "}\n";

static cmd_t init_group[] = {
    { .type = DIR, .as.dir = { .name = "lib" } },
    { .type = FILE_FROM_STR,
        .as.file_from_str = { .name = "main.c", .content = def_main } },
    { .type = FILE_FROM_STR,
        .as.file_from_str = { .name = "beaver.c", .content = def_beaver } },
    { .type = REMOTE_FILE,
        .as.remote_file = { .name = "lib/beaver.h",
            .url = "https://raw.githubusercontent.com/The-C-Nature-Reserve/"
                   "beaver/main/beaver.h" } },
    { .type = CMD, .as.cmd = { .cmd = "gcc -o beaver beaver.c" } },
};
static const uint32_t sizeof_init_group
    = sizeof(init_group) / sizeof(*init_group);

static cmd_group_t groups[] = {
    { .name = "init", .group = init_group, .sizeof_group = sizeof_init_group },
};
static const uint32_t sizeof_groups = sizeof(groups) / sizeof(*groups);

uint32_t get_sizeof_groups(void) { return sizeof_groups; }
cmd_group_t* get_groups(void) { return groups; }
