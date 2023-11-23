#include <unistd.h>
#include <iostream>
#include <string>
#include <cctype>

int main() {
    int fd1[2], fd2[2];   // pipe1 - from parent to child1, pipe2 - from child2 to parent
    if (pipe(fd1) == -1) { //при успехе возвращает 0
        perror("error pipe!\n");
        exit(-1);
    }
    if (pipe(fd2) == -1) {
        perror("error pipe!\n");
        exit(-1);
    }

    pid_t child1 = fork(); //возвращает id процесса
    if (child1 == -1) {
        perror("Fork error!\n");
        return -1;
    }
    if (child1 == 0) {
        close(fd1[1]);
        close(fd2[0]);
        if(close(fd1[1]) == -1){ //в случае успеха возвращает 0
            perror('close error\n');
        };
        if(close(fd2[0]) == -1){
            perror('close error\n');
        };

        int fd3[2];
        if (pipe(fd3) == -1) {
            perror("error pipe!\n");
            exit(-1);
        }

        pid_t child2 = fork();
        if (child2 == -1) {
            perror("Fork error!\n");
            return -1;
        }

        if (child2 == 0) { // child2
            if(close(fd3[0]) == -1){ //в случае успеха возвращает 0
                perror('close error\n');
            };
            if(close(fd1[1]) == -1){
                perror('close error\n');
            };

            if (dup2(fd1[0], STDIN_FILENO) == -1) { //возвращает новый дескриптор файла, который полностью описывает (т.е. дублирует) состояние файла
                perror("dup2 error!\n");
                return -1;
            }
            if (dup2(fd3[1], STDOUT_FILENO) == -1) {
                perror("dup2 error!\n");
                return -1;
            }

            if (execl("../build/child2", "../build/child2", NULL) == -1){
                perror("execl error\n");
                //возвращает значение только при ошибке, предоставляет программе список указателей на параметры доступные программе
            };

            if(close(fd3[1]) == -1){ //в случае успеха возвращает 0
                perror('close error\n');
            };
            if(close(fd1[0]) == -1){
                perror('close error\n');
            };
        } else { 
            if(close(fd1[0]) == -1){ //в случае успеха возвращает 0
                perror('close error\n');
            };
            if(close(fd2[1]) == -1){
                perror('close error\n');
            };   // child1

            if (dup2(fd3[0], STDIN_FILENO) == -1) {
                perror("dup2 error!\n");
                return -1;
            }
            if (dup2(fd2[1], STDOUT_FILENO) == -1) {
                perror("dup2 error!\n");
                return -1;
            }

            if (execl("../build/child1", "../build/child1", NULL) == -1){
                perror('execl error\n');
            };
            if(close(fd1[0]) == -1){ //в случае успеха возвращает 0
                perror('close error\n');
            };
            if(close(fd2[1]) == -1){
                perror('close error\n');
            };
        }
    } else { 
           // parent
        if(close(fd1[0]) == -1){ //в случае успеха возвращает 0
            perror('close error\n');
        };
        if(close(fd2[1]) == -1){
            perror('close error\n');
        };
        char c = getchar();
        while (c != EOF) {
            if (write(fd1[1], &c, sizeof(c)) == -1){ //возвращает объект
                perror('write error\n');
            };
            if (read(fd2[0], &c, sizeof(c)) == -1){ // возвращает количество фактически считанных байтов, 
                perror('read error\n');
            };

            putchar(c);

            c = getchar();
        }
    if(close(fd1[1]) == -1){ //в случае успеха возвращает 0
        perror('close error\n');
    };
    if(close(fd2[0]) == -1){
        perror('close error\n');
    };
    }
    
    return 0;
}
