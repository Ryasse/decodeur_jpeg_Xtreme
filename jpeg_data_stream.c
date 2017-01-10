#include "jpeg_data_stream.h"

uint8_t* retrieveRawdata( uint8_t* data, int* bytesCount )
{
    int i = 0;
    int ignoreNext = 0;
    int bytesToKeep = 0;
    uint8_t* tmpData = NULL;

    while( data[ i ] != MARKER_EOI )
    {
        if( ignoreNext )
        {
            ignoreNext = 0;
            continue;
        }
        else if( data[ i ] == 0xFF )
        {
            ignoreNext = 1;
            i++;
        }
        else
            i++;
    }

    bytesToKeep = i;
    i = 0;

    tmpData = ( uint8_t* )malloc( bytesToKeep * sizeof( uint8_t ) );

    while( data[ i ] != MARKER_EOI )
    {
        if( ignoreNext )
        {
            ignoreNext = 0;
            continue;
        }

        if( data[ i ] == 0xFF )
            ignoreNext = 1;

        tmpData[ i ] = data[ i ];

        i++;
    }

    *bytesCount = i;

    return tmpData;
}
