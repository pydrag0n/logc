#include <stdio.h>
#include "logger.h"

int main()
{
     Config newConfig = { "new_log.log", "%(level): %(message)\n", ALL };

    const char* info_message = "This is an info message";
    const char* warning_message = "This is a warning message";
    const char* error_message = "This is an error message";
    const char* debug_message = "This is a debug message";
    const char* fatal_message = "This is a fatal message";


    log_info(info_message);
    log_warning(warning_message);
    log_error(error_message);
    log_debug(debug_message);
    log_fatal(fatal_message);

    set_def_cfg(&newConfig);

    log_info(info_message);
    log_warning(warning_message);
    log_error(error_message);
    log_debug(debug_message);
    log_fatal(fatal_message);

    return 0;
}
