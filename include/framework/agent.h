#ifndef __agent_h
#define __agent_h

#include <machine.h>

#include "message.h"

__BEGIN_SYS

class Agent: public Message {
private:
    typedef void (Agent:: * Member)();

public:
    void exec() {
        switch(id().type()) {
        case UTILITY_ID:
            handle_utility();
            break;
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

private:
    static Member _handlers[LAST_TYPE_ID];
};

__END_SYS

#endif
