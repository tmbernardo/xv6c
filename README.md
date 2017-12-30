# xv6c  
A simple container for the xv6 operating system by MIT  
Can currently only be run on a raspberry pi.

### Authors: Matthew Bernardo && Brian Bushree  

## Root Container  
Once xv6 is run in qemu a root container is created which initializes the xv6 OS.  
The root container is monolithic as it has the ability to create, manipulate and monitor "child" containers along with xv6 system resources.  

## Child Container  
Child containers are completely isolated and can only operate within its own scope.
In order to do this we needed to implement proper system isolation which includes:
    - Process Isolation
    - File System Isolation
    - Memory Isolation

## USAGE  
### Qemu  
```
$ make qemu
```
This will run xv6 and start a root container for the user  
Once the root container is running and user has access to the console/shell we can start creating containers using the container tools.

### Container Tools  
The ctool user program is responsible for controlling how each container behaves as well as the resources that are allocated to it.  
ctool uses system calls that we have implemented in order to create container resources safely using the kernel.

#### Create Container:  
```
$ ctool create <name> [<path> ...]
```
Creates a container
name - desired name of container  
path - path(s) to programs that the user would like to copy to the child container  

#### Start Container:  
```
$ ctool start <name> [-p <max_proc>] [-m <max_mem>] [-d <max_disk>] <prog> [<arg1> ...]
```
Start a created container
Flags:
    -p: max processes a container is allowed to run
    -m: max memory a container has access to (in bytes)
    -d: max disk space to be allocated to container

prog - program to start within container followed by its arguments

#### Pause Container:  
```
$ ctool pause <name>
```

#### Resume Container:  
```
$ ctool resume <name>
```

#### Stop Container:  
```
$ ctool stop <name>
```

#### View Container Information:  
```
$ ctool info
```

