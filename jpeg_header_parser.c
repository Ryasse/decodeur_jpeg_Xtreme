#include "jpeg_header_parser.h"

int shortToInt(uint8_t lsb,uint8_t msb)
{
    return lsb | ( int )( msb << 8 );
}

int parsingSOF( uint8_t* data, pJPEGDATA jpegData )
{
    uint16_t SOFIndicator = shortToInt( data[0], data[1] );

    if( SOFIndicator != 0xFFC0 )
    {
        printf( "Erreur : type de SOF non pris en charge (0x%X)\n", SOFIndicator );
    }

    uint16_t sofSize = shortToInt( data[2], data[3] );

    jpegData->psof = ( pSOF )malloc( sizeof( SOF ) );
    pSOF psof = jpegData->psof;

    psof->precision = data[4];

    psof->height = shortToInt( data[5], data[6] );
    psof->width = shortToInt( data[7], data[8] );



    return 0;
}
