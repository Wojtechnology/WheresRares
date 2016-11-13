#include <string>

typedef void (*FuncPtr)( const std::string );

extern "C" {
    void SetDebugFunction( FuncPtr fp );
    void InitCameraPlugin(int height, int width);
    bool DetectPerson(unsigned char* map, int height, int width);
    int* Track(unsigned char* map, int height, int width);
}
