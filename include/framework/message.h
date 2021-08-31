#ifndef __message_h
#define __message_h

#include <utility/list.h>

#include "id.h"

extern "C" { void _syscall(void *); }

__BEGIN_SYS

class Message {
private:
    static const unsigned int MAX_PARAMETERS_SIZE = 256;

public:
    enum {
        PRINT
    };

    typedef int Method;
    typedef Method Result;

    typedef Simple_List<Message> List;
    typedef List::Element Element;

public:
    template<typename ... Tn>
    Message(const Id & id, const Method & m, Tn && ... an): _id(id), _method(m), _link(this) { out(an ...); }

    template<typename ... Tn>
    void in(Tn && ... an) {
        typename IF<(SIZEOF<Tn ...>::Result <= MAX_PARAMETERS_SIZE), int, void>::Result index = 0;
        DESERIALIZE(_parms, index, an ...);
    }

    template<typename ... Tn>
    void out(const Tn & ... an) {
        typename IF<(SIZEOF<Tn ...>::Result <= MAX_PARAMETERS_SIZE), int, void>::Result index = 0;
        SERIALIZE(_parms, index, an ...);
    }

    void act() {
        // const char * s;
        // in(s);
        // Display::puts(s);
        _syscall(this);
    };

private:
    Id _id;
    Method _method;
    char _parms[MAX_PARAMETERS_SIZE];

    Element _link;
};

__END_SYS

#endif
