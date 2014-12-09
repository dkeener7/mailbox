#include <string>
#include <vector>

#include <boost/interprocess/ipc/message_queue.hpp>

using namespace boost::interprocess;

// This program forks into to child processes which open reader and writer ends of message queue.
// The writer sends messages and the reader receives them.  It sends 1000 messages.
//

const int NumMessages = 1000;

// Writer process
int main1 ()
{
    message_queue *mq;

    struct msg {
        int n;
        char buf[4096];
    };

    msg m;
    memset(m.buf,'x',sizeof(m.buf));
    std::cout << "Starting Writer..." << std::endl;
    try{
        //Erase previous message queue
        message_queue::remove("message_queue");

        //Create a message_queue.
        mq = new message_queue
                (create_only               //only create
                        ,"message_queue"           //name
                        ,4096                       //max message number
                        ,size_t(8192)               //max message size
                );
    }
    catch(interprocess_exception &ex){
        std::cout << "MQ-Create Error:" << ex.what() << std::endl;
        return 1;
    }

    try {
        //Send 1000 numbers
        for(int i = 0; i < NumMessages; ++i){
            m.n = i;
            mq->send(&m, sizeof(m), 0);
        }
    }
    catch(interprocess_exception &ex){
        std::cout << "MQ-Send Error:" << ex.what() << std::endl;
        return 1;
    }

    std::cout << "Ending Writer..." << std::endl;

    return 0;
}

// Reader process
int main2 ()
{
    struct msg {
        int n;
        char buf[4096];
    };

    msg m;

    sleep(1);  // wait for queue to get set up

    std::cout << "Starting Reader..." << std::endl;

    message_queue *mq;
    try{
        //Open a message queue.
        mq = new message_queue
                (open_only        //only open
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
        //Receive 1000 numbers
        for(int i = 0; i < NumMessages; ++i){

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
    std::cout << "Ending Reader..." << std::endl;
    return 0;
}

int main ( int argc, char *argv[])
{

	std::cout << "Hi Igor" << std:endl;

    // fork into parent and child processes
    if ( fork() == 0 )
        main1();
    else
        main2();


    return 0;
}

