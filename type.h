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

typedef void (*func_f)(void*);

typedef struct cmd_t cmd_t;
struct cmd_t {
    cmd_type_t type;

    union {
        struct {
            const char* const name;
            const char* const url;
        } remote_file;

        struct {
            const char* const name;
            const char* const content;
        } file_from_str;

        struct {
            const char* const name;
        } dir;

        struct {
            const char* const cmd;
        } cmd;

        struct {
            const func_f fn;
            const void* const arg;
        } function;
    } as;
};

typedef struct cmd_group_t cmd_group_t;
struct cmd_group_t {
    const char* const name;
    const cmd_t* group;
    uint32_t sizeof_group;
};

uint32_t get_sizeof_groups(void);
const cmd_group_t* get_groups(void);

#endif
