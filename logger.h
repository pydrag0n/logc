#include "format.h"
#define DEFAULT_FILENAME        "out/logs.log"

typedef struct log {
    char    *filename;
    char    *format;
    int     level;
    char    color;
} Config;

int _based_print(const char *msg, Config *logCfg, int lineNum, const char *filename, int f_type);
