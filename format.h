#define ALL     0
#define INFO    1
#define ERROR   2
#define WARN    3
#define DEBUG   4
#define FATAL   5

#define COLOR_ENABLED   1

#define DEFAULT_DATE_FORMAT     3

#define DEFAULT_FORMAT          "%(time) [%(level)] %(filename):%(line) %(message)\n"

#define log_info(msg)     _log_append(msg, __LINE__, __FILE__, INFO)
#define log_warning(msg)  _log_append(msg, __LINE__, __FILE__, WARN)
#define log_error(msg)    _log_append(msg, __LINE__, __FILE__, ERROR)
#define log_debug(msg)    _log_append(msg, __LINE__, __FILE__, DEBUG)
#define log_fatal(msg)    _log_append(msg, __LINE__, __FILE__, FATAL)

#define RESET       "\e[0m"
#define RED         "\e[31m"
#define GREEN       "\e[32m"
#define YELLOW      "\e[33m"
#define BLUE        "\e[34m"
#define MAGENTA     "\e[35m"
#define CYAN        "\e[36m"
#define WHITE       "\e[37m"

#define FATAL_RED   "\e[38;5;196m"
#define GREY        "\e[38;5;240"
