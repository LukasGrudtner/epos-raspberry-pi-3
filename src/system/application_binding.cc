#include <system.h>
#include <framework/main.h>
#include <framework/message.h>

__BEGIN_SYS
Framework::Cache Framework::_cache;
__END_SYS

__BEGIN_SYS
OStream kerr;
__END_SYS

extern "C" {
    void _panic() { _API::Thread::exit(-1); }
    void _exit(int s) { _API::Thread::exit(s); for(;;); }
}

__USING_SYS;
extern "C" {
    void _syscall(void * m) { CPU::syscall(m); }

    void _print(const char * s) {
        Message msg(Id(UTILITY_ID, 0), Message::PRINT, reinterpret_cast<unsigned int>(s));
        msg.act();
    }

    void _print_preamble() {}
    void _print_trailler(bool error) {}
}
