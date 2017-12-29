
## Project 4 Tests


### ISOLATION TESTS

#### scheduler test:
ctool create s1 sh ls cat schedtest kill ps
ctool start s1 sh
schedtest 2 1000
ctrl-t
schedtest 1 1000
cat sched.out (both consoles)

#### process isolation test:
(in root)
ps (get PID of process in root)
ctrl-t
kill <PID>
ctrl-t
ps (show process is alive)

FS isolation test:
(make sure were in container s1)
cd ..
ls

### RESOURCE LIMIT TESTS

#### forkbomb test:
ctool create c1 sh forkbomb membomb diskbomb
ctool start c1 -m 50000000 sh
forkbomb

#### membomb test:
ctool start c1 -m 50000000 sh
membomb

#### diskbomb test:
ctool start c1 -m 50000000 -d 15000
diskbomb


