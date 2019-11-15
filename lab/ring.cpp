#include <iostream>
#include <cstring>

using namespace std;

struct rr
{
    int id;
    bool finished;
    string state;
} process[10];

int i, j, k, m, n;

int main()
{
    int temp;
    cout << "Enter the number of process : ";
    cin >> n;

    for (i = 0; i < n; i++)
    {
        cout << "Enter id of process : ";
        cin >> process[i].id;
        process[i].state = "active";
        process[i].finished = false;
    }

    for (i = 0; i < n - 1; i++)
    {
        for (j = 0; j < n - 1; j++)
        {
            if (process[j].id > process[j + 1].id)
            {
                temp = process[j].id;
                process[j].id = process[j + 1].id;
                process[j + 1].id = temp;
            }
        }
    }

    int init;
    int temp1;
    int temp2;
    int arr[10];

    process[n - 1].state = "inactive";
    cout << "\nProcess " << process[n - 1].id << " is selected as coordinator \n";

    cout << "\nEnter the process number who intialised election : ";
    cin >> init;

    temp2 = init;
    temp1 = init + 1;
    i = 0;

    while (temp2 != temp1)
    {
        if (process[temp1].state == "active" && process[temp1].finished == false)
        {
            cout << "Process " << process[init].id << " send message to " << process[temp1].id << "\n";
            process[temp1].finished = true;
            init = temp1;
            arr[i] = process[temp1].id;
            i++;
        }

        if (temp1 == n)
            temp1 = 0;
        else
            temp1++;
    }

    cout << "Process " << process[init].id << " send message to " << process[temp1].id << "\n";
    arr[i] = process[temp1].id;
    i++;

    int max = -1;

    for (j = 0; j < i; j++)
        if (max < arr[j])
            max = arr[j];

    cout << "\nProcess " << max << " is selected as coordinator";

    for (i = 0; i < n; i++)
        if (process[i].id == max)
            process[i].state = "inactive";

    return 0;
}