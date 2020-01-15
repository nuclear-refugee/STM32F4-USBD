#include "../header/uart_print.h"

int uart_print(const char *data, ...) {
    va_list arg_ptr;
    va_start(arg_ptr, data);
    int count = 0;

    while (data[count] != '\0') {
        if (data[count] != '%') {  // output char data until %
            uart_put_c(data[count++]);
            continue;
        }
        data++;  // backward a char
        switch (data[count++]) {
            case 'd': {
                int dd, temp = 0;
                char buf[16] = {0};
                dd = va_arg(arg_ptr, int);
                itoa(dd, buf, 10);
                while (buf[temp]) {
                    uart_put_c(buf[temp++]);
                }
                break;
            }
            case 'c': {
                char cc;
                cc = (char)va_arg(arg_ptr, int);
                uart_put_c(cc);
                break;
            };
            case 's': {
                char *ss;
                ss = va_arg(arg_ptr, char *);
                while (*ss) {
                    uart_put_c(*ss++);
                }
                break;
            };
            case 'x': {
                int xx, bytes, temp;
                char hex_char[17] = "0123456789ABCDEF";  // HEX string
                xx = va_arg(arg_ptr, int);
                if (xx > 167772145) {  // 4byte
                    bytes = 4;
                } else if (xx > 65535) {  // 3byte
                    bytes = 3;
                } else if (xx > 255) {  // 2byte
                    bytes = 2;
                } else {  // 1byte
                    bytes = 1;
                }
                uart_put_c('0');
                uart_put_c('x');
                while (bytes--) {
                    temp = (xx >> 8 * (bytes)) & 255;
                    uart_put_c(hex_char[high_bits(temp)]);
                    uart_put_c(hex_char[low_bits(temp)]);
                }
                break;
            };
            default:
                break;
        }
        va_end(arg_ptr);  // free memory
    }
    return 0;  // ok
}