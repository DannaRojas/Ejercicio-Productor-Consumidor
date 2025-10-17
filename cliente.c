/*******************************************************************************************************
*               Pontificia Universidad Javeriana
*                           ----------                                                                  *
*       Autor: Danna Gabriela Rojas Bernal                                                              *
*       Fecha: 10 Octubre de 2025                                                                       *
*       Docente: J. Corredor                                                                            *
*       Materia: Sistemas Operativos                                                                    *
*       Tema: Implementación de Productor Consumidor a partir de Pipe Named                             *
*       Descripción: Archivo servidor que permite la comunicación entre procesos mediante un pipe FIFO. *
******************************************************************************************************/
// cliente

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>

#define FIFO_FILE "/tmp/fifo_twoway" // Ruta del FIFO (canal con nombre)

int main() {
   int fd;
   int end_process;
   int stringlen;
   int read_bytes;
   char readbuf[80];
   char end_str[5];
   printf("FIFO_CLIENT: Send messages, infinitely, to end enter \"end\"\n");
   //Abrir el FIFO con permisos para escribir y leer
   fd = open(FIFO_FILE, O_CREAT|O_RDWR);
   //Inicializa la cadena
   strcpy(end_str, "end");

   while (1) {
      printf("Enter string: ");
      //Leer lo que ingresan por consola
      fgets(readbuf, sizeof(readbuf), stdin);
  //Quitar el salto de línea final
      stringlen = strlen(readbuf);
      readbuf[stringlen - 1] = '\0';
      //Comparar si el mensaje es "end"
      end_process = strcmp(readbuf, end_str);

      //Si el mensaje no es end, enviarlo y espera respuesta
      if (end_process != 0) {
      //Envia mensaje al servidor
         write(fd, readbuf, strlen(readbuf));
         printf("FIFOCLIENT: Sent string: \"%s\" and string length is %d\n", readbuf, (int)strlen(readbuf));
         //Lee las respuestas del servidor
         read_bytes = read(fd, readbuf, sizeof(readbuf));
         readbuf[read_bytes] = '\0';
         printf("FIFOCLIENT: Received string: \"%s\" and length is %d\n", readbuf, (int)strlen(readbuf));
      } else { //Si el mensaje esend,envia y se sale del bucle
         write(fd, readbuf, strlen(readbuf));
         printf("FIFOCLIENT: Sent string: \"%s\" and string length is %d\n", readbuf, (int)strlen(readbuf));
         close(fd);
         break;
      }
   }
   return 0;
}