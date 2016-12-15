#include "file_io.h"

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

void imageWritePPM( const char* filename, int width, int height, uint8_t* r, uint8_t* g, uint8_t* b )
{
    FILE* stream = fopen( filename, "w" );

    fprintf( stream, "P3\n%d %d\n", width, height );

    int x, y;
    for( y = 0; y < height; y++ )
    {
        for( x = 0; x < width; x++ )
            fprintf( stream, "%d\t%d\t%d\t\t", r[ y * width + x ], g[ y * width + x ], b[ y * width + x ] );

        fprintf( stream, "\n" );
    }

    fclose( stream );
}
