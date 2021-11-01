#include "mytar.h"
#include "tarform.h"
#include <iostream>
#include <fstream>
#include <cmath>
#include <cstring>
#include <iomanip>

using std::setw;
using std::setfill;
using std::ios;
using std::fstream;
using std::cerr;
using std::cout;

// default constructor
tarRder::tarRder() {}

// custom constructor
tarRder::tarRder(string file)
{
    this->file = file;
}

// custom destructor
tarRder::~tarRder()
{
    this->THrs.reserve(0);
    this->file.clear();
    if (this->fs.is_open()) this->fs.close();
}

// clear for reuse
auto tarRder::clear() -> void
{
    this->THrs.clear();
    this->file.clear();
    if (this->fs.is_open()) this->fs.close();
    this->filesz = 0;
}

// set member: filesz
auto tarRder::setfilesz() -> void
{
    fs.seekg(0, ios::end);
    filesz = fs.tellg();
    fs.seekg(0, ios::beg);
}

// judge if it USTAR format
auto tarRder::isUSTAR() -> int
{
	// if its size can't be divided by __USTAR_BLOCK, it's not USTAR format.
    if (filesz % tarRder::__USTAR_BLOCK) return tarRder::__NOT_USTAR;
	
    char buffer[6];
	
	// "ustar " shall start at 257-th byte.
    fs.seekg(257, ios::beg);
    fs.read(buffer, 6);
    fs.seekg(0, ios::beg);

    if (strcmp(buffer, "ustar ") == 0)
        return tarRder::__USTAR;
    return tarRder::__NOT_USTAR;
}

// read .tar file and push the information into a vector, THrs.
auto tarRder::readTAR() -> int
{
    if (this->fs.is_open()) this->fs.close();
    fs.open(file, ios::in | ios::binary);
	// can't open this file, or directory.
    if (!fs.is_open()) { return tarRder::__FILE_ERROR; }

    setfilesz();
	
	// judge if this file a USTAR
    if (isUSTAR() == tarRder::__NOT_USTAR) { return tarRder::__NOT_USTAR; }

    while(fs.good())
    {
        TarHeader thr;
        int64_t blocksize = -1;
        if(fs.read((char *)&thr, tarRder::__USTAR_BLOCK))
        {
            if (thr.filename[0] == 0) break;
            THrs.emplace_back(thr);
            blocksize = otod(atoll(thr.filesize));
            int movblk = ceil((double)blocksize / tarRder::__USTAR_BLOCK);
			
			// move forward, skip this block
            fs.seekg(movblk * tarRder::__USTAR_BLOCK, ios::cur);
        }
    }

    return (THrs.size() > 0 ? tarRder::__USTAR : tarRder::__NOT_USTAR);
}

// show information in this USTAR header
auto tarRder::info() -> void
{
    for (const auto& it : THrs)
    {
		// [type][filemode] [username]/[groupname] [filesize] [year]-[month]-[day] [hour]:[minute] [filename]
		
        cout << gettype(it.type) << getmode(it.filemode) << ' ';
        cout << it.username << '/' << it.groupname << ' ';
        cout << setfill(' ') << setw(6) << otod(atoi(it.filesize)) << ' ';
		
		int64_t time, sec, min, hr, mon, yr, leap;
        gettime(it.mtime, time, sec, min, hr, mon, yr, leap);
		
		// time format
		cout << yr << '-' << setfill('0') << setw(2) << mon << '-' << setfill('0') << setw(2) << time << ' ';
		cout << setfill('0') << setw(2) << hr << ':' << setfill('0') << setw(2) << min << ' ';
		
        cout << it.filename << '\n';
    }
}

