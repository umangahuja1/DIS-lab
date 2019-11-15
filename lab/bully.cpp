#include <stdio.h>
#include <cstring>
#include <iostream>
#include <stdlib.h>

using namespace std;

struct process
{
    string name;
    int prior;
    string state;
} p[10];

int main()
{
    int i, j, k, l, m, n;

    cout << "\nEnter the number of proceess \t";
    cin >> n;

    for (i = 0; i < n; i++)
    {
        cout << "Enter the name of process\t";
        cin >> p[i].name;
        cout << "Enter the priority of process\t";
        cin >> p[i].prior;
        p[i].state = "active";
    }

    for (i = 0; i < n - 1; i++)
    {
        for (j = 0; j < n - 1; j++)
        {
            if (p[j].prior < p[j + 1].prior)
            {
                string temp;
                int t = p[j].prior;
                p[j].prior = p[j + 1].prior;
                p[j + 1].prior = t;
                temp = p[j].name;
                p[j].name = p[j + 1].name;
                p[j + 1].name = temp;
            }
        }
    }

    int min = 0;
    for (i = 0; i < n; i++)
        cout << p[i].name << "\t" << p[i].prior << endl;

    for (i = 0; i < n; i++)
    {
        for (i = 0; i < n; i++)
        {
            if (min < p[i].prior)
                min = p[i].prior;
        }
    }

    for (i = 0; i < n; i++)
    {
        if (p[i].prior == min)
        {
            cout << "\nProcess " << p[i].name << " is selected as coordinator";
            p[i].state = "iactive";
            break;
        }
    }

    int pr;
    while (1)
    {
        int ch;
        cout << "\n1)election\t";
        cout << "\n2) exit  \t";
        cin >> ch;
        int max = 0;
        int ar[20];
        string str;
        k = 0;
        int fl = 0;
        switch (ch)
        {
        case 1:
            cout << "\n 1)intialise election\t";

            cin >> str;
            fl = 0;
        l1:
            for (i = 0; i < n; i++)
            {
                if (str == p[i].name)
                    pr = p[i].prior;
            }

            for (i = 0; i < n; i++)
            {
                if (pr < p[i].prior)
                {
                    cout << "\nProcess " << str << " send message to " << p[i].name;
                }
            }
            for (i = 0; i < n; i++)
            {
                if (pr < p[i].prior && p[i].state == "active")
                {
                    if (fl == 0)
                    {
                        ar[k] = p[i].prior;
                        k++;
                    }
                    cout << "\nProcess " << p[i].name << " send OK message to " << str;
                    if (p[i].prior > max)
                        max = p[i].prior;
                }
            }
            fl = 1;

            if (k != 0)
            {
                k = k - 1;
                for (i = 0; i < n; i++)
                {
                    if (ar[k] == p[i].prior)
                        str = p[i].name;
                }

                goto l1;
            }
            m = 0;
            for (j = 0; j < n; j++)
            {
                if (p[j].prior > m && p[j].state == "active")
                {
                    cout << "\nProcess " << p[j].name << " is selected as new coordinator";
                    p[j].state = "inactive";
                    break;
                }
            }

            for (i = 0; i < n; i++)
            {
                if (p[i].state == "active" && p[j].prior > p[i].prior)
                {
                    cout << "\nProcess " << p[j].name << " send alert message to " << p[i].name;
                }
            }

            break;
        case 2:
            exit(1);
        }
    }

    return 0;
}