#include <iostream>
#include <vector>
#include <thread>
#include <cmath>
#include <chrono>

// Функция для расчета детерминанта матрицы с использованием определения детерминанта
float calculateDeterminant(const std::vector<std::vector<float>>& matrix) {
    int n = matrix.size();
    float det = 0.0;
    
    if (n == 1) {
        return matrix[0][0];
    }
    else if (n == 2) {
        return matrix[0][0] * matrix[1][1] - matrix[0][1] * matrix[1][0];
    }
    else {
        for (int i = 0; i < n; i++) {
            std::vector<std::vector<float>> submatrix(n - 1, std::vector<float>(n - 1, 0.0));
            
            for (int j = 1; j < n; j++) {
                int k = 0;
                for (int l = 0; l < n; l++) {
                    if (l != i) {
                        submatrix[j-1][k] = matrix[j][l];
                        k++;
                    }
                }
            }
            
            float sign = (i % 2 == 0) ? 1.0 : -1.0;
            det += sign * matrix[0][i] * calculateDeterminant(submatrix);
        }
    }
    
    return det;
}

// Функция для создания потоков и расчета детерминанта
void calculateDeterminantInThreads(const std::vector<std::vector<float>>& matrix, int numThreads) {
    int n = matrix.size();
    int numElements = n / numThreads;
    int remainder = n % numThreads;
    
    std::vector<std::thread> threads;
    std::vector<float> determinants(numThreads);
    
    for (int i = 0; i < numThreads; i++) {
        int start = i * numElements;
        int end = start + numElements;
        if (i == numThreads - 1) end += remainder;
        
        threads.emplace_back([=, &matrix, &determinants]() {
            std::vector<std::vector<float>> submatrix(end - start, std::vector<float>(n, 0.0));
            
            for (int j = start; j < end; j++) {
                for (int k = 0; k < n; k++) {
                    submatrix[j - start][k] = matrix[j][k];
                }
            }
            determinants[i] = calculateDeterminant(submatrix);
        });
    }
    
    for (auto& thread : threads) {
        thread.join();
    }
    
    float det = 0.0;
    for (int i = 0; i < numThreads; i++) {
        std::cout << determinants[i] << '\n';
        det += determinants[i];
    }
    
    std::cout << "Determinant: " << det << std::endl;
}

int main(int argc, char *argv[]) {
    int numThreads = atoi(argv[1]);
    std::vector<std::vector<float>> matrix = {
        {1.0, 2.0, 3.0},
        {4.0, 5.0, 6.0},
        {7.0, 8.0, 10.0},
    };
    auto start = std::chrono::high_resolution_clock::now();
    calculateDeterminantInThreads(matrix, numThreads);
    auto end = std::chrono::high_resolution_clock::now();
    std::cout << "Количество используемых потоков: " << numThreads << std::endl;
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    // Вывод времени выполнения
    std::cout << "Время выполнения: " << duration.count()  << " мс" << std::endl;
    return 0;
}
