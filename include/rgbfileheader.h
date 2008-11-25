/* $Id: rgbfileheader.h,v 1.1.1.1 2008-11-25 08:02:37 mcouprie Exp $ */

struct RGBFILEHEADER {       /* size 108 bytes */
  uint16_t magic;      /* size 2 bytes : magic number = 474 */
  uint8_t compression; /* size 1 byte : 0 for no compression */
  uint8_t bytespercha; /* size 1 byte : nb. bytes per channel */
  uint16_t dim;        /* size 2 bytes : nb. channels */
  uint16_t width;      /* size 2 bytes : image row size */
  uint16_t height;     /* size 2 bytes : image col size */
  uint16_t components; /* size 2 bytes : components */
  uint32_t mincol;      /* size 4 bytes : 0 */
  uint32_t maxcol;      /* size 4 bytes : 255 */
  uint32_t dummy;       /* size 4 bytes : dummy */
  char name[80];             /* size 80 bytes : image name or comment */
  uint32_t cmaptype;    /* size 4 bytes : 0 for NORMAL RGB */
}; /** plus 404 bytes dummy padding to make header 512 bytes **/
