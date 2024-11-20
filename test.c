#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>

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

    case 1: // 20-11-2024 21:54:05  
        sprintf(datetime, "%02d-%02d-%04d",
                timeinfo->tm_mday,
                timeinfo->tm_mon+1,
                timeinfo->tm_year+1900);
        printf("%i\n", 1);
        break;
    case 2: // 11-20-2024 21:50:10
        sprintf(datetime, "%02d-%02d-%04d",
                timeinfo->tm_mon+1,
                timeinfo->tm_mday,
                timeinfo->tm_year+1900);
        printf("%i\n", 2);
        break;
    case 3: // 2024-11-20 21:50:10
        sprintf(datetime, "%04d-%02d-%02d", 
                timeinfo->tm_year+1900,
                timeinfo->tm_mon+1,
                timeinfo->tm_mday);
        printf("%i\n", 3);
        break;
    case 4: // 2024-20-11 21:50:10
        sprintf(datetime, "%04d-%02d-%02d", 
                timeinfo->tm_year+1900,
                timeinfo->tm_mday,
                timeinfo->tm_mon+1);
        printf("%i\n", 4);
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

int main(int argc, char const *argv[])
{
    getFormatTime(4);
    return 0;
}
