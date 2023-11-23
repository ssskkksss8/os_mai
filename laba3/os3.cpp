#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/wait.h>
#include <ctype.h>

// Функция для преобразования строки в верхний регистр
void convertToUpper(char* str) {
    while (*str) {
        *str = toupper((unsigned char)*str);
        str++;
    }
}

// Функция для замены пробельных символов на символ "_"
void replaceWhitespace(char* str) {
    while (*str) {
        if (isspace((unsigned char)*str)) {
            *str = '_';
        }
        str++;
    }
}

int main() {
    // Создание shared-отображения файла
    int fd = open("shared_file.txt", O_RDWR | O_CREAT, S_IRUSR | S_IWUSR);
    if (fd == -1) {
        perror("open");
        return 1;
    }
    ftruncate(fd, sizeof(char) * 100);

    // Создание отображаемого области памяти для общения между процессами
    char* shared_memory = mmap(NULL, sizeof(char) * 100, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if (shared_memory == MAP_FAILED) {
        perror("mmap");
        return 1;
    }

    // Создание первого дочернего процесса Child1
    pid_t child1_pid = fork();
    if (child1_pid == -1) {
        perror("fork");
        return 1;
    }
    else if (child1_pid == 0) {
        // Код Child1
        printf("Child1: Started\n");

        // Ожидание данных от родительского процесса
        while (shared_memory[0] == '\0') {}


        // Преобразование строки в верхний регистр
        convertToUpper(shared_memory);

        printf("Child1: Converted to uppercase: %s\n", shared_memory);

        // Завершение Child1
        printf("Child1: Exiting\n");
        exit(0);
    }

    // Создание второго дочернего процесса Child2
    pid_t child2_pid = fork();
    if (child2_pid == -1) {
        perror("fork");
        return 1;
    }
    else if (child2_pid == 0) {
        // Код Child2
        printf("Child2: Started\n");

        // Ожидание данных от родительского процесса
        while (shared_memory[0] == '\0') {}

        // Замена пробельных символов на "_"
        replaceWhitespace(shared_memory);

        printf("Child2: Replaced whitespace: %s\n", shared_memory);

        // Завершение Child2
        printf("Child2: Exiting\n");
        exit(0);
    }

    // Код родительского процесса
    printf("Parent: Started\n");

    // Ввод строки с клавиатуры
    printf("Parent: Enter a string: ");
    fgets(shared_memory, sizeof(char) * 100, stdin);

    // Ожидание завершения дочерних процессов
    waitpid(child1_pid, NULL, 0);
    waitpid(child2_pid, NULL, 0);

    // Вывод результата
    printf("Parent: Result: %s\n", shared_memory);

    // Удаление отображения
    munmap(shared_memory, sizeof(char) * 100);

    printf("Parent: Exiting\n");

    return 0;
}
