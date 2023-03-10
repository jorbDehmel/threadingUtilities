#ifndef MUTEX_H
#define MUTEX_H

#include <thread>
using namespace std;

////////////////////////////////////

template <class T>
class Mutex
{
public:
    // Create a new Mutex from data
    Mutex(T Data);

    // Returns true if data was loaded to Data
    bool lock(T *&Data);

    // Unlock mutex and nullify Data pointer
    void unlock(T *&Data);

protected:
    bool isLocked;
    T data;
};

template <class T>
Mutex<T>::Mutex(T Data)
{
    data = Data;
    isLocked = false;
}

template <class T>
bool Mutex<T>::lock(T *&Data)
{
    if (this->isLocked)
    {
        Data = nullptr;
        return false;
    }
    else
    {
        Data = &data;
        isLocked = true;
        return true;
    }
}

template <class T>
void Mutex<T>::unlock(T *&Data)
{
    if (Data != &data)
    {
        throw runtime_error("Mutex contents not returned during unlock");
    }

    Data = nullptr;
    isLocked = false;
    return;
}

////////////////////////////////////

template <class T, class A>
class ArgThreadPool
{
public:
    // Create a new pool of threads running a function
    ArgThreadPool(int Size, T Func, A Arg);

    // Join all threads and clean up
    ~ArgThreadPool();

    // Join all threads
    void joinAll();

protected:
    // Dynamically allocated array of thread references
    thread **threadHandles;

    int size;
    bool running;
};

template <class T, class A>
ArgThreadPool<T, A>::ArgThreadPool(int Size, T Func, A Arg)
{
    size = Size;
    threadHandles = new thread *[size];

    running = true;
    for (int i = 0; i < size; i++)
    {
        threadHandles[i] = new thread(Func, Arg);
    }

    return;
}

template <class T, class A>
ArgThreadPool<T, A>::~ArgThreadPool()
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
    delete[] threadHandles;

    return;
}

template <class T, class A>
void ArgThreadPool<T, A>::joinAll()
{
    for (int i = 0; i < size; i++)
    {
        threadHandles[i]->join();
    }
    running = false;

    return;
}

////////////////////////////////////

template <class T>
class ThreadPool
{
public:
    // Create a new pool of threads running a function
    ThreadPool(int Size, T Func);

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

template <class T>
ThreadPool<T>::ThreadPool(int Size, T Func)
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

template <class T>
ThreadPool<T>::~ThreadPool()
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
    delete[] threadHandles;

    return;
}

template <class T>
void ThreadPool<T>::joinAll()
{
    for (int i = 0; i < size; i++)
    {
        threadHandles[i]->join();
    }
    running = false;

    return;
}

////////////////////////////////////

#endif
