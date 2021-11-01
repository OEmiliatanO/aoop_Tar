#ifndef __MYTAR_H_
#define __MYTAR_H_
#include <vector>
#include <fstream>
#include <string>

using std::vector;
using std::fstream;
using std::string;

typedef struct TarHeader
{
    // Header
    char filename[100];
    char filemode[8];
    char userid[8];
    char groupid[8];
    char filesize[12];
    char mtime[12];
    char checksum[8];
    char type;
    char lname[100];

    /* USTAR Section */
    char USTAR_id[6];
    char USTAR_ver[2];
    char username[32];
    char groupname[32];
    char devmajor[8];
    char devminor[8];
    char prefix[155];
    char pad[12];
}TarHeader;


class tarRder
{
public:
    tarRder();
    tarRder(string file);
    ~tarRder();

    auto clear() -> void;
    auto readTAR() -> int;
    auto info() -> void;

    static constexpr int __USTAR_BLOCK = 512;
    static constexpr bool __NOT_USTAR = false;
    static constexpr bool __USTAR = true;

private:
    string file;
    vector<TarHeader> THrs;
    fstream fs;
    int filesz;
    int filenum;

    auto getfilesz() -> void;
    auto isUSTAR() -> bool;
    auto flnum() -> int;
};
#endif