#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define ALL     0
#define INFO    1
#define ERROR   2
#define WARN    3
#define DEBUG   4


#define LOG_COLOR 0;

#define DEFAULT_FILENAME "_ed_log.log"

typedef struct log
{

    char *filename;
    char *format;
    int level;

} Log_Config;


int setLevel(Log_Config *logCfg, int const Level) 
{

    logCfg->level = Level;
    return 0;

}

int setFilename(Log_Config *logCfg, char *Filename) 
{

    logCfg->filename = Filename;
    return 0;

}

int setFormat(Log_Config *logCfg, char *Format) 
{

    logCfg->format = Format;
    return 0;

}

Log_Config initConf(char *Filename, char *Format, int Level) 
{
    Log_Config logCfg;
    
    logCfg.filename = (Filename != NULL) ? Filename : DEFAULT_FILENAME;
    logCfg.format = (Format != NULL) ? Format : NULL;
    logCfg.level = Level;

    return logCfg;
}

char *useFormat(char const *msg, 
                Log_Config *logCfg, 
                int f_type, 
                int line_num,
                const char *filename) 
{

    const int output_size = 1024;
    char *output = malloc(output_size);
    char *in_output = malloc(output_size);

    if (!output || !in_output) {
        perror("Mem error");
        return '\0';
    }

    output[0] = '\0';
    char *form = logCfg->format;
    
    for (int i = 0; form[i] != '\0'; i++) {
        char form_char = form[i];
        if (form_char == '%') {
            form_char = form[i + 1];
            switch (form_char) {
            case 't': {
                time_t rawtime;
                struct tm *timeinfo;
                time(&rawtime);
                timeinfo = localtime(&rawtime);

                sprintf(in_output, "%02d-%02d-%04d %02d:%02d:%02d", 
                        timeinfo->tm_mday,
                        timeinfo->tm_mon + 1,
                        timeinfo->tm_year + 1900,
                        timeinfo->tm_hour,
                        timeinfo->tm_min,
                        timeinfo->tm_sec);
                strcat(output, in_output);
                break;
            }
            case 'l':
                sprintf(in_output, "%i", line_num);
                strcat(output, in_output);
                break;
            case 'm': // msg
                sprintf(in_output, "%s", msg);
                strcat(output, in_output);
                break;
            case 'r': //filename 
                sprintf(in_output, "%s", filename);
                strcat(output, in_output);
                break;
            case 'b':
                if (f_type == ERROR) {
                    sprintf(in_output, "%s", "ERROR");
                } else if (f_type == INFO) {
                    sprintf(in_output, "%s", "INFO");
                } else if (f_type == WARN) {
                    sprintf(in_output, "%s", "WARNING");
                } else if (f_type == DEBUG) {
                    sprintf(in_output, "%s", "DEBUG");
                } else { sprintf(in_output, "%s", "ALL"); }
                strcat(output, in_output);
                break;
            default:
                break;
            }
            i++;
        } else {
            strncat(output, &form[i], 1);
        }
    }
    free(in_output);
    return output;
}
int _writeFile(char *msg, char *filename) {
    FILE *fp = fopen(filename, "w");
    if (fp) {
        fputs(msg, fp);
        fclose(fp);
        return 0;
    }

    return -1;
}



int _err_print(char *msg, Log_Config *logCfg, int line_num, const char *filename) {
    int _level = logCfg->level;
    char *_filename = logCfg->filename;
    char *p = useFormat(msg, logCfg, ERROR, line_num, filename);

    if (!p) {
        return -1;
    }

    if (_level == ERROR || _level == ALL) {
        printf("%s\n", p);
        _writeFile(p, _filename);
    } else {
        _writeFile(p, _filename);
    }

    free(p);
    return 0;
}
#define err_print(msg, logCfg) _err_print(msg, logCfg, __LINE__, __FILE__)


int main(int argc, char const *argv[])
{
    Log_Config logConfig = initConf("_ed_log.log", "%t [%b] %r:%l %m", ALL);
    err_print("An error occurred while processing the request", &logConfig);
    return 0;
}
