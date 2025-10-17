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
#include <stdio.h>       // Funciones de entrada/salida estándar (printf, fgets)
#include <unistd.h>      // API de POSIX (read, write, close)
#include <sys/types.h>   // Tipos de datos del sistema
#include <string.h>      // Funciones de manipulación de cadenas (strlen, strcmp, strcpy)
#include <fcntl.h>       // Control de archivos (open, O_CREAT, O_RDWR)
#include <sys/stat.h>    

// Definición de la ruta del archivo FIFO para comunicación bidireccional
#define FIFO_FILE "/tmp/fifo_twoway" // Ruta del FIFO (canal con nombre)

int main() {
   int fd;
   int end_process;
   int stringlen;
   int read_bytes;
   char readbuf[80];
   char end_str[5];
   
   // Muestra mensaje inicial al usuario con instrucciones
   printf("FIFO_CLIENT: Send messages, infinitely, to end enter \"end\"\n");
   
   //Abrir el FIFO con permisos para escribir y leer
   // O_CREAT: Crea el archivo si no existe
   // O_RDWR: Abre en modo lectura y escritura
   fd = open(FIFO_FILE, O_CREAT|O_RDWR);

   // Copia "end" en end_str para usarla como referencia de terminación
   strcpy(end_str, "end");
   
   // Bucle infinito para intercambio continuo de mensajes
   while (1) {
      // Solicita al usuario que ingrese una cadena
      printf("Enter string: ");
      
      //Leer lo que ingresan por consol
      fgets(readbuf, sizeof(readbuf), stdin);
   
      // Calcula la longitud de la cadena ingresada
      stringlen = strlen(readbuf);
      // Reemplaza el '\n' final por el terminador nulo '\0'
      readbuf[stringlen - 1] = '\0';
      
      //Comparar si el mensaje es "end"
      // strcmp retorna 0 si las cadenas son iguales
      end_process = strcmp(readbuf, end_str);
      
      //Si el mensaje no es end, enviarlo y espera respuesta
      if (end_process != 0) {
      //Envia mensaje al servidor
         // Escribe la cadena en el FIFO para que el servidor la lea
         write(fd, readbuf, strlen(readbuf));
         // Confirma el envío del mensaje mostrando la cadena y su longitud
         printf("FIFOCLIENT: Sent string: \"%s\" and string length is %d\n", readbuf, (int)strlen(readbuf));
         
         //Lee las respuestas del servidor
         // Lee la respuesta del servidor desde el FIFO
         read_bytes = read(fd, readbuf, sizeof(readbuf));
         // Agrega el terminador nulo al final de la cadena recibida
         readbuf[read_bytes] = '\0';
         // Muestra la respuesta recibida del servidor
         printf("FIFOCLIENT: Received string: \"%s\" and length is %d\n", readbuf, (int)strlen(readbuf));
      } else { //Si el mensaje esend,envia y se sale del bucle
         // Envía el mensaje "end" al servidor para notificar la terminación
         write(fd, readbuf, strlen(readbuf));
         // Confirma el envío del último mensaje
         printf("FIFOCLIENT: Sent string: \"%s\" and string length is %d\n", readbuf, (int)strlen(readbuf));
         // Cierra el descriptor de archivo del FIFO
         close(fd);
         break;
      }
   }
   // Retorna 0 indicando terminación exitosa del programa
   return 0;
}
