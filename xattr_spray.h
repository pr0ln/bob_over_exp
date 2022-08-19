#ifndef XATTR_SPRAY
#define XATTR_SPRAY
#include <sys/xattr.h>
#include <stdint.h>

#define ATTRIBUTE_NAME_LEN 0x100
#define XATTR_VALUE_LEN 0x1024
#define COMMAND_MAX_LEN 0x100
#define FILENAME_MAX_LEN 0x80

char *generate_tmp_filename(void);
void create_xattr(const char *filename, char *attribute_name, char *xattr_value);
void spray_simple_xattr(char *filename, uint32_t objsize, uint32_t spray_size);
void spray_simple_xattr2(char *filename, uint32_t minsize, uint32_t spray_size);
void free_simple_xattr(char *filename, uint64_t idx);

#endif