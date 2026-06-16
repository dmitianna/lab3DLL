#include <Windows.h>
#include <winerror.h>

#include <stdio.h>

int main()
{
    int i = 0;
    printf("Target Process PID: %lu\n", GetCurrentProcessId());
    while (true)
    {   
        printf("Processing - %d\n", i++);
        Sleep(1000);
    }
    return 0;
}
