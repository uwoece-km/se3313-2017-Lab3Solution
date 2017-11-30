#include "SharedObject.h"
#include "Semaphore.h"
#include "Mailbox.h"
#include <iostream>
#include <stdlib.h>
#include <time.h>

int main(void)
{
    Shared<Mailbox> mailbox("Mailbox",false);
    Semaphore protect("Protect");
    Semaphore available("Available");

    while (1)
    {
        available.Wait();
        mailbox->Show();
        protect.Signal();
    }
}
