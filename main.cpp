#include <iostream>
#include <string>
#include "src/mytar.h"

using namespace std;

int main(int argc, char *argv[])
{
    if(argc != 2)
    {
        cerr << argv[0] << ": wrong parameters number\n";
        cerr << argv[0] << ": Error is not recoverable: exiting now...\n";
        return 0;
    }

    tarRder tr(string{argv[1]});
	int FLAG = tr.readTAR();
	
    if (FLAG == tarRder::__NOT_USTAR)
	{
		cerr << argv[0] << ": Cannot open: Not USTAR format\n";
		cerr << argv[0] << ": Error is not recoverable: exiting now...\n";
		return 0;
	}
	else if (FLAG == tarRder::__FILE_ERROR)
	{
		cerr << argv[0] << ": Cannot open: No such file\n";
		cerr << argv[0] << ": Error is not recoverable: exiting now...\n";
		return 0;
	}
	else if (FLAG == tarRder::__UNKNOWN_ERROR)
	{
		cerr << argv[0] << ": Unknown error occurs\n";
		cerr << argv[0] << ": Error is not recoverable: exiting now...\n";
		return 0;
	}
	
    tr.info();

    return 0;
}