#include <stdio.h>
#include <sys/stat.h>
#include <linux/limits.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <inttypes.h>
#include <stdbool.h>

bool IsCorrectExecutableFile(char *file_name) {
    bool res = true;

    // fopen - ret ptr to file or NULL
    FILE *f = fopen(file_name, "r");
    if (f == NULL) {
        return false;
    }

    // #! - 2 + Path_max
    char begin[PATH_MAX + 2];
    if (fgets(begin, PATH_MAX + 2, f) == NULL) {
        return false;
    }


    // change \n -> \0
    char *feed_line_ptr = memchr(begin, '\n', PATH_MAX + 2);
    if (feed_line_ptr) {
        *feed_line_ptr = '\0';
    }
    

    // check sheebang
    if (begin[0] == '#' && begin[1] == '!') {
        // get stat
        struct stat sheebang_stat;
        if (lstat(begin + 2, &sheebang_stat) != -1) {
            // check that sheebang marked as executable
            if (!(sheebang_stat.st_mode & S_IXUSR)) {
                res = false;
            }
        } else {
            res = false;
        }
        goto Exit;
    }

    // check elf file
    if (!(begin[0] == 0x7f && begin[1] == 'E' && begin[2] == 'L' && begin[3] == 'F')) {
        res = false;
    }

Exit:
    fclose(f);
    return res;
}

int main() {

    char file_name[PATH_MAX]; // PATH_MAX - max size of file name 
    struct stat cur_stat;

    // fgets - ret file_name or NULL
    while (fgets(file_name, PATH_MAX, stdin)) {
        char *feed_line_ptr = memchr(file_name, '\n', sizeof(file_name));

        // change \n -> \0
        if (feed_line_ptr) {
            *feed_line_ptr = '\0';
        }

        // lstat - ret 0 or -1
        if (lstat(file_name, &cur_stat) != -1) {
            // check that file marked as executable
            if (cur_stat.st_mode & S_IXUSR) {
                
                // check that file is correct
                if (!IsCorrectExecutableFile(file_name)) {
                    printf("%s\n", file_name);
                }
            }
        } else {
            return 1;
        }
    }

    return 0;
}