#include "jpeg_header_parser.h"

int shortToInt(uint8_t lsb,uint8_t msb)
{
    return lsb | ( int )( msb << 8 );
}

int isFileJPEGFormat(uint8_t* tab_jpeg)
{
    uint8_t data1,data2;
    int test;
    data1 = tab_jpeg[0];
    data2 = tab_jpeg[1];
    if (data1==0xDD && data2==0xD8)
    {
        test = 1;
    }else{test = 0;}
    return test;
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

int parsingDHT( uint8_t* data, pJPEGDATA jpegData )
{
    pDHT pdht = jpegData->dhtHead;

    uint16_t DHTIndicator = shortToInt( data[ 0 ], data[ 1 ] );

    if( DHTIndicator != 0xFFC4 )
    {
        printf( "Erreur : identificateur DHT invalide (0x%X)\n", DHTIndicator );
        return ERROR_DHT;
    }

    uint16_t sectionSize = shortToInt( data[ 2 ], data[ 3 ] );

    if( ( data[ 4 ] & 0b11100000 ) != 0 )//3 birs msb doivent etre a 0
    {
        printf( "Erreur DHT : 3 bits msb pas a 0 (0x%X)\n", data[ 4 ] & 0b11100000 );
        return ERROR_DHT;
    }

    pdht->type = ( data[ 4 ] & 0b00010000 ) >> 4;
    pdht->id = ( data[ 4 ] & 0b00001111 );

    //Lecture des codes
    uint32_t currentIndex = 5;
    int i,j;

    pdht->countSymbols = 0;

    for( i = 0; i < 16; i++ )
    {
        pdht->nbSymbols[ i ] = data[ currentIndex++ ];
        pdht->countSymbols += pdht->nbSymbols[ i ];
    }

    if( pdht->countSymbols >= 256 || pdht->countSymbols < 0 )
    {
        printf( "Erreur DHT : nombre total de symboles superieur a 256 ou negatif (%d)\n", pdht->countSymbols );
        return ERROR_DHT;
    }

    pdht->codes = ( HuffmanCode* )malloc( pdht->countSymbols * sizeof( HuffmanCode ) );

    int nbbits = 1;

    for( i = 0, j = 0; i < pdht->countSymbols; i++ )
    {
        ///incrementation de la taille du code des qu'on a fait tous les codes d'une taille donnee
        while( pdht->nbSymbols[ nbbits - 1 ] <= j )
        {
            j = 0;
            nbbits++;
        }

        pdht->codes[ i ].nbbits = nbbits;
        pdht->codes[ i ].value = shortToInt( data[ currentIndex ], data[ currentIndex + 1 ] );

        currentIndex++;

        j++;///1 code lu de plus de la taille actuelle
    }

    return NO_ERROR;
}

int parsingDQT( uint8_t* data, pJPEGDATA jpegData )
{
    uint16_t DQTIndicator = shortToInt( data[0], data[1] );
    if( DQTIndicator != 0xFFDB )
    {
        printf( "Erreur : type de SOF non pris en charge (0x%X)\n", DQTIndicator );
        return ERROR_DQT;
    }
    jpegData->dqtHead = ( pDQT )malloc( sizeof( DQT ) );
    pDQT pdqt = jpegData->dqtHead;
    pdqt->precision = shortToInt( data[2] , data[3] );
}

