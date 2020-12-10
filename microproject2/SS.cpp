#include <pthread.h>
#include <semaphore.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string>
#include <vector>
#include <iostream>

const int bufSize = 2;
int buf[bufSize]; //буфер
int front = 0; //индекс для чтения из буфера
int rear = 0; //индекс для записи в буфер
int count = 0; //количество занятых ячеек буфера
//unsigned int seed = 101; // инициализатор генератора случайных чисел
std::vector<std::string> res;

pthread_mutex_t mutex; // мьютекс для условных переменных

pthread_cond_t not_full;

pthread_cond_t not_empty;

//стартовая функция потоков – производителей (писателей)
void* Doctor(void* param) {
    int pNum = *((int*)param);
    int data, i;
    while (1) {
        //создаем элемент для буфера
        data = rand() % 3 + 1;
        pthread_mutex_lock(&mutex); //защита операции записи

        while (count == bufSize) {
            pthread_cond_wait(&not_full, &mutex);
        }

        //запись в общий буфер
        buf[rear] = data;
        rear = (rear + 1) % bufSize;
        count++; //появилась занятая ячейка

        //конец критической секции
        pthread_mutex_unlock(&mutex);
        //разбудить потоки-читатели после добавления элемента в буфер
        pthread_cond_broadcast(&not_empty);
        sleep(rand() % 2 + 0.02);
    }
    return NULL;
}

//стартовая функция потоков – потребителей (читателей)
void* Client(void* param) {
    int cNum = *((int*)param);
    int result;
    //извлечь элемент из буфера
    pthread_mutex_lock(&mutex); //защита операции чтения

    //заснуть, если количество занятых ячеек равно нулю
    while (count == 0) {
        pthread_cond_wait(&not_empty, &mutex);
    }

    //изъятие из общего буфера – начало критической секции
    result = buf[front];
    front = (front + 1) % bufSize; //критическая секция
    count--; //занятая ячейка стала свободной
    res.push_back(std::to_string(cNum) + " "+std::to_string(result)+" "+std::to_string(front));
    //конец критической секции
    pthread_mutex_unlock(&mutex);
    //разбудить потоки-писатели после получения элемента из буфера
    pthread_cond_broadcast(&not_full);
    return NULL;
}

int main() {
    int n;
    std::string howMuch;
    printf("Input how much clients will be in hospital today\n");
    std::cin >> howMuch;
    size_t found = howMuch.find_first_not_of("1234567890");
    while (found != std::string::npos || stoi(howMuch)==0||stoi(howMuch)>1000) {
        std::cout << "Something wrong, try again "  << std::endl;
        std::cin >> howMuch;
        found = howMuch.find_first_not_of("1234567890");
    }
    n = stoi(howMuch);
    
    int i;
    //инициализация мутексов и семафоров
    pthread_mutex_init(&mutex, NULL);
    pthread_cond_init(&not_full, NULL);
    pthread_cond_init(&not_empty, NULL);

    //запуск врачей
    pthread_t threadP[2];
    int producers[2];
    for (i = 0; i < 2; i++) {
        producers[i] = i + 1;
        pthread_create(&threadP[i], NULL, Doctor, (void*)(producers + i));
    }
    //запуск посетителей
    pthread_t* threadC = new pthread_t[n];
    int* consumers = new int[n];
    for (i = 0; i < n; i++) {
        consumers[i] = i + 1;
        pthread_create(&threadC[i], NULL, Client, (void*)(consumers + i));
    }
    for (i = 0; i < n; i++) {
        consumers[i] = i + 1;
        pthread_join(threadC[i], NULL);
    }

    std::vector<int> clients;
    std::vector<std::string> toWhat;
    std::vector<int> docNum;
    for(auto client: res)
    {
        std::string words[3];
        int index = -1;
        for (int i = 0; i < client.size(); i++) {
            if (client[i] == ' ') {
                words[++index] = client.substr(0, i);
                client = client.substr(i + 1, res.size() - i - 1);
                i = 0;
            }
        }
        if (words[2] == "") {
            words[2] = client;
        }
        clients.push_back(stoi(words[0]));
        toWhat.push_back(words[1]);
        docNum.push_back(stoi(words[2]));
    }

    int temp1; // временная переменная для обмена элементов местами
    std::string temp2;
    int temp3;
    // Сортировка массива пузырьком
    for (int i = 0; i < res.size()-1; i++) {
        for (int j = 0; j < res.size() - i - 1; j++) {
            if (clients[j] > clients[j + 1]) {
                // меняем элементы местами
                temp1 = clients[j];
                clients[j] = clients[j + 1];
                clients[j + 1] = temp1;

                temp2 = toWhat[j];
                toWhat[j] = toWhat[j + 1];
                toWhat[j + 1] = temp2;

                temp3 = docNum[j];
                docNum[j] = docNum[j + 1];
                docNum[j + 1] = temp3;
            }
        }
    }
    for (int i = 0; i < res.size(); i++) {
        std::string str;
        if (stoi(toWhat[i]) == 1) {
            str = "dentist";
        }
        else if (stoi(toWhat[i]) == 2) {
            str = "surgeon";
        }
        else {
            str = "therapist";
        }
        std::cout<<"Client " << clients[i] << " go to " << str << " from doctor "<< docNum[i]<<std::endl;
    }
    delete[] threadC;
    delete[] consumers;
    return 0;
}