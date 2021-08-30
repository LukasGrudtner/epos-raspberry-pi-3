#include <utility/ostream.h>
#include <process.h>
#include <time.h>

using namespace EPOS;

OStream cout;

Thread * m;

int print_info()
{
    Task * task = Task::self();
    Address_Space * as = task->address_space();

    CPU::Log_Addr code = task->code();
    CPU::Log_Addr data = task->data();

    cout << "Task_" << task->id() << "[" << endl
         << "  thread=" << Thread::self() << endl
         << "  pd=" << as->pd() << endl
         << "  cs_log=" << code << ", cs_phy=" << as->physical(code) << "," << endl
         << "  ds_log=" << data << ", ds_phy=" << as->physical(data) << endl
         << "]\n" << endl;

    return 0;
}

void create_task()
{
    Task * task = Task::self();
    Address_Space * as = task->address_space();

    Segment * cs = new (SYSTEM) Segment(task->code_segment()->size());
    cout << "Created code segment " << cs << " (" << cs->size() << " bytes)" << endl;

    auto cs_log = as->attach(cs);
    cout << "Attached code segment " << cs << " to address space " << as << " => " << cs_log << endl;

    memcpy(cs_log, task->code(), cs->size());
    cout << "Copied " << cs->size() << " bytes from code segment " << task->code() << " to " << cs_log << endl;

    as->detach(cs_log);
    cout << "Detached " << cs_log << " from address space " << as << endl;



    Segment * ds = new (SYSTEM) Segment(task->data_segment()->size());
    cout << "Created data segment " << ds << " (" << ds->size() << " bytes)" << endl;

    auto ds_log = as->attach(ds);
    cout << "Attached data segment " << ds << " to address space " << as << " => " << ds_log << endl;

    memcpy(ds_log, task->data(), ds->size());
    cout << "Copied " << ds->size() << " bytes from data segment " << task->data() << " to " << ds_log << endl;

    as->detach(ds_log);
    cout << "Detached " << ds_log << " from address space " << as << endl;



    MMU::flush_tlb();
    cout << "Flushed TLB" << endl;

    // This couts need to happen before becasuse the task starts running immediately.
    cout << "Created task\n" << endl;
    cout << "--------------------------------------------------------------------------------" << endl;
    cout << "Printing info from the main thread of the new Task created" << endl;
    cout << "--------------------------------------------------------------------------------" << endl;
    new (SYSTEM) Task(cs, ds, &print_info);
}

int main()
{
    cout << "--------------------------------------------------------------------------------" << endl;
    cout << "Printing info from the main thread of the first Task created" << endl;
    cout << "--------------------------------------------------------------------------------" << endl;
    print_info();

    cout << "--------------------------------------------------------------------------------" << endl;
    cout << "Creating another thread in the first Task" << endl;
    cout << "--------------------------------------------------------------------------------" << endl;
    m = new (SYSTEM) Thread(&print_info);

    Alarm::delay(1000000);
    m->join();

    cout << "--------------------------------------------------------------------------------" << endl;
    cout << "Creating another task" << endl;
    cout << "--------------------------------------------------------------------------------" << endl;
    create_task();

    return 0;
}
