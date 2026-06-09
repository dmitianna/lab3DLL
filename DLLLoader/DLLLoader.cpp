
#include <Windows.h>
#include <winerror.h>

int main()
{
    // You also specify the complete path.
    LoadLibraryA("VirusDLL.dll");
    return 0;
}
