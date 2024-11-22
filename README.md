# logging C

![file](file.png)

![console](console.png)

## Config

- level - set a filter for the output of messages
- filename - file for write logs
- format - log format

## Levels

- ALL
- INFO
- ERROR
- WARN
- DEBUG

## Format

``` c
"%(time)\t[%(level)] \t%(filename):%(line)\t%(message)\n"
```

## Functions

- info_print;
- warning_print;
- error_print;
- debug_print;

## Example

```c
#include "logger.c"

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

    info_print(info_message, &config);
    warning_print(warning_message, &config);
    error_print(error_message, &config);
    debug_print(debug_message, &config);

    return 0;
}
```
