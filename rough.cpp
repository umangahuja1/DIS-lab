#include <iostream>
using namespace std;

int main()
{

#ifdef __WIN32__
    cout << "yes";
#else
    cout << "no";
#endif
}