struct container {
  char name[16];             // unique key
  char consname[10];         // name of console
  char rootpath[64];         // path of rootdir
  struct inode* rootdir;     // root dir
  struct inode* cons;        // console

  int proc_used;
  int mem_used;
  int disk_used;

  int max_proc;
  int max_mem;
  int max_disk;
  
  int proc_tick;            // current lowest processor tick
  int ticks;                // total ticks
};
