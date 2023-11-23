#include <iostream>
#include <unistd.h>

int main() {
    char c;
    while (read(STDIN_FILENO, &c, sizeof(c)) != -1) {
        if (c == ' ') {
            c = '_';
        }
        write(STDOUT_FILENO, &c, sizeof(c));
    }
    close(STDIN_FILENO);
    close(STDOUT_FILENO);

    return 0;
}