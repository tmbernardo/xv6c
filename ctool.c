// container tools
// executes container commands
#include "types.h"
#include "fcntl.h"
#include "stat.h"
#include "user.h"

#define CTOOL_USAGE  "ctool (create | start | pause | resume) [<arg> ...]"
#define CREATE_USAGE "ctool create <name> [<path> ...]"
#define START_USAGE  "ctool start <name> [-p <max_proc>] [-m <max_mem>] [-d <max_disk>] <prog> [<arg1> ...]"
#define PAUSE_USAGE  "ctool pause <name>"
#define RESUME_USAGE "ctool resume <name>"
#define STOP_USAGE   "ctool stop <name>"
#define INFO_USAGE   "ctool info"

#define MAX_FILENAME 15

void
create_vc(char* dname)
{
    int i, fd;
    char num[8];

    strcpy(dname, "vc");

    i = vcmake();

    num[0] = 0;
    dname[2] = 0;
    sprintf((char *)&num, "%d", i);
    strcat(dname, num);

    if ((fd = open(dname, O_RDWR)) < 0){
        mknod(dname, 1, i+1);
    } else {
        // vc already exists...
        close(fd);
    }
}

// create container and populate FS
int
create (int argc, char **args) {

    char *name;
    char *cp_str[4];
    char rootdir[16];
    int i, pid;

    if (argc < 1) {
        printf(1, "%s\n", CREATE_USAGE);
        return -1;
    }

    name = args[0];

    if (strcmp(name, "/") == 0) {

        // root (special-case)

        // root_dir = name;
        //    no cp needed...
        strcpy(rootdir, "");

    } else {

        mkdir(name);

        if (argc > 1) {
            for (i = 1; i < argc; i++) {
                pid = fork();

                if (pid == 0) {

                    cp_str[0] = malloc(2 + 1);
                    strcpy(cp_str[0], "cp");

                    cp_str[1] = malloc(strlen(args[i]) + 1);
                    strcpy(cp_str[1], args[i]);

                    cp_str[2] = malloc((strlen(name) + 1 + strlen(args[i])) + 1);
                    strcpy(cp_str[2], name);
                    strcat(cp_str[2], "/");
                    strcat(cp_str[2], args[i]);

                    cp_str[3] = (char*)0;

                    exec("cp", (char**)cp_str);
                    printf(1, "ctool: create: exec fail\n");
                    exit();

                }
            }

            for (i = 1; i < argc; i++) {
                wait();
            }
        }

        strcpy(rootdir, "/");
    }

    strcat(rootdir, name);

    cmake(name, rootdir);

    return 0;
}

// start a container
int
start (int argc, char **args) {
    // give a virtual console, container directory, an initial program to start, 
    // max number of processes, max mem, max disk space (you can have default values for the resources)

    char *name;
    char **prog;
    char vc_name[10];
    int maxproc = -1;
    int maxmem = -1;
    int maxdisk = -1;

    int i;
    int fd;
    int id;

    if (argc < 2) {
        printf(1, "%s\n", START_USAGE);
        return -1;
    }

    // parse container args
    name = args[0];
    for (i = 1; i < argc; i++) {
        if (strcmp(args[i], "-p") == 0 && i + 1 < argc) {
            maxproc = atoi(args[++i]);
        } else if (strcmp(args[i], "-m") == 0 && i + 1 < argc) {
            maxmem = atoi(args[++i]);
        } else if (strcmp(args[i], "-d") == 0 && i + 1 < argc) {
            maxdisk = atoi(args[++i]);
        } else {
            prog = &args[i];
            break;
        }
    }

    if (climits(name, maxproc, maxmem, maxdisk) == -1) {
        printf(1, "ctools: start: %s: container not found\n", name);
        exit();
    }

    id = cfork(name);

    if (id == 0) {

        create_vc(vc_name);
        ccons(name, vc_name);    
        fd = open(vc_name, O_RDWR);

        if (!strcmp(name, "/") == 0) {
            chdir(name);
        }
        close(0);
        close(1);
        close(2);
        dup(fd);
        dup(fd);
        dup(fd);
        exec(prog[0], prog);
        exit();

    } else if (id == -1) {
        printf(1, "ctools: start: %s: failed to start %s\n", name, prog[0]);
    } else {
        printf(1, "ctools: start: %s: started %s\n", name, prog[0]);
    }

    exit();
}

// pause a container
int
pause (int argc, char *args[]) {

    if (argc < 1) {
        printf(1, "%s\n", PAUSE_USAGE);
        return -1;
    }

    char *name = args[0];
    if (cpause(name) != 0) {
        printf(1, "ctools: pause: could not pause container %s\n", name);
    }

    printf(1, "ctools: pause: paused container %s\n", name);
    return 0;
}

// resume a container
int
resume (int argc, char *args[]) {

    if (argc < 1) {
        printf(1, "%s\n", RESUME_USAGE);
        return -1;
    }

    char *name = args[0];
    if (cresume(name) != 0) {
        printf(1, "ctools: resume: could not resume container %s\n", name);
    }

    printf(1, "ctools: resume: resumed container %s\n", name);

    return 0;
}

// stop a container
int
stop(int argc, char *args[]) {

    if (argc < 1) {
        printf(1, "%s\n", STOP_USAGE);
        return -1;
    }

    char *name = args[0];

    // stop

    return cstop(name);
}

// show all containers
void
info(int argc, char *args[]) {

    cinfo();
    // print all container stats
}

// parses first argument and passes to function
int
parse_tool(int argc, char *argv[]) {

    if (argc < 2) {
        printf(1, "%s\n", CTOOL_USAGE);
        return -1;
    }

    char *funct = argv[1];
    
    if (strcmp(funct, "create") == 0) {
        create(argc-2, &argv[2]);
    } else if (strcmp(funct, "start") == 0) {
        start(argc-2, &argv[2]);
    } else if (strcmp(funct, "pause") == 0) {
        pause(argc-2, &argv[2]);
    } else if (strcmp(funct, "resume") == 0) {
        resume(argc-2, &argv[2]);
    } else if (strcmp(funct, "stop") == 0) {
        stop(argc-2, &argv[2]);
    } else if (strcmp(funct, "info") == 0) {
        info(argc-2, &argv[2]);
    } else {
        printf(1, "ctool: %s: tool not found\n%s\n", funct, CTOOL_USAGE);
    }

    return 0;
}

int main(int argc, char *argv[]) {

    parse_tool(argc, argv);

    exit();
}
