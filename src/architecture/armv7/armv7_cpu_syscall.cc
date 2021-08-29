// EPOS ARMv7 CPU System Call Function Implementation

#include <architecture/armv7/armv7_cpu.h>
#include <machine/ic.h>

__BEGIN_SYS

void CPU::syscall(void * msg)
{
    ASM("SVC    0x0");
}

__END_SYS
