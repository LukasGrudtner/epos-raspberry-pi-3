#ifndef __agent_h
#define __agent_h

#include <machine.h>
#include <process.h>

#include "message.h"

__BEGIN_SYS

class Agent: public Message {
private:
    typedef void (Agent:: * Member)();

public:
    void exec() {
        switch(id().type()) {
        case UTILITY_ID:
            Display::puts("\nAgent::handle_utility()\n");
            handle_utility();
            break;
        // case THREAD_ID:
        //     Display::puts("\nAgent::handle_thread/()\n");
        //     handle_thread();
        //     break;
        default:
            result(UNDEFINED);
        }
    }

private:
    void handle_utility() {
        Result res = 0;

        switch(method()) {
        case PRINT: {
            const char * s;
            in(s);
            Display::puts(s);
        } break;
        default:
            res = UNDEFINED;
        }

        result(res);
    }

    // void handle_thread()
    // {
    //     Thread * thread = reinterpret_cast<Thread *>(id().unit());
    //     Result res = 0;

    //     switch(method()) {
    //     case CREATE1:
    //     case CREATE2:
    //     case CREATE3:
    //     case CREATE4: {
    //         int p1, p2, p3;
    //         in(p1); in(p2); in(p3);
    //         int (*entry)(int, int, int);
    //         in(entry);
    //         id(Id(THREAD_ID, reinterpret_cast<Id::Unit_Id>(new Thread(Thread::Configuration(Thread::READY, Thread::NORMAL, 0, 0), entry, p1, p2, p3))));
    //     } break;
    //     case CREATE5:
    //     case CREATE6:
    //     case CREATE7:
    //     case CREATE8:
    //     case CREATE9: {
    //         int (*entry)();
    //         in(entry);
    //         id(Id(THREAD_ID, reinterpret_cast<Id::Unit_Id>(new Thread(Thread::Configuration(Thread::READY, Thread::NORMAL, 0, 0), entry))));
    //     } break;
    //     case DESTROY:
    //         delete thread;
    //         break;
    //     case SELF:
    //         id(Id(THREAD_ID, reinterpret_cast<Id::Unit_Id>(Thread::self())));
    //         break;
    //     case THREAD_PRIORITY0:
    //         res = thread->priority();
    //         break;
    //     case THREAD_PRIORITY1: {
    //         int p;
    //         in(p);
    //         thread->priority(Thread::Criterion(p));
    //     } break;
    //     case THREAD_JOIN:
    //         res = thread->join();
    //         break;
    //     case THREAD_PASS:
    //         thread->pass();
    //         break;
    //     case THREAD_SUSPEND:
    //         thread->suspend();
    //         break;
    //     case THREAD_RESUME:
    //         thread->resume();
    //         break;
    //     case THREAD_YIELD:
    //         Thread::yield();
    //         break;
    //     case THREAD_EXIT: {
    //         int r;
    //         in(r);
    //         Thread::exit(r);
    //     } break;
    //     default:
    //         res = UNDEFINED;
    //     }

    //     result(res);
    // };

private:
    static Member _handlers[LAST_TYPE_ID];
};

__END_SYS

#endif
