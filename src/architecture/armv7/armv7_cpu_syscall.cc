// EPOS ARMv7 CPU System Call Function Implementation

#include <architecture/armv7/armv7_cpu.h>
#include <machine/ic.h>

#include <machine.h>

__BEGIN_SYS

void CPU::syscall(void * msg)
{
    CPU::r0(Reg32(msg));

    ASM("PUSH   {lr}");
    ASM("SVC    0x0");
    ASM("POP    {lr}");
}

__END_SYS
