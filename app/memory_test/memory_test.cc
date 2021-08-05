#include <machine/display.h>

using namespace EPOS;

OStream cout;

int main()
{
    cout << endl << "Alloc new char[100]" << endl;
    new char[100];

    cout << endl << "Alloc new char[100]" << endl;
    new char[100];

    cout << endl << "Alloc new char[100]" << endl;
    new char[100];

    return 0;
}