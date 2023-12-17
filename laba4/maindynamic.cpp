#include <iostream>
#include <dlfcn.h>
#include <string>

using namespace std;

int main(){

    cout << "динамическое соединение во время выполнения" << endl << endl;

    // открытие динамических библиотек
    void* lib1 = dlopen("libSystem.so", RTLD_LAZY);//отложенная загрузка до момента использования
    if (lib1 == nullptr) {
        cerr << "Библиотека System не открылась( " << dlerror() ;
        return 1;
    }

    void* lib2 = dlopen("libPi.so", RTLD_LAZY);
    if (lib2 == nullptr) {
        cerr << "Библиотека Pi не открылась( " << dlerror() ;
        return 2;
    }

    unsigned int switch_ = 1;

    while(1){
        cout << "Введите подключаемую библиотеку- 1 (системы счисления) или 2 (число pi)" << endl;
        cout << "Если необходимо поменять реализацию программы, введите 0 (по умолчанию реализация - первая из таблицы)" << endl;
        cout << "Для выхода из программы можете ввести exit" << endl;
        unsigned int indicator;
        if(!(cin >> indicator)){
            dlclose(lib1);
            dlclose(lib2);
            cout << "корректное завершение" << endl;
            break;

        } else if (indicator == 0){
            if (switch_ == 1){
                switch_ = 2;
            } else {
                switch_ = 1;
            }

            cout << "Реализация изменена на " << switch_ << endl;

        } else if (indicator == 1) {
            
            long A;
            string (*f1)(long); //функция первой библиотеки

            if (switch_ == 1) {
                f1 = (string (*)(long)) dlsym(lib1, "systemSecond"); //с помощью dlsym получаем адрес функции
            } else {
                f1 = (string (*)(long)) dlsym(lib1, "systemThree");
            }

            if (dlerror()) {
                cerr << "Ошибка при вызове dlsym() в первой библиотеке 1 " <<  dlerror();

                if (!(dlclose(lib1) == 0)){
                cerr << "Библиотека system не закрылась( ";
                return 3;
                }
                if (!(dlclose(lib2) == 0)){
                    cerr << "Библиотека Pi не закрылась( ";
                    return 4;
                }

                return 11;
            }

            cout << "Введите число для его перевода в другую систему счисления" << endl;;
            cin >> A;

            string rez = (*f1)(A); 
            cout << "Ответ: " << rez << endl;

        } else if (indicator == 2) {

            int K;
            float (*f2)(int); //функция второй библиотеки

            if (switch_ == 1) {
                f2 = (float (*)(int)) dlsym(lib2, "piFirst");
            } else {
                f2 = (float (*)(int)) dlsym(lib2, "piSecond");
            }

            if (dlerror()) {
                cerr << "Ошибка при вызове dlsym() во второй библиотеке 2 " <<  dlerror() ;

                if (!(dlclose(lib1) == 0)){
                cerr << "Библиотека system не закрылась( ";
                return 3;
                }
                if (!(dlclose(lib2) == 0)){
                    cerr << "Библиотека Pi не закрылась( ";
                    return 4;
                }

                return 22;
            }

            cout << "Введите число K - длина ряда " << endl;;
            cin >> K;

            float pi = (*f2)(K); 
            cout << "Ответ: " << pi << endl;

        }

    }


    return 0;
}