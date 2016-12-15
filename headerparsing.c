#include "headerparsing.h"

int isFileJPEGFormat( FILE* filename )
{
    return 0;
}

uint16_t shortToInt( uint8_t msb, uint8_t lsb )
{
    return ( uint16_t )lsb | ( ( uint16_t )msb << 8 );
}

int parseHeader( FILE* file, JPEGHeader* header )
{
    int lastError;

    lastError = isFileJPEGFormat( file );
    if( lastError )
        return lastError;

    lastError = parseSOF( file, header );
    if( lastError )
        return lastError;

    lastError = parseDQT( file, header );
    if( lastError )
        return lastError;

    lastError = parseSOS( file, header );
    if( lastError )
        return lastError;

    lastError = buildHuffmanCodes( header );
    if( lastError )
        return lastError;

    lastError = parseDHT( file, header );
    if( lastError )
        return lastError;

    return NO_ERROR;
}

int parseSOF( FILE* file, JPEGHeader* header )
{
    return NO_ERROR;
}

int parseSOS( FILE* file, JPEGHeader* header )
{
    return NO_ERROR;
}

int parseDQT( FILE* file, JPEGHeader* header )
{
    return NO_ERROR;
}

int parseDHT( FILE* file, JPEGHeader* header )
{
    return NO_ERROR;
}

int buildHuffmanCodes( JPEGHeader* header )
{
    return NO_ERROR;
}
