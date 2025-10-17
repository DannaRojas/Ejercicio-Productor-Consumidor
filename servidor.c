/*******************************************************************************************************
*               Pontificia Universidad Javeriana
*                           ----------                                                                  *
*       Autor: Danna Gabriela Rojas Bernal                                                              *
*       Fecha: 16 Octubre de 2025                                                                       *
*       Docente: J. Corredor                                                                            *
*       Materia: Sistemas Operativos                                                                    *
*       Tema: Implementación de Productor Consumidor a partir de Pipe Named                             *
*       Descripción: Archivo servidor que permite la comunicación entre procesos mediante un pipe FIFO. *
******************************************************************************************************/

//Servidor//

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>



#define FIFO_FILE "/tmp/fifo_twoway"
//FUnción que invierte la cadena
void reverse_string(char *);
int main() {
   int fd;
   char readbuf[80];
   char end[10];
   int to_end;
   int read_bytes;

   /* Create the FIFO if it does not exist */
   mkfifo(FIFO_FILE, S_IFIFO|0640);
   strcpy(end, "end");
   //Abrir el FIFO en modo lectura y escritura
   fd = open(FIFO_FILE, O_RDWR);
   while(1) {
 //Leer datos del cliente
      read_bytes = read(fd, readbuf, sizeof(readbuf));
      readbuf[read_bytes] = '\0';
      printf("FIFOSERVER: Received string: \"%s\" and length is %d\n", readbuf, (int)strlen(readbuf));
      //Verificar si el mensaje es end
      to_end = strcmp(readbuf, end);

      if (to_end == 0) {
         close(fd);
         break;
      }
      //Invertir la cadena y enviarlo
      reverse_string(readbuf);
      printf("FIFOSERVER: Sending Reversed String: \"%s\" and length is %d\n", readbuf, (int) strlen(readbuf));
      write(fd, readbuf, strlen(readbuf));
      sleep(2);//Pausa
   }
   return 0;
}
//FUnción para invertir la cadena
void reverse_string(char *str) {
   int last, limit, first;
   char temp;
   last = strlen(str) - 1;
   limit = last/2;
   first = 0;

   while (first < last) {
      temp = str[first];
      str[first] = str[last];
      str[last] = temp;
      first++;
      last--;
   }
   return;
}