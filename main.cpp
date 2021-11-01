#include <iostream>
#include <string>
#include "src/mytar.h"

using namespace std;

int main(int argc, char *argv[])
{
    if(argc != 2)
    {
        cerr << argv[0] << ": wrong parameters number\n";
        cerr << argv[0] << ": Error is not recoverable: exiting now\n";
        return 0;
    }

    tarRder tr(string{argv[1]});
    if (tr.readTAR() == tarRder::__NOT_USTAR)
        return 0;

    tr.info();

    return 0;
}