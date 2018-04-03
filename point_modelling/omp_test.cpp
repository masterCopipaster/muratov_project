#include <iostream>
#include <omp.h>

using namespace std;

int main()
{
    #pragma omp parallel
    {
        cout << "Hello world" << endl;
    }
    return 0;
}
