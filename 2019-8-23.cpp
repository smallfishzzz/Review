#include <cstring>
#include <iostream>
using namespace std;



//输出数组中过半的元素
int main()
{
    int str[] = {1,2,3,2,2,2,5,4,2};

    int temp;
    int len = sizeof(str)/sizeof(int);
    int p[len] = {0};
 
    for (int i = 0; i < len; i++)
    {
        p[str[i] ]++;
        if (  p[str[i] ] > len/2)
        {
            cout << "asd" << str[i]   << endl;
            return 0;
        }
        
    }
    






    return 0;
}