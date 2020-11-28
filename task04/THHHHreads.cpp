
#include <iostream>
#include <fstream>
#include <iomanip>
#include <limits>
#include <ctime>
#include <thread>
#include <vector>
#include <mutex>
#include <string>
#include <omp.h>
using namespace std;

int main(int argc, char* argv[]) {
    string pathFrom = argv[1];
    string n;
    std::string line;
    string threadNum;
    std::ifstream in(pathFrom); // окрываем файл для чтения
    string res;
    double* A = nullptr;
    int index = 0;
    int n1 = 0;
    int thrNum = 0;
    if (in.is_open()) {
        getline(in,threadNum);
        getline(in, n);
        
        try {
            thrNum = stoi(threadNum);
            n1 = stoi(n);
            A = new double[n1];
            while (getline(in, line)) {//считываем массив из файла
                if (index < n1) {
                    A[index] = stoi(line);
                    index++;
                }
            }
        }
        catch (int a) {
            std::cout << "We caught an int exception with value: " << a << '\n';
            return 1;
        }
        catch (const std::invalid_argument& ia) {
            std::cerr << "Incorrect file " << '\n';
            return 1;
        }
    }
    if (n1 < 1000 || thrNum < 1) {
        std::cout << "Incorrect size of vector = " << n1 << " or incorrect threadNumber = " << thrNum << "\n";
        return 1;
    }
    if (index != n1) {
        std::cout << "Incorrect size of vector = " << n1 << "\n";
        return 1;
    }
    clock_t start_time = clock(); 

    int MaxSize ; 
    int pos;
    vector<int> MaxSizes;
    vector<int> poses;

#pragma omp parallel shared(A,MaxSize,pos,MaxSizes,poses,thrNum)
    {
#pragma omp for 
        for (int i = 0; i < thrNum; i++)
        {
            int k = n1 / thrNum;
            int start = i * k;
            int finish = 0;
            if (i == thrNum - 1) {
                finish = n1 - 1;
            }
            else {
                finish = (i + 1) * k;
            }
            int d = 1;

            int* d1;
            d1 = new int[n1];

            for (int j = start; j < finish; ++j) {
                d1[j] = 1;
                int g = n1;
                for (int l = j + 1; l < g; ++l) {
                    if (A[l] > A[l - 1]) {
                        d1[j]++;//если след эл-т больше, то длина последовательности увеличивается
                    }
                    else {
                        g = 0;//чтобы завершить цикл предварительно
                    }
                }
                if (d1[j] > MaxSize) {
                    MaxSize = d1[j];
                    pos = j;
                }
            }
        }
    }

    clock_t end_time = clock();


    std::cout << "MaxSize =  " << MaxSize << "\n";
    std::cout << "Position of i =  " << pos << "\n";
    std::cout << "Position of j =  " << pos+MaxSize-1 << "\n";
    std::cout << "WorkingTime = " << end_time - start_time << "\n";
    delete[] A;
    return 0;
}