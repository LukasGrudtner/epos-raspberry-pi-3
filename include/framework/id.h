#ifndef __id_h
#define __id_h

__BEGIN_SYS

class Pointer_Id {
public:
    typedef unsigned int Type_Id;
    typedef unsigned int Unit_Id;

public:
    Pointer_Id(const Type_Id & t, const Unit_Id & u): _type(t), _unit(u) {}

    const Type_Id & type() const { return _type; }
    const Unit_Id & unit() const { return _unit; }

    friend Debug & operator << (Debug & db, const Pointer_Id & id) {
        db << "{t=" << id.type() << ",u=" << reinterpret_cast<void *>(id.unit()) << "}";
        return db;
    }

private:
    Type_Id _type;
    Unit_Id _unit;
};

typedef Pointer_Id Id;

__END_SYS

#endif