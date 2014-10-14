#include <string>
#include <vector>

#include <boost/interprocess/ipc/message_queue.hpp>

using namespace boost::interprocess;

int main1 ()
{
    message_queue *mq;

    struct msg {
        int n;
        char buf[4096];
    };

    msg m;
    memset(m.buf,'x',sizeof(m.buf));
    try{
        //Erase previous message queue
        message_queue::remove("message_queue");

        //Create a message_queue.
        mq = new message_queue
                (create_only               //only create
                        ,"message_queue"           //name
                        ,1024                       //max message number
                        ,size_t(8192)               //max message size
                );
    }
    catch(interprocess_exception &ex){
        std::cout << "MQ-Create Error:" << ex.what() << std::endl;
        return 1;
    }

    try {
        //Send 100 numbers
        for(int i = 0; i < 100; ++i){
            m.n = i;
            mq->send(&m, sizeof(m), 0);
        }
    }
    catch(interprocess_exception &ex){
        std::cout << "MQ-Send Error:" << ex.what() << std::endl;
        return 1;
    }

    return 0;
}

int main2 ()
{
    struct msg {
        int n;
        char buf[4096];
    };

    msg m;

    sleep(2);  // wait for queue to get set up

    message_queue *mq;
    try{
        //Open a message queue.
        mq = new message_queue
                (open_only        //only create
                        ,"message_queue"  //name
                );


    }
    catch(interprocess_exception &ex){
        message_queue::remove("message_queue");
        std::cout << "MQ-OPEN Error:" << ex.what() << std::endl;
        return 1;
    }
    unsigned int priority;
    message_queue::size_type recvd_size;

    try {
        //Receive 100 numbers
        for(int i = 0; i < 100; ++i){

            if ( !mq->try_receive(&m, size_t(8192), recvd_size, priority) )
                return 1;
            std::cout << getpid() << ":Received:" << m.n << std::endl;

        }
    }
    catch(interprocess_exception &ex){
        message_queue::remove("message_queue");
        std::cout << "MQ-RECV Error:" << ex.what() << std::endl;
        return 1;
    }
    message_queue::remove("message_queue");
    return 0;
}

int main ( int argc, char *argv[])
{

    if ( fork() == 0 )
        main1();
    else
        main2();

    return 0;
}

