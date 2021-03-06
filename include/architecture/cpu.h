// EPOS CPU Mediator Common Package

#ifndef __cpu_h
#define __cpu_h

#include <system/config.h>

__BEGIN_SYS

class CPU_Common
{
protected:
    static const bool BIG_ENDIAN = (Traits<CPU>::ENDIANESS == Traits<CPU>::BIG);

protected:
    CPU_Common() {}

public:
    typedef unsigned char  Reg8;
    typedef unsigned short Reg16;
    typedef unsigned long  Reg32;
    typedef unsigned long long Reg64;

    template <typename Reg>
    class Log_Addr
    {
    public:
        Log_Addr() {}
        Log_Addr(const Log_Addr & a) : _addr(a._addr) {}
        Log_Addr(const Reg & a) : _addr(a) {}
        template<typename T>
        Log_Addr(T * a) : _addr(Reg(a)) {}

        operator const Reg &() const { return _addr; }

        template<typename T>
        operator T *() const { return reinterpret_cast<T *>(_addr); }

        template<typename T>
        bool operator==(T a) const { return (_addr == Reg(a)); }
        template<typename T>
        bool operator< (T a) const { return (_addr < Reg(a)); }
        template<typename T>
        bool operator> (T a) const { return (_addr > Reg(a)); }
        template<typename T>
        bool operator>=(T a) const { return (_addr >= Reg(a)); }
        template<typename T>
        bool operator<=(T a) const { return (_addr <= Reg(a)); }

        template<typename T>
        Log_Addr operator-(T a) const { return _addr - Reg(a); }
        template<typename T>
        Log_Addr operator+(T a) const { return _addr + Reg(a); }
        template<typename T>
        Log_Addr & operator+=(T a) { _addr += Reg(a); return *this; }
        template<typename T>
        Log_Addr & operator-=(T a) { _addr -= Reg(a); return *this; }
        template<typename T>
        Log_Addr & operator&=(T a) { _addr &= Reg(a); return *this; }
        template<typename T>
        Log_Addr & operator|=(T a) { _addr |= Reg(a); return *this; }

        Log_Addr & operator[](int i) { return *(this + i); }

        friend OStream & operator<<(OStream & os, const Log_Addr & a) { os << reinterpret_cast<void *>(a._addr); return os; }

    private:
        Reg _addr;
    };

    template<typename Reg>
    using Phy_Addr = Log_Addr<Reg>;

    typedef unsigned long Hertz;

    class Context;

public:
    static unsigned int id();
    static unsigned int cores();

    static void halt() { for(;;); }

    static Hertz clock()  { return Traits<CPU>::CLOCK; }
    static void clock(const Hertz & frequency) {}
    static Hertz max_clock() { return Traits<CPU>::CLOCK; }
    static Hertz min_clock() { return Traits<CPU>::CLOCK; }

    static void fpu_save();
    static void fpu_restore();

    static bool tsl(volatile bool & lock) {
        bool old = lock;
        lock = 1;
        return old;
    }

    static int finc(volatile int & value) {
        int old = value;
        value++;
        return old;
    }

    static int fdec(volatile int & value) {
        int old = value;
        value--;
        return old;
    }

    static int cas(volatile int & value, int compare, int replacement) {
        int old = value;
        if(value == compare) {
            value = replacement;
        }
        return old;
    }

    template <int (* finc)(volatile int &)>
    static void smp_barrier(unsigned int cores, unsigned int id) {
        static volatile int ready[2];
        static volatile int i;

        int j = i;

        finc(ready[j]);
        if(id == 0) {
            while(ready[j] < int(cores));       // wait for all CPUs to be ready
            i = !i;                             // toggle ready
            ready[j] = 0;                       // signalizes waiting CPUs
        } else {
            while(ready[j]);                    // wait for CPU[0] signal
        }
    }

    static Reg64 htole64(Reg64 v) { return (BIG_ENDIAN) ? swap64(v) : v; }
    static Reg32 htole32(Reg32 v) { return (BIG_ENDIAN) ? swap32(v) : v; }
    static Reg16 htole16(Reg16 v) { return (BIG_ENDIAN) ? swap16(v) : v; }
    static Reg64 letoh64(Reg64 v) { return htole64(v); }
    static Reg32 letoh32(Reg32 v) { return htole32(v); }
    static Reg16 letoh16(Reg16 v) { return htole16(v); }

    static Reg64 htobe64(Reg64 v) { return (!BIG_ENDIAN) ? swap64(v) : v; }
    static Reg32 htobe32(Reg32 v) { return (!BIG_ENDIAN) ? swap32(v) : v; }
    static Reg16 htobe16(Reg16 v) { return (!BIG_ENDIAN) ? swap16(v) : v; }
    static Reg64 betoh64(Reg64 v) { return htobe64(v); }
    static Reg32 betoh32(Reg32 v) { return htobe32(v); }
    static Reg16 betoh16(Reg16 v) { return htobe16(v); }

    static Reg32 htonl(Reg32 v) { return (BIG_ENDIAN) ? v : swap32(v); }
    static Reg16 htons(Reg16 v) { return (BIG_ENDIAN) ? v : swap16(v); }
    static Reg32 ntohl(Reg32 v) { return htonl(v); }
    static Reg16 ntohs(Reg16 v) { return htons(v); }

protected:
    static Reg64 swap64(Reg64 v) { return
        ((v & 0xff00000000000000ULL) >> 56) |
        ((v & 0x00ff000000000000ULL) >> 40) |
        ((v & 0x0000ff0000000000ULL) >> 24) |
        ((v & 0x000000ff00000000uLL) >> 8)  |
        ((v & 0x00000000ff000000ULL) << 8)  |
        ((v & 0x0000000000ff0000ULL) << 24) |
        ((v & 0x000000000000ff00ULL) << 40) |
        ((v & 0x00000000000000ffULL) << 56); }
    static Reg32 swap32(Reg32 v) { return
        ((v & 0xff000000) >> 24) |
        ((v & 0x00ff0000) >> 8) |
        ((v & 0x0000ff00) << 8) |
        ((v & 0x000000ff) << 24); }
    static Reg16 swap16(Reg16 v) { return
        ((v & 0xff00) >> 8) |
        ((v & 0x00ff) << 8); }
};

template<typename T>
inline T align32(const T & addr) { return (addr + 3) & ~3U; }
template<typename T>
inline T align64(const T & addr) { return (addr + 7) & ~7U; }
template<typename T>
inline T align128(const T & addr) { return (addr + 15) & ~15U; }

__END_SYS

#endif

#if defined(__CPU_H) && !defined(__cpu_common_only__)
#include __CPU_H
#endif
