#include <stdio.h>
#include <sys/utsname.h>

int main(){
    struct utsname u;
    uname(&u);
    printf("Hello from %s\n", u.nodename);
    printf(" Kernel: %s\n", u.release);
    printf(" Architecture: %s\n", u.machine);
    return 0;
}

