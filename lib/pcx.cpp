#include "pcx.h"
#include <stdio.h>
#include <alloc.h>


PCXbuffer* PCXLoadFile(char* filePath) {
   FILE *file;

   // open PCX file for read binary
   if ((file=fopen(filePath, "rb")) == NULL)
      return NULL; // error opening file (does not exist, invalid permisions)

   PCXhead header;

   if( (fread(&header, sizeof(PCXhead), 1, file)) == NULL)
      return NULL; // error reading PCX header info.

   PCXbuffer *pcx = (PCXbuffer*)malloc(sizeof(PCXbuffer));

   pcx->Width = header.right - header.left + 1;  // set width of PCX
   pcx->Height = header.bottom - header.top + 1; // set length of PCX

   unsigned long totalBytes = (unsigned long)pcx->Width * (unsigned long)pcx->Height;

   if((pcx->buffer=(unsigned char far *)farmalloc(totalBytes)) == NULL) {
      free(pcx);
      return NULL; // error allocating memory for buffer
   }

   /******* (Decode) Read pixels *******/

   unsigned char byte, runLength;
   unsigned long cnt = 0;

   // read all pcx data into buffer
   while(cnt < totalBytes) {
      fread(&byte, 1, 1, file);

      // if upper two bits are set, lower six are run length
      if (byte > 192) {
         runLength = byte - 192; // get run length

         fread(&byte, 1, 1, file); // get the next byte in file

         // write the repeating byte to the buffer until runlength = 0
         while(runLength -- > 0) {
            pcx->buffer[cnt]=byte;
            cnt++;
         }

      } else { // if run length bits are not set, write the byte to the buffer
         pcx->buffer[cnt]=byte;
         cnt++;
      }
   } // end read all pcx stuff into buffer

   // End Read pixels


   // begin Read pallet

   unsigned short n;

   // seek back from end of file 768 bytes
   fseek(file, -768L, SEEK_END);

   for(n=0;n<256;n++) {
      // read the pallet information RGB
      pcx->pallet.r[n]=fgetc(file);
      pcx->pallet.g[n]=fgetc(file);
      pcx->pallet.b[n]=fgetc(file);
   }

   fclose(file); // close file

   // End Read pallet

   return pcx;
}


void PCXFree(PCXbuffer* pcx) {
   farfree(pcx->buffer);
   free(pcx);
}