// EPOS ARMv7 Time-Stamp Counter Mediator Declarations

#ifndef __armv7_tsc_h
#define __armv7_tsc_h

#include <architecture/cpu.h>
#include <architecture/tsc.h>

#define __ic_common_only__
#include <machine/ic.h>
#undef __ic_common_only__

__BEGIN_SYS

class TSC: private TSC_Common
{
    friend class CPU;
    friend class IC;

private:
    static const Hertz CLOCK = Traits<CPU>::CLOCK / (Traits<Build>::MODEL == Traits<Build>::Zynq ? 2 : 1);
    static const PPB ACCURACY = 40000; // ppb

    enum {
        TSC_BASE = Traits<Build>::MODEL == Traits<Build>::eMote3  ?      0x40033000 // TIMER3_BASE
                 : Traits<Build>::MODEL == Traits<Build>::LM3S811 ?      0x40031000 // TIMER1_BASE
                 : Traits<Build>::MODEL == Traits<Build>::Zynq ?         0xf8f00200 // GLOBAL_TIMER_BASE
                 : Traits<Build>::MODEL == Traits<Build>::Realview_PBX ? 0x1f000200 // GLOBAL_TIMER_BASE
                 : Traits<Build>::MODEL == Traits<Build>::Raspberry_Pi3 ? 0x3f003000
                 : 0
    };

    // Cortex-M3 GPTM registers offsets
    enum {              // Description
        GPTMTAR = 0x48, // Counter
    };

    // Zynq Global Timer Registers offsets
    enum {             // Description
        GTCTRL = 0x00, // Low Counter
        GTCTRH = 0x04, // High Counter
        GTCLR  = 0x08, // Control
        GTISR  = 0x0C  // Interrupt Status
    };

    // BCM2835 cortex-a53 System Timer
    enum {                                      // Description
        STCS                        = 0x00,     // Control/Status
        STCLO                       = 0x04,     // Low COUNTER
        STCHI                       = 0x08,     // High Counter
        STC0                        = 0x0C,     // Compare 0 - Used by GPU
        STC1                        = 0x10,     // Compare 1 - Value used to generate interrupt 1
        STC2                        = 0X14,     // Compare 2 - Used by GPU
        STC3                        = 0X18      // Compare 3 - Value used to generate interrupt 3
        // Interrupts mapped to "Enable IRQ 1" - c1 and c3 == irq1 and irq3
    };

public:
    using TSC_Common::Time_Stamp;

    static const unsigned int FREQUENCY = CLOCK;

public:
    TSC() {}

    static Hertz frequency() { return CLOCK; }
    static PPB accuracy() { return ACCURACY; }

    static Time_Stamp time_stamp() {

#ifdef __cortex_a__

#if defined(__mmod_raspberry_pi3__)
        return reg(STCLO);
#else
        if(sizeof(Time_Stamp) == sizeof(CPU::Reg32))
            return reg(GTCTRL);

        Time_Stamp high;
        CPU::Reg32 low;

        do {
            high = reg(GTCTRH);
            low = reg(GTCTRL);
        } while(reg(GTCTRH) != high);

        return (high << 32) | low;

#endif

#endif
#ifdef __cortex_m__

        return (_overflow << 32) + reg(GPTMTAR); // Not supported by LM3S811 on QEMU (version 2.7.50)

#endif

    }

private:
    static void init();

    static volatile CPU::Reg32 & reg(unsigned int o) { return reinterpret_cast<volatile CPU::Reg32 *>(TSC_BASE)[o / sizeof(CPU::Reg32)]; }

#if defined(__mmod_emote3__) || defined(__mmod_lm3s811__)

    static void int_handler(IC_Common::Interrupt_Id int_id) { _overflow++; }

    static volatile Time_Stamp _overflow;

#endif

};

__END_SYS

#endif
