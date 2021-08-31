#include <machine.h>
#include <process.h>
#include <system.h>

#include <framework/message.h>

// Bindings
extern "C" {
    __USING_SYS;

    // Libc legacy
    void _panic() { Machine::panic(); }
    void _exit(int s) { Thread::exit(s); for(;;); }
    void __exit() { Thread::exit(CPU::fr()); }  // must be handled by the Page Fault handler for user-level tasks
    void __cxa_pure_virtual() { db<void>(ERR) << "Pure Virtual method called!" << endl; }

    // Utility-related methods that differ from kernel and user space.
    // OStream
    void _print(const char * s) {
        Message msg(Id(UTILITY_ID, 0), Message::PRINT, reinterpret_cast<unsigned int>(s));
        msg.act();
    }

    void _syscall(void * m) { CPU::syscall(m); }
}
