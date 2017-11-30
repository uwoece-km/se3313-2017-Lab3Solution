#ifndef MAILBOX_H
#define MAILBOX_H
class Mailbox
{
private:
    int threadId;
    int msgId;
    int sinceLast;
public:
    void Show(void)
    {
        std::cout << threadId << " " << msgId << " " << sinceLast << std::endl;
    }

    void Update(int threadId, int msgId, int sinceLast)
    {
        this->threadId = threadId;
        this->msgId = msgId;
        this->sinceLast = sinceLast;
    }
};

#endif // MAILBOX_H
