
#include <iostream>
#include <fstream>
#include <iomanip>
#include <limits>
#include <ctime>
#include <thread>
#include <vector>
#include <mutex>
#include <string>
using namespace std;
std::mutex g_lock;

/// <summary>
/// метод, подсчитывающий макс длину возрастающей последовательности в пределах значений
/// </summary>
/// <param name="n">кол-во элементов массива</param>
/// <param name="a">массив</param>
/// <param name="start">верхний предел</param>
/// <param name="finish">нижний предел</param>
/// <param name="Max">макисмальное кол-во эл-в в возрастающей последовательности</param>
/// <param name="position">позиция эл-та от которого начинается отсчет</param>
/// <param name="potok">номер потока</param>
void lap(int n, double* a, int start, int finish, double& Max,double& position,int potok) {//к - начало , l - конец  int& MaxSize,int& k,int& l,int start,int finish
    //g_lock.lock();
    int* d;//массив эл-ов из длин последовательностей начиная с iого эл-та
    d = new int[n] ; // константа MAXN равна наибольшему возможному значению n
    for (int i = start; i < finish; ++i) {
        d[i] = 1;
        int k = n;
        for (int j = i + 1; j < k; ++j) {
            if (a[j] > a[j - 1]) {
                d[i]++;//если след эл-т больше, то длина последовательности увеличивается
            }
            else {
                k = 0;//чтобы завершить цикл предварительно
            }
        }
    }
    double MaxSize = 0;
    int pos = -1;
    for (int i = 0; i < n; i++) {
        if (d[i] > MaxSize) {
            MaxSize = d[i];
            pos = i;
        }
    }
    Max = MaxSize;
    position = pos;
    //cout << "potok№ " << potok << " MaxSize = " << MaxSize << " pos = " << pos << endl;
    //g_lock.unlock();
}

int main(int argc, char* argv[]) {
    string pathFrom = argv[1];
    string n;
    std::string line;
    string threadNum;
    std::ifstream in(pathFrom); // окрываем файл для чтения
    string res;
    double* A = new double[0];
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
    if (n1 < 1000||thrNum<1) {
        std::cout << "Incorrect size of vector = " << n1 <<" or incorrect threadNumber = " << thrNum<< "\n";
        return 1;
    }
    if (index!= n1) {
        std::cout << "Incorrect size of vector = " << n1 << "\n";
        return 1;
    }
    std::thread* thr = new thread[thrNum];
    double* maxOfThreads = new double[thrNum];
    double* positions = new double[thrNum];
    clock_t start_time = clock(); 

    for (int i = 0; i < thrNum; i++) {
        int k = n1 / thrNum;
        int start = i * k;
        int finish = 0;
        if (i == thrNum - 1) {
            finish = n1 - 1;
        }
        else {
            finish = (i + 1) * k;
        }
        thr[i] = std::thread{ lap, n1, A,start,finish , ref(maxOfThreads[i]),ref(positions[i]) ,i};
    }

    double MaxSize = 0.0; 
    int pos;
    vector<int> MaxSizes;
    vector<int> poses;
    for (int i = 0; i < thrNum; i++) {
        thr[i].join();
        MaxSizes.push_back(maxOfThreads[i]);
        poses.push_back(positions[i]);
    }
    delete[] thr;
    for (int i = 0; i < MaxSizes.size(); i++)
    {
        if (MaxSizes[i] > MaxSize) {
            MaxSize = MaxSizes[i];
            pos = poses[i];
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