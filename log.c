#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define ALL     0
#define INFO    1
#define ERROR   2
#define WARN    3
#define DEBUG   4



#define LOG_COLOR               0
#define DEFAULT_FILENAME        "_logs.log"
#define DEFAULT_FORMAT          "%(time) [%(level)] %(filename):%(line) %(message)"
#define DEFAULT_DATE_FORMAT     3


typedef struct log
{

    char *filename;
    char *format;
    int level;

} Log_Config;


void setLevel(Log_Config *logCfg, int const Level) { logCfg->level = Level; }

void setFilename(Log_Config *logCfg, char *Filename) { logCfg->filename = Filename; }

void setFormat(Log_Config *logCfg, char *Format) { logCfg->format = Format; }

Log_Config initConf(char *Filename, char *Format, int Level) 
{
    Log_Config logCfg;
    
    logCfg.filename = (Filename != NULL) ? Filename : DEFAULT_FILENAME;
    logCfg.format = (Format != NULL) ? Format : DEFAULT_FORMAT;
    logCfg.level = Level;

    return logCfg;
}
char *getFormatTime(char dateType) 
{
    int date_size = 256;
    char *datetime = malloc(date_size);
    char *_time = malloc(date_size);
    time_t rawtime;
    struct tm *timeinfo;
    time(&rawtime);
    timeinfo = localtime(&rawtime);

    switch (dateType)
    {

    case 1: // 20-11-2024 21:50:10
        sprintf(datetime, "%02d-%02d-%04d",
                timeinfo->tm_mday,
                timeinfo->tm_mon+1,
                timeinfo->tm_year+1900);
        break;
    case 2: // 11-20-2024 21:50:10
        sprintf(datetime, "%02d-%02d-%04d",
                timeinfo->tm_mon+1,
                timeinfo->tm_mday,
                timeinfo->tm_year+1900);
        break;
    case 3: // 2024-11-20 21:50:10
        sprintf(datetime, "%04d-%02d-%02d", 
                timeinfo->tm_year+1900,
                timeinfo->tm_mon+1,
                timeinfo->tm_mday);
        break;
    case 4: // 2024-20-11 21:50:10
        sprintf(datetime, "%04d-%02d-%02d", 
                timeinfo->tm_year+1900,
                timeinfo->tm_mday,
                timeinfo->tm_mon+1);
        break;
    default:
        break;

    }

    sprintf(_time, " %02d:%02d:%02d", 
            timeinfo->tm_hour,
            timeinfo->tm_min,
            timeinfo->tm_sec);

    strcat(datetime, _time);
    return datetime;
}


char *useFormat(char const *msg,
                Log_Config *logCfg,
                int f_type,
                int line_num,
                const char *filename) 
{
    // NULL = error

    const int output_size = 1024;
    char *output = malloc(output_size);
    char *in_output = malloc(output_size);

    if (!output || !in_output) {
        perror("Mem error");
        return NULL;
    }

    output[0] = '\0';
    const char *form = logCfg->format;
    
    for (int i = 0; form[i] != '\0'; i++) {
        if (form[i] == '%' && form[i + 1] == '(') {
            i+=2;
            int j = 0;
            char param[50];
            while (form[i] != ')' && form[i] != '\0' && j < sizeof(param) - 1) {
                param[j++] = form[i++];
            }
            param[j] = '\0';

            if (form[i] != ')') {
                free(in_output);
                free(output);
                printf("%s", "\nFormat error: unclosed parentheses\n");
                return NULL;
            }

            if (strcmp(param, "line") == 0) {
                sprintf(in_output, "%i", line_num);
            } else if (strcmp(param, "filename") == 0) {
                sprintf(in_output, "%s", filename);
            } else if (strcmp(param, "time") == 0) {
                sprintf(in_output, "%s", getFormatTime(DEFAULT_DATE_FORMAT));
            } else if (strcmp(param, "message") == 0) {
                sprintf(in_output, "%s", msg);
            } else if (strcmp(param, "level") == 0) {
                switch (f_type)
                {
                case ERROR:
                    sprintf(in_output, "%s", "ERROR");
                    break;
                case INFO:
                    sprintf(in_output, "%s", "INFO");
                    break;
                case WARN:
                    sprintf(in_output, "%s", "WARNING");
                    break;
                case DEBUG:
                    sprintf(in_output, "%s", "DEBUG");
                    break;
                default:
                    sprintf(in_output, "%s", "ALL");
                    break;
                }
            } else {
                sprintf(in_output, "UNCNOWN_PARAM(%s)", param);
            }

            strcat(output, in_output);

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

int _based_print(const char *msg, Log_Config *logCfg, int lineNum, const char *filename, int f_type) 
{
    char *p = useFormat(msg, logCfg, f_type, lineNum, filename);
    if (!p) { return -1; }

    if (logCfg->level == f_type || logCfg->level == ALL) {
        printf("%s\n", p);
        _writeFile(p, DEFAULT_FILENAME);
    } else { _writeFile(p, DEFAULT_FILENAME); }

    free(p);
    return 0;
}

#define info_print(msg, logCfg)     _based_print(msg, logCfg, __LINE__, __FILE__, INFO)
#define warning_print(msg, logCfg)  _based_print(msg, logCfg, __LINE__, __FILE__, WARN)
#define err_print(msg, logCfg)      _based_print(msg, logCfg, __LINE__, __FILE__, ERROR)
#define debug_print(msg, logCfg)    _based_print(msg, logCfg, __LINE__, __FILE__, DEBUG)

void test_logging() {
    Log_Config config;

    config.level = INFO;
    config.format = "%(time) [%(level)] %(filename):%(line) %(message)";
    config.filename = "logs.log";

    const char* info_message = "This is an info message";
    const char* warning_message = "This is a warning message";
    const char* error_message = "This is an error message";
    const char* debug_message = "This is a debug message";

    info_print(info_message, &config);
    warning_print(warning_message, &config);
    err_print(error_message, &config);
    debug_print(debug_message, &config);

}

int main(int argc, char const *argv[])
{
    test_logging();
    
    // Log_Config logConfig = initConf("_ed_log.log", "%(time) [%(level)] %(filename):%(line) %(message)", ALL);
    // info_print("============info print========", &logConfig);
    return 0;
}
