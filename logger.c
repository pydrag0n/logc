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

static Config def_cfg = {DEFAULT_FILENAME, DEFAULT_FORMAT, ALL};

void set_filename(char *Filename)   { def_cfg.filename = Filename; }

void set_format(char *Format)       { def_cfg.format = Format; }

void set_level(int const Level)     { def_cfg.level = Level; }

void set_def_cfg(Config *log_cfg)   { def_cfg = *log_cfg; }

Config *get_def_cfg() { return &def_cfg; }

char *get_datetime(char date_type)
{
    int date_size = 256;
    char *datetime = malloc(date_size);
    char *_time = malloc(date_size);
    time_t rawtime;
    struct tm *timeinfo;
    time(&rawtime);
    timeinfo = localtime(&rawtime);

    switch (date_type)
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


char *_Formatter(char const *msg,
                Config *log_cfg,
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
    const char *form = log_cfg->format;

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
                sprintf(in_output, "%s", get_datetime(DEFAULT_DATE_FORMAT));
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

int _write_file(char *msg, const char *filename) {
    FILE *fp = fopen(filename, "a");
    if (fp) {
        fputs(msg, fp);
        fclose(fp);
        return 0;
    }

    return -1;
}

int _log_append(const char *msg, int line_num, const char *filename, int f_type)
{
    Config *log_cfg = get_def_cfg();
    char *p = _Formatter(msg, log_cfg, f_type, line_num, filename);

    if (!p) { return -1; }

    if (log_cfg->level == f_type || log_cfg->level == ALL) {

        #ifdef COLOR_ENABLED
        printf("%s%s%s", _log_colors[f_type], p, RESET);

        #else
        printf("%s", p);

        #endif

        _write_file(p, def_cfg.filename);
    } else {
        _write_file(p, def_cfg.filename);
    }

    free(p);
    return 0;
}
