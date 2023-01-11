#include <thread>
#include <iostream>
#include "mutex.hpp"
#include <vector>
using namespace std;

Mutex<int> m(0);
int id_global = 0;
bool isRunning = true;
int counts[5] = {0};

void threadFunction()
{
    id_global++;
    int id = id_global;

    while (isRunning)
    {
        int *realData;

        if (m.lock(realData))
        {
            cout << "Thread "<< id << " got lock! Data is: " << *realData << "\n";
            (*realData)++;
            m.unlock(realData);

            counts[id - 1]++;

            system((string("sleep ") + to_string((rand() & 100) / 1000.0)).c_str());
        }
        else
        {
            cout << "Thread "<< id << " failed to get lock.\n";
        }
    }

    return;
}

int main()
{    
    srand(time(NULL));
    ThreadPool pool(5, threadFunction);

    system("sleep 5");
    
    isRunning = false;
    pool.joinAll();

    for (int i = 0; i < 5; i++)
    {
        cout << "i=" << i + 1 << ": " << counts[i] << '\n';
    }

    return 0;
}
