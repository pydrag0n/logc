#include "format.h"
#define DEFAULT_FILENAME        "out/logs.log"

typedef struct log {
    char    *filename;
    char    *format;
    int     level;
} Config;


int _log_append(const char *msg, int line_num, const char *filename, int f_type);

void set_def_cfg(Config *log_cfg);
void set_filename(char *Filename);
void set_format(char *Format);
void set_level(int const Level);

char *get_datetime(char date_type);
Config *get_def_cfg();
