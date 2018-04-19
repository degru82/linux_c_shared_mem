#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <fcntl.h>

#include <time.h>

#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>

#include <semaphore.h>


int main (void) {

    int fd = 0;
    fd = open ("/tmp/kims", O_RDWR | O_CREAT);

    const int MAX_LEN = 20;
    char* shared_mem;
    shared_mem = mmap (NULL, MAX_LEN, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);

    memset (shared_mem, ' ', MAX_LEN);
    sprintf (shared_mem, "%s", "1298");
    close (fd);

    sem_t* sem;
    sem_init (sem, 0, 1);
    
    int i = 0;
    int pid[2] = {0};
    for (i=0; i<2; i++) {
        pid[i] = fork();

        if (pid[i] > 0) { // child proc
            int nth = i;
            int myPid = pid[i];
    
            while (1) {
                sleep(1);

                sem_wait (sem);

                int number = atoi (shared_mem);
                printf ("%dth proc/%d read/%d  write/%d\n", nth+1, myPid, number, number+1);
                fflush (stdout);

                sprintf (shared_mem, "%d", number+1);

                sem_post (sem);
            }
        }
    } 
 }