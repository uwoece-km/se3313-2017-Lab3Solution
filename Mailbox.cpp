#include <sys/mman.h>
#include <sys/stat.h>        /* For mode constants */
#include <fcntl.h>           /* For O_* constants */
#include <unistd.h>
#include <sys/types.h>
#include <iostream>
#include <stdlib.h>
#include <time.h>

struct Mailbox
{
    int procId;
    int msgId;
    int number;
    void Show();
};

void Mailbox::Show()
{
    std::cout << procId << " " << msgId << " " << number << std::endl;
}

#ifdef LOCAL_TEST
int main(void)
{
    pid_t child = fork();

    if (child > 0)
    {
        int sharedMemory = shm_open("/Mailbox",O_RDWR | O_CREAT,0777);
        ftruncate(sharedMemory, sizeof(Mailbox));
        Mailbox * pm = (Mailbox*) mmap(NULL, sizeof(Mailbox), 0777, MAP_SHARED,sharedMemory,0);
        pm->procId = 1;
        srand (time(NULL));
        for (int i=0;i<100;i++)
        {
            pm->msgId = i;
            pm->number = rand()%100 +1;
            sleep(1);
        }
        munmap(pm,sizeof(Mailbox));
        shm_unlink("Mailbox");
    }
    else if (child == 0)
    {
        int sharedMemory = shm_open("/Mailbox",O_RDWR | O_CREAT,0777);
        ftruncate(sharedMemory, sizeof(Mailbox));
        Mailbox * pm = (Mailbox*) mmap(NULL, sizeof(Mailbox), 0777, MAP_SHARED,sharedMemory,0);

        for (int i=0;i<200;i++)
        {
           pm->Show();
           usleep(750000);
        }
        munmap(pm,sizeof(Mailbox));
        shm_unlink("Mailbox");
    }
}
#endif