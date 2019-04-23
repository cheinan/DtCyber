#include <string>
#include <iostream>

#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>


using namespace std::string_literals;

// TODO: Read cyber.ini file to get CM backing store path.
const auto cmPath = "/home/cheinan/PLATO/CybisRelease1/PersistStore/cmStore"s;

int main()
{
    std::cout << cmPath << std::endl;
    
    auto cyberMemFd = ::open(cmPath.c_str(), O_RDWR);
    if (cyberMemFd == -1) {
        std::cerr << "::open failed on file \"" << cmPath << "\", errno = " << errno << std::endl;
        exit(1);
    }

    auto cyberMemPtr = ::mmap(nullptr, 8388608, PROT_READ | PROT_WRITE, MAP_SHARED, cyberMemFd, 0);
    if (cyberMemPtr == MAP_FAILED) {
        std::cerr << "::mmap failed on file \"" << cmPath << "\", errno = " << errno << std::endl;
        exit(2);
    }

    // TODO: Fix this unholy HACK where the address of the EM is hard-coded.
    long* cyberBuffer = static_cast<long*>(cyberMemPtr) + 0xc32aa;
    for (ptrdiff_t offset = 0; offset < 10; offset++) {
        std::cout << std::oct << *(cyberBuffer + offset) << '\n';
    }
    std::cout << std::oct << (*cyberBuffer)++ << '\n';

    return 0;
}

