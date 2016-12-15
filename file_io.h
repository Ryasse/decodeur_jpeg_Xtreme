#ifndef FILE_IO_H_
#define FILE_IO_H_

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#define DEBUG 1

/**
 * \struct PPM tools.h
 * \brief La structure struct PPM permet de stocker les informations
 * d'entête pour le format PPM.
 **/
struct PPM{
	// PPM header
	char magic[2];	/*!< 2 bytes - 'P' '3' */
	int width;      /*!< 4 bytes : the image width */
	int height;		/*!< 4 bytes : the image height */
	int max;		/*!< 4 bytes : max value (less than 65536 and more than zero).*/
};

uint8_t* fileRead( const char*, int* );
void imageWritePPM( char*, int, int, uint8_t*, uint8_t*, uint8_t* );

#endif /* FILE_IO_H_ */
