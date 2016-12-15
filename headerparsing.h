#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>

/** \brief Codes renvoyés par les fonctions de parsing
 */
enum
{
    NO_ERROR = 0, /**< \brief Pas d'erreur */
    FILE_ERROR = 1, /**< \brief Erreur de lecture dans le fichier */
    FORMAT_ERROR = 2 /**< \brief Erreur liée aux valeurs lues (ex : valeurs illégales/incohérentes) */
};

/** \brief Teste la presence du nombre magique JPEG
 * \param file Fichier a traiter
 * \return Code erreur parsing
 */
int isFileJPEGFormat( FILE* file );

/** \brief Transforme 2 octets séparés en 1 mot de 2 octets
 * \param msb Octet poids fort
 * \param lsb Octet poids faible
 * \return Mot de 2 octets
 */
uint16_t shortToInt( uint8_t msb, uint8_t lsb );

/** \brief Contient les données du header du fichier JPEG
 */
typedef struct
{
    uint8_t truc; /*!< \brief Ceci est un test */

} JPEGHeader;

/** \brief Lit toutes les donnees du header JPEG
 * \param file Fichier contenant les données
 * \param header Structure à remplir
 * \return Code erreur parsing
 */
int parseHeader( FILE* file, JPEGHeader* header );

/** \brief Lit les données du SOF
 * \param file Fichier contenant les données
 * \param header Structure à remplir
 * \return Code erreur parsing
 */
int parseSOF( FILE* file, JPEGHeader* header );

/** \brief Lit les données du SOS
 * \param file Fichier contenant les données
 * \param header Structure à remplir
 * \return Code erreur parsing
 */
int parseSOS( FILE* file, JPEGHeader* header );

/** \brief Lit les données du DQT
 * \param file Fichier contenant les données
 * \param header Structure à remplir
 * \return Code erreur parsing
 */
int parseDQT( FILE* file, JPEGHeader* header );

/** \brief Lit les données du DHT
 * \param file Fichier contenant les données
 * \param header Structure à remplir
 * \return Code erreur parsing
 */
int parseDHT( FILE* file, JPEGHeader* header );

/** \brief Produit les codes pour le codage Huffman
 * \param header Structure JPEGHeader source
 * \return Code erreur
 */
int buildHuffmanCodes( JPEGHeader* header );
