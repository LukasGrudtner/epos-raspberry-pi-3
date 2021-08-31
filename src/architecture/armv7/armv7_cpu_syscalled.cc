// EPOS ARMv7 CPU System Call Entry Implementation

#include <architecture/armv7/armv7_cpu.h>

// FILIPE: remove later
#include<framework/message.h>
#include <machine.h>

__BEGIN_SYS

void CPU::syscalled()
{
    // Do the system call by calling _exec with the message pointed by ecx
    // ASM("PUSH   {R0}    \n"
    //     "BL     _exec   \n"
    //     "POP    {R0}    \n");

    // Return to user-level
    // ASM("LDMFD  sp!, {R0-R12,pc}^   \n");

    auto msg = reinterpret_cast<Message *>(CPU::r0());

    const char * s;
    msg->in(s);
    Display::puts(s);
}

__END_SYS
