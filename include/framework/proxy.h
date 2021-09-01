#ifndef __proxy_h
#define __proxy_h

#include "message.h"

__BEGIN_SYS

template<typename Component>
class Proxy;


template<typename Component>
class Proxied: public Proxy<Component>
{
public:
    Proxied(): Proxy<Component>(Proxy<Component>::PROXIED) {}

    void * operator new(size_t s, void * adapter) {
        Framework::Element * el= Framework::_cache.search_key(reinterpret_cast<unsigned int>(adapter));

        void * proxy;

        if(el) {
            proxy = el->object();
        } else {
            proxy = new Proxy<Component>(Id(Type<Component>::ID, reinterpret_cast<Id::Unit_Id>(adapter)));
            el = new Framework::Element(proxy, reinterpret_cast<unsigned int>(adapter));
            Framework::_cache.insert(el);
        }

        return proxy;
    }
};


template<typename Component>
class Proxy: public Message
{
    template<typename> friend class Proxy;
    template<typename> friend class Proxied;

private:
    enum Private_Proxied { PROXIED };

public:
    template<typename ... Tn>
    Proxy(const Tn & ... an): Message(Id(Type<Component>::ID, 0)) { invoke(CREATE0 + sizeof ... (Tn), an ...); }

    ~Proxy() { invoke(DESTROY); }

    static Proxy<Component> * self() { return new (reinterpret_cast<void *>(static_invoke(SELF))) Proxied<Component>; }

    int state() { return invoke(THREAD_STATE); }
    int priority() { return invoke(THREAD_PRIORITY0); }
    void priority(int p) { invoke(THREAD_PRIORITY1, p); }
    int join() { return invoke(THREAD_JOIN); }
    int pass() { return invoke(THREAD_PASS); }
    void suspend() { invoke(THREAD_SUSPEND); }
    void resume() { invoke(THREAD_RESUME); }
    static int yield() { return static_invoke(THREAD_YIELD); }
    static void exit(int r) { static_invoke(THREAD_EXIT, r); }

private:
    template<typename ... Tn>
    Result invoke(const Method & m, const Tn & ... an) {
        method(m);
        out(an ...);
        act();
        return result();
    }

    template<typename ... Tn>
    static Result static_invoke(const Method & m, const Tn & ... an) {
        Message msg(Id(Type<Component>::ID, 0)); // avoid calling ~Proxy()
        msg.method(m);
        msg.out(an ...);
        msg.act();
        return (m == SELF) ? msg.id().unit() : msg.result();
    }
};

__END_SYS

#endif
