int int2chars(char *buf, int val, int base) {
    char const * buf_begin = buf;
    char digit_chars[16];
    char *p_digit_chars = digit_chars;
    if(val == 0) {
        *buf++ = '0';
    } else if(val < 0) {
        *buf++ = '-';
        val = -val;
    }
    while(val > 0) {
        char digit = val % base;
        *p_digit_chars++ = digit < 10 ? '0' + digit : 'a' + digit - 10;
        val /= base;
    }
    while(--p_digit_chars >= digit_chars) {
        *buf++ = *p_digit_chars;
    }
    const int length = buf - buf_begin;
    return length;
}


int sprintf(char *buf, const char *fmt, ...) {
    char const *buf_begin = buf;
    char c;
    char escaped = 0;
    int *va = (int*)(&fmt + 1);
    while(c = *fmt) {
        int intval;
        if(escaped) {
            switch(c) {
                case 'd':
                    intval = *va++;
                    buf += int2chars(buf, intval, 10);
                    break;
                case 'x':
                    intval = *va++;
                    buf += int2chars(buf, intval, 16);
                    break;
                case '%':
                    *buf++ = '%';
                    break;
                default:
                    return -1;  // Parse Error
            }
            escaped = 0;
        } else {
            if(c == '%') {
                escaped = 1;
            } else {
                *buf++ = *fmt;
            }
        }
        ++fmt;
    }
    *buf = '\0';
    const int length = buf - buf_begin;
    return length;
}

