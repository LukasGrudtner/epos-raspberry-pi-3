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
        CREATE0,
        CREATE1,
        CREATE2,
        CREATE3,
        CREATE4,
        CREATE5,
        CREATE6,
        CREATE7,
        CREATE8,
        CREATE9,

        DESTROY,

        SELF,

        COMPONENT = 0x10,

        THREAD_STATE = COMPONENT,
        THREAD_PRIORITY0,
        THREAD_PRIORITY1,
        THREAD_JOIN,
        THREAD_PASS,
        THREAD_SUSPEND,
        THREAD_RESUME,
        THREAD_YIELD,
        THREAD_EXIT,

        PRINT = COMPONENT,

        UNDEFINED = -1
    };

    typedef int Method;
    typedef Method Result;

    typedef Simple_List<Message> List;
    typedef List::Element Element;

public:
    Message(const Id & id): _id(id), _link(this) {}

    template<typename ... Tn>
    Message(const Id & id, const Method & m, Tn && ... an): _id(id), _method(m), _link(this) { out(an ...); }

    const Id & id() const { return _id; }
    void id(const Id & id) { _id = id; }

    const Method & method() const { return _method; }
    void method(const Method & m) { _method = m; }

    const Result & result() const { return _method; }
    void result(const Result & r) { _method = r; }

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

    void act() { _syscall(this); };

    friend Debug & operator << (Debug & db, const Message & m) {
        db << "{id=" << m._id << ",m=" << hex << m._method << ",p={"
           << reinterpret_cast<void *>(*static_cast<const int *>(reinterpret_cast<const void *>(&m._parms[0]))) << ","
           << reinterpret_cast<void *>(*static_cast<const int *>(reinterpret_cast<const void *>(&m._parms[4]))) << ","
           << reinterpret_cast<void *>(*static_cast<const int *>(reinterpret_cast<const void *>(&m._parms[8]))) << "}}";
        
        return db;
    }

private:
    Id _id;
    Method _method;
    char _parms[MAX_PARAMETERS_SIZE];

    Element _link;
};

__END_SYS

#endif
