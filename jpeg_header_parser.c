#include "jpeg_header_parser.h"

int shortToInt(uint8_t lsb,uint8_t msb)
{
    return lsb | ( int )( msb << 8 );
}

int parsingSOF( uint8_t* data, pJPEGDATA jpegData )
{
    uint16_t SOFIndicator = shortToInt( data[ 0 ], data[ 1 ] );

    if( SOFIndicator != 0xFFC0 )
    {
        printf( "Erreur : type de SOF non pris en charge (0x%X)\n", SOFIndicator );
        return ERROR_SOI;
    }

    uint16_t sectionSize = shortToInt( data[ 2 ], data[ 3 ] );

    ///TODO : verifier si structure sof deja allouee
    jpegData->psof = ( pSOF )malloc( sizeof( SOF ) );
    pSOF psof = jpegData->psof;

    psof->precision = data[ 4 ];

    psof->height = shortToInt( data[ 5 ], data[ 6 ] );
    psof->width = shortToInt( data[ 7 ], data[ 8 ] );

    psof->nbComponents = data[ 9 ];

    psof->components = ( pCOMPONENT )malloc( sizeof( COMPONENT ) * psof->nbComponents );

    int i;
    int currentByte = 10;
    for( i = 0; i < psof->nbComponents; i++ )
    {
        psof->components[ i ].previousDC = 0;
        psof->components[ i ].id = data[ currentByte++ ];
        psof->components[ i ].horizontal_sample_factor = ( data[ currentByte ] & 0xF0 ) >> 4;
        psof->components[ i ].vertical_sample_factor = ( data[ currentByte++ ] & 0x0F );
        psof->components[ i ].idQ = data[ currentByte++ ];
    }

    return NO_ERROR;
}

int parsingSOS( uint8_t* data, pJPEGDATA jpegData )
{
    uint16_t SOSIndicator = shortToInt( data[ 0 ], data[ 1 ] );

    if( SOSIndicator != 0xFFDA )
    {
        printf( "Erreur : type de SOS non pris en charge (0x%X)\n", SOSIndicator );
        return ERROR_SOI;
    }

    uint16_t sectionSize = shortToInt( data[ 2 ], data[ 3 ] );

    uint8_t nbComponents = data[ 4 ];

    pSOF psof = jpegData->psof;

    if( psof->nbComponents != nbComponents )
    {
        printf( "Erreur : nombre de composantes different entre SOF (%d) et SOS (%d)\n",
                psof->nbComponents,
                nbComponents );

        return ERROR_SOI;
    }

    int i;
    int currentByte = 5;
    for( i = 0; i < nbComponents; i++ )
    {
        //Relecture de l'id de la composante inutile
        //psof->components[ i ].id = data[ currentByte ];
        currentByte++;

        psof->components[ i ].idDC = ( data[ currentByte ] & 0xF0 ) >> 4;
        psof->components[ i ].idAC = ( data[ currentByte++ ] & 0x0F );
    }

    //Ici 3 octets inutilises
    currentByte += 3;

    return NO_ERROR;
}

int buildHuffmanCodes( pDHT pdht )
{
    uint16_t DHTIndicator = shortToInt( data[ 0 ], data[ 1 ] );

    if( DHTIndicator != 0xFFC4 )
    {
        printf( "Erreur : identificateur DHT invalide (0x%X)\n", SOSIndicator );
        return ERROR_DHT;
    }

    uint16_t sectionSize = shortToInt( data[ 2 ], data[ 3 ] );

    if( ( data[ 4 ] & 0b11100000 ) != 0 )//3 birs msb doivent etre a 0
    {
        printf( "Erreur DHT : 3 bits msb pas a 0 (0x%X)\n", data[ 4 ] & 0b11100000 );
        return ERROR_DHT;
    }

    pdht->type = ( data[ 4 ] & 0b00010000 ) >> 4;


    return NO_ERROR;
}
