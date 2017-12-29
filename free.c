// prints used/total memory
#include "types.h"
#include "user.h"

int main(int argc,char **argv) {
    printf(1, "used mem:\t%d\n", umem());
    printf(1, "total mem:\t%d\n", tmem());
    exit();
}
