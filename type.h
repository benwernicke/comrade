#ifndef TYPE_H
#define TYPE_H

#include <stdint.h>

typedef enum {
    REMOTE_FILE,
    FILE_FROM_STR,
    DIR,
    CMD,
    FUNCTION,
} cmd_type_t;

typedef void (*func_f)(void);

typedef struct cmd_t cmd_t;
struct cmd_t {
    cmd_type_t type;

    union {
        struct {
            char* name;
            char* url;
        } remote_file;

        struct {
            char* name;
            char* content;
        } file_from_str;

        struct {
            char* name;
        } dir;

        struct {
            char* cmd;
        } cmd;

        struct {
            func_f fn;
        } function;
    } as;
};

typedef struct cmd_group_t cmd_group_t;
struct cmd_group_t {
    char* name;
    cmd_t* group;
    uint32_t sizeof_group;
};

uint32_t get_sizeof_groups(void);
cmd_group_t* get_groups(void);

#endif
