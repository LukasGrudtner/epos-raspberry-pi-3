// EPOS Task Implementation

#include <process.h>

__BEGIN_SYS

// Class attributes
Task * volatile Task::_current;
unsigned int Task::_next_id = 0;

// Methods
Task::~Task()
{
    db<Task>(TRC) << "~Task(this=" << this << ")" << endl;

    while(!_threads.empty())
        delete _threads.remove()->object();
}

__END_SYS
