#pragma once
#include <locale.h>
#include <stdio.h>
#ifdef _WIN32
#include <io.h>
#include <fcntl.h>
#endif

void init_console(void) {
    setlocale(LC_CTYPE, "");
    setlocale(LC_NUMERIC, "C");

#ifdef _WIN32
    _setmode(_fileno(stdout), _O_U16TEXT);
#endif
}
