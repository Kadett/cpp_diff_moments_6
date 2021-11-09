#include <iostream>
#include <thread>
#include <mutex>
#include <queue>
#include "Timer.h"


using namespace std;

mutex m;

void pcout(string str) {
    m.lock();
    this_thread::sleep_for(0.5s);
    cout << str << endl;
    m.unlock();
}

uint64_t primeNumber(int n) {
    system("cls");
    uint64_t res{};
    int i, j, count{};
    int percent{};
    for (i = 2; count < n; i++) {
        for (j = 2; j * j <= i; j++) {
            if (i % j == 0) break;
        }
        if (j * j > i) {
            ++count;
            res = i;

        }
        if (count * 100 / n > percent) {
            system("cls"); // В Clion не поддерживается команда system("cls"), по этому вывод в консоль в Clion будет кривой.
            percent = count * 100 / n;   // Если запустить в командной строке, то прогресс будет отображаться корректно.
            cout << fixed;
            cout.precision(1);
            cout << "Progress " << percent << "%";
        }
    }
    cout << endl;
    return res;
}

void bring_thing(priority_queue<pair<int, string>> &house, const vector<pair<int, string>> &things, mutex &mut) {
    pair<int, string> t;
    while (!house.empty()) {
        t = things[rand() % 10];
        mut.lock();
        house.push(t);
        cout << "The owner brought a " << t.second << ". Cost = " << t.first << endl;
        mut.unlock();
        this_thread::sleep_for(2s);
    }

}

void steal_thing(priority_queue<pair<int, string>> &house, const vector<pair<int, string>> &things, mutex &mut) {
    while (!house.empty()) {
        mut.lock();
        cout << "The thief stole a " << house.top().second << ". Cost = " << house.top().first << endl;
        house.pop();
        mut.unlock();
        this_thread::sleep_for(1s);
    }
    cout << "The thief stole all things!";
}

int main() {
    // Task 1
    cout << "Task 1: " << endl;
    for (int i = 0; i < 5; ++i) {
        thread th1(pcout, "thread 1");
        thread th2(pcout, "thread 2");
        th1.join();
        th2.join();
    }

    // Task 2
    cout << "\n\n" << "Task 2: " << endl;
    int n = 1'000'000;
    uint64_t res;

    thread th3([&](){res = primeNumber(n);});
    th3.join();
    cout << "Prime number " << n << " = " << res << endl;

    // Task 3
    cout << "\n\n" << "Task 3: " << endl;
    mutex mut;
    vector<pair<int, string>> things{
            {1,  "food"},
            {2,  "chair"},
            {3,  "lamp"},
            {4,  "mirror"},
            {5,  "printer"},
            {6,  "sofa"},
            {7,  "bed"},
            {8,  "TV"},
            {9,  "PC"},
            {10, "microwave"}
    };
    priority_queue<pair<int, string>> house;
    for (int i = 0; i < 10; ++i) {
        house.push(things[rand() % 10]);
    }
/*    while (!house.empty()) {
        cout << house.top().first << " : " << house.top().second << endl;
        house.pop();
    }*/

    thread owner([&](){bring_thing(house,things,mut);});
    thread thief([&](){steal_thing(house,things,mut);});
    owner.join();
    thief.join();

    return 0;
}
