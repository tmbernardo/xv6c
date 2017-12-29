// prints used/total memory
#include "types.h"
#include "user.h"

int main(int argc,char **argv) {
    printf(1, "used disk:\t%d\n", udisk());
    printf(1, "total disk:\t%d\n", tdisk());
    exit();
}
