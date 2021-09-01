#include <process.h>

#include <utility/ostream.h>

using namespace EPOS;

OStream cout;

Thread * m;

int world()
{
    cout << "world!" << endl;
    return 0;
}

int main()
{
    cout << "Hello ";
    m = new Thread(&world);

    return 0;
}
