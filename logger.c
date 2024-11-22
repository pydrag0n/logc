#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "logger.h"

const char* _log_colors[] = {
    RESET,          // ALL
    GREEN,          // INFO
    RED,            // ERROR
    YELLOW,         // WARN
    BLUE,           // DEBUG
    FATAL_RED       // FATAL
};

const char* _log_level_text[] = {
    "ALL",
    "INFO",
    "ERROR",
    "WARNING",
    "DEBUG",
    "FATAL"
};


void setLevel(Config *logCfg, int const Level)      { logCfg->level = Level; }

void setFilename(Config *logCfg, char *Filename)    { logCfg->filename = Filename; }

void setFormat(Config *logCfg, char *Format)        { logCfg->format = Format; }

void setColor(Config *logCfg, char color)           { logCfg->color = color; }


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


    const int output_size = strlen(msg) + strlen(filename) + line_num + 256 + 50;
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
                sprintf(in_output, _log_level_text[f_type]);
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
    char *final_message = p;

    if (logCfg->level == f_type || logCfg->level == ALL) {
        #ifdef COLOR_ENABLED
        printf("%s%s%s", _log_colors[f_type], p, RESET);
        final_message = p;

        #else
        printf("%s", p);

        #endif
        _writeFile(p, DEFAULT_FILENAME);
    } else {
        _writeFile(p, DEFAULT_FILENAME);
    }

    free(p);
    return 0;
}

// =============================================================================================================
// починить дефолтные значения log_Config / сделать более читабельный код / дать нормальные назвния функциям / пофиксить ошибки / сделать автоматическое расширение буффера  ? цветной режим ?
