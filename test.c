#include "logger.h"

int main()
{
    Config config;

    config.level = ALL;
    config.format = "%(time)  [%(level)] \t%(filename):%(line)\t%(message)\n";
    config.filename = "logs.log";

    const char* info_message = "This is an info message";
    const char* warning_message = "This is a warning message";
    const char* error_message = "This is an error message";
    const char* debug_message = "This is a debug message";
    const char* fatal_message = "This is a fatal message";

    log_info(info_message, &config);
    log_warning(warning_message, &config);
    log_error(error_message, &config);
    log_debug(debug_message, &config);
    log_fatal(fatal_message, &config);

    return 0;
}
