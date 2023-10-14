/* this is a executable file
used to test the hacker functions
*/

#include <stdio.h>
#include <windows.h>


int main(int argc, char *argv[])
{
    int count = 0;
    int rate = 1;

    while (1)
    {
        printf("sheep count : %d\n", count);
        // sleep here
        getchar();
        count += rate;
    }
}