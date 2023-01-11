#ifndef MUTEX_H
#define MUTEX_H

#include <thread>
using namespace std;

template <class T>
class Mutex
{
public:
    // Create a new Mutex from data
    Mutex(T);

    // Returns true if data was loaded to Data
    bool lock(T *&Data);

    // Unlock mutex and nullify Data pointer
    void unlock(T *&Data);

protected:
    bool isLocked;
    T data;
};

template<class T>
Mutex<T>::Mutex(T Data)
{
    data = Data;
    isLocked = false;
}

template<class T>
bool Mutex<T>::lock(T *&Data)
{
    if (this->isLocked) {
        Data = nullptr;
        return false;
    } else {
        Data = &data;
        isLocked = true;
        return true;
    }
}

template<class T>
void Mutex<T>::unlock(T *&Data)
{
    Data = nullptr;
    isLocked = false;
    return;
}

class ThreadPool
{
public:
    // Create a new pool of threads running a function
    ThreadPool(int Size, void (*Func)());

    // Join all threads and clean up
    ~ThreadPool();

    // Join all threads
    void joinAll();

protected:
    // Dynamically allocated array of thread references
    thread **threadHandles;

    int size;
    bool running;
};

ThreadPool::ThreadPool(int Size, void (*Func)())
{
    size = Size;
    threadHandles = new thread *[size];

    running = true;
    for (int i = 0; i < size; i++)
    {
        threadHandles[i] = new thread(Func);
    }

    return;
}

ThreadPool::~ThreadPool()
{
    if (running)
    {
        joinAll();
    }

    for (int i = 0; i < size; i++)
    {
        delete threadHandles[i];
        threadHandles[i] = nullptr;
    }
    delete [] threadHandles;

    return;
}

void ThreadPool::joinAll()
{
    for (int i = 0; i < size; i++)
    {
        threadHandles[i]->join();
    }
    running = false;

    return;
}

#endif
