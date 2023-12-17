#include <iostream>
#include "system.hpp"
#include "pi.hpp"
#include <string>
#include <dlfcn.h>

using namespace std;

int main(){

    cout << "программа join на этапе линковки" << endl << endl;

    unsigned int switch_ = 1;

    while(1){
        cout << "Введите, какую библиотеку Вы хотите подключить - 1 (системы счисления) или 2 (число pi)" << endl;
        cout << "Если необходимо поменять реализацию программы, введите 0 (по умолчанию реализация - первая из таблицы)" << endl;
        cout << "Для выхода из программы можете ввести exit" << endl;
        int indicator;
        if(!(cin >> indicator)){
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
            
            int A;

            if (switch_ == 1) {
                cout << "(двоичная система) Введите число - A для перевода его в двоичную систему счисления" << endl;;
                cin >> A;

                string rez = systemSecond(A); 
                cout << "(двоичная система) Ответ: " << rez << endl;

            } else {
                cout << "(троичная система) Введите число- А для перевода его в троичную систему счисления" << endl;;
                cin >> A;

                string rez = systemThree(A); 
                cout << "(троичная система) Ответ: " << rez << endl;
            }


        } else if (indicator == 2) {

            int K;
            if (switch_ == 1) {
                cout << "(первое число пи) Введите число K - длина ряда " << endl;;
                cin >> K;

                float pi = piFirst(K); 
                cout << "(первое число пи) Ответ: " << pi << endl;

            } else {
                cout << "(второе число пи) Введите число K - длина ряда " << endl;;
                cin >> K;

                float pi = piSecond(K); 
                cout << "(второе число пи) Ответ: " << pi << endl;
            }


        }

    }
    return 0;
}
