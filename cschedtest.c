#include "fcntl.h"
#include "types.h"
#include "stat.h"
#include "user.h"

char *cont_create[] = {"ctool", "create", "stest", "schedtest", 0};
char *cont_create2[] = {"ctool", "create", "stest2", "schedtest", 0};
char *cont_create3[] = {"ctool", "create", "stest3", "schedtest", 0};

char *cont_start[] = {"ctool", "start", "stest", "schedtest", "2", "5000", 0};
char *cont_start2[] = {"ctool", "start", "stest2", "schedtest", "3", "5000", 0};
char *cont_start3[] = {"ctool", "start", "stest3", "schedtest", "4", "5000", 0};

char *exec_schedtest[] = {"schedtest", "1", "1000", 0};
char *exec_info[] = {"ctool", "info", 0};

int main(int argc, char *argv[]) { 
    int pid;

    // make c1
    pid = fork();
    if (pid == 0) {
        exec("ctool", cont_create);
        exit();
    }
    wait();

    // make c2
    pid = fork();
    if (pid == 0) {
        exec("ctool", cont_create2);
        exit();
    }
    wait();

    // make c3
    pid = fork();
    if (pid == 0) {
        exec("ctool", cont_create3);
        exit();
    }
    wait();

    // start c1
    pid = fork();
    if(pid == 0) {
        exec("ctool", cont_start);
        exit();
    }

    // start c2
    pid = fork();
    if(pid == 0) {
        exec("ctool", cont_start2);
        exit();
    }

    // start c3
    pid = fork();
    if(pid == 0) {
        exec("ctool", cont_start3);
        exit();
    }

    wait();
    wait();
    wait();

    // run info
    // pid = fork();
    // if(pid == 0) {
    //     exec("ctool", exec_info);
    //     exit();
    // }
    // wait();

    exit();
}
