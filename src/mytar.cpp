#include "mytar.h"
#include <iostream>
#include <fstream>
#include <cmath>
#include <cstring>
#include <array>
#include <iomanip>

using std::setw;
using std::setfill;
using std::ios;
using std::fstream;
using std::cerr;
using std::cout;
using std::array;

tarRder::tarRder() {}

tarRder::tarRder(string file)
{
    this->file = file;
    filenum = -1;
}

tarRder::~tarRder()
{
    this->THrs.reserve(0);
    this->file.clear();
    if (this->fs.is_open())
        this->fs.close();
}

auto tarRder::clear() -> void
{
    this->THrs.clear();
    this->file.clear();
    if (this->fs.is_open())
        this->fs.close();
    this->filesz = 0;
    this->filenum = -1;
}

auto tarRder::getfilesz() -> void
{
    fs.seekg(0, ios::end);
    filesz = fs.tellg();
    fs.seekg(0, ios::beg);
}

auto tarRder::isUSTAR() -> bool
{
    if (filesz % tarRder::__USTAR_BLOCK) return tarRder::__NOT_USTAR;
    char buffer[6];

    fs.seekg(257, ios::beg);
    fs.read(buffer, 6);
    fs.seekg(0, ios::beg);

    if (strcmp(buffer, "ustar ") == 0)
        return tarRder::__USTAR;
    return tarRder::__NOT_USTAR;
}

auto tH_otod(int64_t n) -> int64_t
{
    int64_t res = 0;
    int64_t i = 0;
    while(n)
    {
        res += (n % 10LL) * (1LL << 3LL*i);
        n /= 10LL;
        ++i;
    }
    return res;
}

auto tarRder::readTAR() -> int
{
    if (this->fs.is_open())
        this->fs.close();
    fs.open(file, ios::in | ios::binary);
    if (!fs.is_open()) { cerr << "Cannot open: No such file\n"; return tarRder::__NOT_USTAR; }

    getfilesz();

    if (isUSTAR() == tarRder::__NOT_USTAR) { cerr << "It's not USTAR format\n"; return tarRder::__NOT_USTAR; }

    while(fs.good())
    {
        TarHeader thr;
        int64_t blocksize = -1;
        if(fs.read((char *)&thr, tarRder::__USTAR_BLOCK))
        {
            if (thr.filename[0] == 0) break;
            THrs.emplace_back(thr);
            blocksize = tH_otod(atoll(thr.filesize));
            int movblk = ceil((double)blocksize/tarRder::__USTAR_BLOCK);
            fs.seekg(movblk * tarRder::__USTAR_BLOCK, ios::cur);
        }
    }

    return (THrs.size() > 0 ? tarRder::__USTAR : tarRder::__NOT_USTAR);
}

auto getmode(const char* mode) -> string
{
    string s;
    const string MODE[] = {"---", "--x", "-w-", "-wx", "r--", "r-x", "rw-", "rwx"};
    for (int i = 4; i <= 6; ++i)
        s += MODE[mode[i] - '0'];
    return s;
}
auto gettype(const char &type) -> char
{
    const char TYPE[] = {'-', ' ', 'l', ' ', ' ', 'd'};
    if (type >= '0' && type <= '5')
        return TYPE[type - '0'];
    return TYPE[0];
}

auto printtime(const char *ttime) -> void
{
    const int Month[] = {0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    
    int64_t time = tH_otod(atoll(ttime)) - 3600 * 7;

    int64_t sec = time % 60;
    time /= 60;
    int64_t min = time % 60;
    time /= 60;
    int64_t hr = time % 24;
    time /= 24;

    if (sec != 0 && min != 0 && hr != 0) ++time;
    time -= 14245;

    int yr = 2009;
    int mon = 1;
    int leap = 0;


    for ( ; time >= 1461; time -= 1461) yr += 4;
    for ( ; time >= 366; time -= 365) ++yr, ++leap;
    for ( ; mon <= 12; ++mon)
        if (time >= 29 && leap == 3)
            time -= 29;
        else if (time > Month[mon])
            time -= Month[mon];
        else
            break;

    cout << yr << '-' << setfill('0') << setw(2) << mon << '-' << setfill('0') << setw(2) << time << ' ';
    cout << setfill('0') << setw(2) << hr << ':' << setfill('0') << setw(2) << min << ' ';
}

auto tarRder::info() -> void
{
    for (const auto& it : THrs)
    {
        cout << gettype(it.type) << getmode(it.filemode) << ' ';
        cout << it.username << '/' << it.groupname << ' ';
        cout << setfill(' ') << setw(6) << tH_otod(atoi(it.filesize)) << ' ';
        printtime(it.mtime);
        cout << it.filename << '\n';
    }
}

