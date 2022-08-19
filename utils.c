#define _GNU_SOURCE
#include <sched.h>
#include "utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>

void pin_cpu(int cpu)
{
        cpu_set_t set;
        CPU_ZERO(&set);
        CPU_SET(cpu, &set);
        if (sched_setaffinity(0, sizeof(cpu_set_t), &set)) {
                printf("error\n");
                exit(-1);
        }
}


void write_file(const char *filename, char *text) {

    int fd = open(filename, O_RDWR);

    write(fd, text, strlen(text));
    close(fd);
}

void new_ns(void) {
    uid_t uid = getuid();
    gid_t gid = getgid();
    char buffer[0x100];

    if (unshare(CLONE_NEWUSER | CLONE_NEWNS))
    {
        perror("unshare(CLONE_NEWUSER | CLONE_NEWNS)");
        exit(-1);
    }
    if (unshare(CLONE_NEWNET))
    {
        perror("unshare(CLONE_NEWNET)");
        exit(-1);
    }
    write_file("/proc/self/setgroups", "deny");

    snprintf(buffer, sizeof(buffer), "0 %d 1", uid);
    write_file("/proc/self/uid_map", buffer);
    snprintf(buffer, sizeof(buffer), "0 %d 1", gid);
    write_file("/proc/self/gid_map", buffer);
}

void copy_excutable(char *src, char * dest)
{
    char buf[1024];
    sprintf(buf, "cp %s %s", src, dest);
    system(buf);
    sprintf(buf, "chmod 755 %s", dest);
    system(buf);

}
