#include "SharedObject.h"
#include "Semaphore.h"
#include "Mailbox.h"
#include <stdlib.h>
#include <time.h>
#include <list>
#include "thread.h"

class WriterThread : public Thread
{
private:
	int threadId;
	int delayTime;
	Sync::Event killEvent;
	
public:
	WriterThread(int id, int s)
	:	threadId(id), delayTime(1000*s),killEvent()
	{
		;
	}
	void Kill(void)
	{
		killEvent.Trigger();
	}
	
	long ThreadMain(void)
	{
		Shared<Mailbox> mailbox("Mailbox",false);
		Semaphore protect("Protect");
		Semaphore available("Available");
		time_t now,then;
		int msgId = 0;

		time(&then);
		Sync::FlexWait pauser(1,&killEvent);
		for(int msgId=0;!pauser.Wait(delayTime);msgId++,then=now)
		{
			time(&now);
			protect.Wait();
			mailbox->Update(threadId,msgId,now-then);
			available.Signal();
		}
	}
	
};

int main(void)
{
    // The writer main thread owns the mailbox
    Shared<Mailbox> theBox("Mailbox",true);
    std::list<WriterThread *> threadList;
    Semaphore protect("Protect",1,true);
    Semaphore available("Available",0,true);

    for(int threadId=0;;threadId++)
    {
        std::string s;
        std::cout << "Would you like to create a writer thread? ";
        std::cin >> s;
        if (s!= "n")
        {
            int delayTime;
            std::cout << "What is the delay time for the new thread? ";
            std::cin >> delayTime;

            threadList.push_back(new WriterThread(threadId,delayTime));
        }
        else
        {
            std::list<WriterThread*>::iterator it;
            for (it=threadList.begin();it!=threadList.end();it++)
            {
                (*it)->Kill();
            }
            break;
        }
    }
}
