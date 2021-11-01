#ifndef __TARFORM_H_
#define __TARFORM_H_
#include <string>
using std::string;

// oct to dex
auto otod(int64_t n) -> int64_t
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

// mode transformation
auto getmode(const char* mode) -> string
{
    string s;
    const string MODE[] = {"---", "--x", "-w-", "-wx", "r--", "r-x", "rw-", "rwx"};
    for (int i = 4; i <= 6; ++i)
        s += MODE[mode[i] - '0'];
    return s;
}

// type transformation
auto gettype(const char &type) -> char
{
    const char TYPE[] = {'-', ' ', 'l', ' ', ' ', 'd'};
    if (type >= '0' && type <= '5')
        return TYPE[type - '0'];
    return TYPE[0];
}

// time transformation
auto gettime(const char *ttime, int64_t &time, int64_t &sec, int64_t &min, int64_t &hr, int64_t &mon, int64_t &yr, int64_t &leap) -> void
{
    const int Month[] = {0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    
    time = otod(atoll(ttime)) - 3600 * 7;

    sec = time % 60;
    time /= 60;
    min = time % 60;
    time /= 60;
    hr = time % 24;
    time /= 24;

    if (sec != 0 && min != 0 && hr != 0) ++time;
    time -= 14245;

    yr = 2009;
    mon = 1;
    leap = 0;


    for (; time >= 1461; time -= 1461) yr += 4;
    for (; time >= 366; time -= 365) ++yr, ++leap;
    for (; mon <= 12; ++mon)
        if (time >= 29 && leap == 3)
            time -= 29;
        else if (time > Month[mon])
            time -= Month[mon];
        else
            break;
}

#endif