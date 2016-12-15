#include <stdio.h>
#include <stdlib.h>

#include "jpeg_header_parser.h"
#include "file_io.h"

int main( int argc, const char* argv[] )
{
    int jpegDataLength = 0;
	uint8_t* jpegData = fileRead( "test.jpg", &jpegDataLength );

	printf( "Fichier jpeg lu de taille : %d octets\n", jpegDataLength );

	free( jpegData );

    return 0;
}
