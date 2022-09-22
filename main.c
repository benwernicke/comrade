#include <stdint.h>
#include <stdio.h>

#include "config.h"
#include "panic.h"
#include <sys/stat.h>

#include <curl/curl.h>
#include <curl/easy.h>

#define INFO(...)                                                              \
    do {                                                                       \
        printf("\033[32m[INFO]\033[39m ");                                     \
        printf(__VA_ARGS__);                                                   \
        putc('\n', stdout);                                                    \
    } while (0)

static size_t write_data(void* ptr, size_t size, size_t nmemb, FILE* stream)
{
    size_t written = fwrite(ptr, size, nmemb, stream);
    return written;
}

static void run_file_from_str(
    const char* const restrict name, const char* const restrict content)
{
    INFO("creating file: %s", name);
    FILE* fh = fopen(name, "w");
    PANIC_IF(!fh, "could not open file '%s': %s", name, strerror(errno));
    fprintf(fh, content);
    fclose(fh);
}

static void run_dir(const char* const name)
{
    INFO("creating directory: %s", name);
    int err = mkdir(name, S_IRWXU);
    PANIC_IF(err, "could not mkdir '%s' %s", name, strerror(errno));
}

static void run_remote_file(
    const char* const restrict name, const char* const restrict url)
{
    INFO("downloading file: %s", name);
    CURL* curl;
    FILE* fh;
    curl = curl_easy_init();
    if (curl != NULL) {
        fh = fopen(name, "wb");
        curl_easy_setopt(curl, CURLOPT_URL, url);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, fh);
        curl_easy_perform(curl);
        curl_easy_cleanup(curl);
        fclose(fh);
    } else {
        PANIC("could not download %s from %s", name, url);
    }
}

static void run_cmd(const char* const cmd)
{
    INFO("running: '%s'", cmd);
    int err = system(cmd);
    PANIC_IF(err, "cmd: '%s' returned error code %d", cmd, err);
}

static void run_group(const cmd_t* restrict iter, uint32_t sizeof_group)
{
    const cmd_t* restrict end = iter + sizeof_group;

    for (; iter != end; ++iter) {
        switch (iter->type) {
        case REMOTE_FILE:
            run_remote_file(
                iter->as.remote_file.name, iter->as.remote_file.url);
            break;
        case FILE_FROM_STR:
            run_file_from_str(
                iter->as.file_from_str.name, iter->as.file_from_str.content);
            break;
        case DIR:
            run_dir(iter->as.dir.name);
            break;
        case CMD:
            run_cmd(iter->as.cmd.cmd);
            break;
        case FUNCTION:
            iter->as.function.fn((void*)iter->as.function.arg);
            break;
        default:
            PANIC("could not find cmd type");
        }
    }
}

int main(int argc, char** argv)
{
    PANIC_IF(argc != 2, "comrade requires 2 arguments");

    uint32_t size = get_sizeof_groups();
    const cmd_group_t* restrict iter = get_groups();
    const cmd_group_t* restrict end = iter + size;

    for (; iter != end; ++iter) {
        if (strcmp(iter->name, argv[1]) == 0) {
            run_group(iter->group, iter->sizeof_group);
            break;
        }
    }

    PANIC_IF(iter == end, "could not find group: '%s'", argv[1]);

    return 0;
}
