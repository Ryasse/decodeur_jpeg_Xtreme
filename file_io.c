#include "file_io.h"
#include <assert.h>

uint8_t* fileRead( const char* filename, int* size )
{
    FILE* stream = fopen( filename, "rb" );

    fseek( stream, 0, SEEK_END );

    int fileLength = ftell( stream );

    rewind( stream );

    uint8_t* data = ( uint8_t* )malloc( fileLength );

    *size = fread( data, 1, fileLength, stream );

    fclose( stream );

    return data;
}
