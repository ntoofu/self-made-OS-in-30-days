#include <stdio.h>
#include "sprintf.h"

int main() {
    char buf[256];
    int len = sprintf(&buf, "test %%d:%d, %%x:%x", -99, 255);
    printf("result: %s\nlength: %d", buf, len);
    return 0;
}

