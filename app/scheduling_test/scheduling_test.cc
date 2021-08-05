#include <machine/display.h>
#include <time.h>
#include <real-time.h>

using namespace EPOS;

OStream cout;
Chronometer chrono;

Periodic_Thread * t1;
Periodic_Thread * t2;
Periodic_Thread * t3;

int n_exec_times_t1 = 0;
int n_exec_times_t2 = 0;
int n_exec_times_t3 = 0;

int execution_times = 25;


// Incrementa a contagem de execuções de cada thread que passar por esta função, e imprime as prioridades de cada thread
void some_workload(int thread_identifier) {
    Microsecond current_time = chrono.read() / 1000;

    switch (thread_identifier) {
        case 1:
            n_exec_times_t1++;
            break;
        case 2:
            n_exec_times_t2++;
            break;
        case 3:
            n_exec_times_t3++;
            break;
    }

    cout << "\n"<< current_time << " ms\t"
         << "Running T" << thread_identifier << " | "
         << "Priority: "
         << "T1=" << t1->priority() << ", "
         << "T2=" << t2->priority() << ", "
         << "T3=" << t3->priority();
}

int handler_t1() {
    do {
        some_workload(1);
    } while (Periodic_Thread::wait_next());
    return 0;
}

int handler_t2() {
    do {
        some_workload(2);
    } while (Periodic_Thread::wait_next());
    return 0;
}

int handler_t3() {
    do {
        some_workload(3);
    } while (Periodic_Thread::wait_next());
    return 0;
}

int main() {
    t1 = new Periodic_Thread(RTConf(150 * 1000, 0, 0, 0, execution_times), &handler_t1);
    t2 = new Periodic_Thread(RTConf(100 * 1000, 0, 0, 0, execution_times), &handler_t2);
    t3 = new Periodic_Thread(RTConf(50 * 1000, 0, 0, 0, execution_times), &handler_t3);

    chrono.start();

    t1->join();
    t2->join();
    t3->join();

    chrono.stop();

    return 0;
}