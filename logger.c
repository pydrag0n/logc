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
#define DEFAULT_DATE_FORMAT     3

#define DEFAULT_FILENAME        "_logs.log"
#define DEFAULT_FORMAT          "%(time) [%(level)] %(filename):%(line) %(message)"

#define info_print(msg, logCfg)     _based_print(msg, logCfg, __LINE__, __FILE__, INFO)
#define warning_print(msg, logCfg)  _based_print(msg, logCfg, __LINE__, __FILE__, WARN)
#define error_print(msg, logCfg)    _based_print(msg, logCfg, __LINE__, __FILE__, ERROR)
#define debug_print(msg, logCfg)    _based_print(msg, logCfg, __LINE__, __FILE__, DEBUG)


#ifdef LOG_COLOR

#define RESET   "\033[0m"
#define RED     "\033[31m"
#define GREEN   "\033[32m"
#define YELLOW  "\033[33m"
#define BLUE    "\033[34m"
#define MAGENTA "\033[35m"
#define CYAN    "\033[36m"
#define WHITE   "\033[37m"
#define GREY    "\e[38;5;240"

#define INFO_TEXT   _colorText(GREEN,   "INFO")
#define ERROR_TEXT  _colorText(RED,     "ERROR")
#define WARN_TEXT   _colorText(RED,     "WARNING")
#define DEBUG_TEXT  _colorText(CYAN,    "DEBUG")
#define ALL_TEXT    _colorText(BLUE,    "ALL")

#else

#define INFO_TEXT   "INFO"
#define ERROR_TEXT  "ERROR"
#define WARN_TEXT   "WARNING"
#define DEBUG_TEXT  "DEBUG"
#define ALL_TEXT  "ALL"

#define UNCNOWN     "UNCNOWN_PARAM"

#endif

typedef struct log
{

    char *filename;
    char *format;
    int level;

} Config;


void setLevel(Config *logCfg, int const Level) { logCfg->level = Level; }

void setFilename(Config *logCfg, char *Filename) { logCfg->filename = Filename; }

void setFormat(Config *logCfg, char *Format) { logCfg->format = Format; }


char *_colorText(char *color, char *text) {
    if (color == NULL || text == NULL) {
        return NULL;
    }

    size_t length = strlen(color) + strlen(text) + strlen(RESET) + 1;
    char *tx = malloc(length);

    if (tx == NULL) {
        return NULL;
    }

    sprintf(tx, "%s%s%s", color, text, RESET);
    return tx;
}


char *getDatetime(char dateType) 
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


char *Formatter(char const *msg,
                Config *logCfg,
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
                sprintf(in_output, "%s", getDatetime(DEFAULT_DATE_FORMAT));
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
    FILE *fp = fopen(filename, "a");
    if (fp) {
        fputs(msg, fp);
        fclose(fp);
        return 0;
    }

    return -1;
}

int _based_print(const char *msg, Config *logCfg, int lineNum, const char *filename, int f_type) 
{
    char *p = Formatter(msg, logCfg, f_type, lineNum, filename);
    if (!p) { return -1; }

    if (logCfg->level == f_type || logCfg->level == ALL) {
        printf("%s", p);
        _writeFile(p, DEFAULT_FILENAME);
    } else { 
        _writeFile(p, DEFAULT_FILENAME); 
    }

    free(p);
    return 0;
}



void test_logging() {
    Config config;

    config.level = ALL;
    config.format = "%(time)  [%(level)]\t %(filename):%(line)   %(message)";
    config.filename = "logs.log";

    const char* info_message = "This is an info message";
    const char* warning_message = "This is a warning message";
    const char* error_message = "This is an error message";
    const char* debug_message = "This is a debug message";

    info_print(info_message, &config);
    warning_print(warning_message, &config);
    error_print(error_message, &config);
    debug_print(debug_message, &config);

}
// =============================================================================================================
// починить дефолтные значения log_Config / сделать более читабельный код / дать нормальные назвния функциям / пофиксить ошибки / сделать автоматическое расширение буффера  ? цветной режим ?
 
