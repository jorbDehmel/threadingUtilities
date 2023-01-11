#include "mutex.hpp"
#include <vector>
#include <string>
#include <thread>
#include <cassert>
#include <iostream>
using namespace std;

///////////////////////////////////////

class Foobar;
void update(Foobar *what);

///////////////////////////////////////

class Foobar
{
public:
    Foobar(int NumThreads, string WhatToAdd, int HowMuch);
    ~Foobar();

    const string what;
    const int howMuch;

    ThreadPool<void (*)(Foobar *), Foobar *> *threads;
    Mutex<vector<string>> *strings;
};

///////////////////////////////////////

Foobar::Foobar(int NumThreads, string WhatToAdd, int HowMuch) : what(WhatToAdd), howMuch(HowMuch)
{
    vector<string> temp;
    strings = new Mutex<vector<string>>(temp);

    threads = new ThreadPool<void (*)(Foobar *), Foobar *>(NumThreads, &update, this);  
}

Foobar::~Foobar()
{
    delete threads;
    delete strings;
}

///////////////////////////////////////

int id = 0;
void update(Foobar *what)
{
    id++;
    int cur_id = id;

    bool running = true;

    while (running)
    {
        // Simulate an expensive calculation
        double toDelay = (rand() % 500 / 100.0);
        system((string("sleep ") + to_string(toDelay)).c_str());

        // Get lock, waiting if needed
        vector<string> *real;
        while (!what->strings->lock(real)) {
            system("sleep .1");
        }

        // Log result to object
        cout << "Thread " << cur_id << " got lock.\n";        
        if (real->size() >= what->howMuch)
        {
            running = false;
        }
        else
        {
            real->push_back(what->what);
        }

        // Unlock
        what->strings->unlock(real);
    }
}

///////////////////////////////////////

int main()
{
    auto start = time(NULL);

    Foobar f(4, "hi", 16);
    f.threads->joinAll();

    cout << "Contents:\n";

    vector<string> *real;
    assert(f.strings->lock(real));
    for (auto f : *real)
    {
        cout << f << '\n';
    }

    cout << "Size: " << real->size() << '\n';

    auto end = time(NULL);

    cout << "Took " << end - start << " seconds.\n";
    cout << "(1 thread costs on average " << (real->size() * 5) / 2 << " seconds)\n";

    return 0;
}