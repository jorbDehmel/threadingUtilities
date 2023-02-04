#include <thread>
#include <iostream>
#include "mutex.hpp"
#include <vector>
using namespace std;

const int NUMTHREADS = 4;

Mutex<int> m(0);
int id_global = 0;
bool isRunning = true;
int counts[NUMTHREADS] = {0};

void threadFunction()
{
    id_global++;
    int id = id_global;

    int *realData;
    while (isRunning)
    {

        if (m.lock(realData))
        {
            cout << "Thread " << id << " got lock! Data is: " << *realData << "\n"
                 << flush;
            (*realData)++;

            int *notRealData = realData;

            m.unlock(realData);

            counts[id - 1]++;

            system((string("sleep ") + to_string((rand() & 100) / 1000.0)).c_str());
        }
        else
        {
            cout << "Thread " << id << " failed to get lock.\n";
        }
    }

    return;
}

int main()
{
    srand(time(NULL));
    ThreadPool<void (*)()> pool(NUMTHREADS, threadFunction);

    system("sleep 10");

    isRunning = false;
    pool.joinAll();

    for (int i = 0; i < NUMTHREADS; i++)
    {
        cout << "i=" << i + 1 << ": " << counts[i] << '\n';
    }

    return 0;
}
