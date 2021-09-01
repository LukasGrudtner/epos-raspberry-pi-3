// EPOS ARMv7 CPU System Call Entry Implementation

#include <architecture/armv7/armv7_cpu.h>

extern "C" { void _exec(void * m); }

__BEGIN_SYS

void CPU::syscalled()
{
    _exec(reinterpret_cast<void *>(CPU::r0()));
}

__END_SYS
