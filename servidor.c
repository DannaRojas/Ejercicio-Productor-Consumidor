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

// Librerías necesarias para el manejo de entrada/salida estándar y procesos
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>

// Define la ruta del archivo FIFO que se utilizará para la comunicación
#define FIFO_FILE "/tmp/fifo_twoway" // Ruta del FIFO (canal con nombre)

int main() {
   // Descriptor de archivo para el FIFO
   int fd;
   // Variable para verificar si se debe terminar el proceso
   int end_process;
   // Longitud de la cadena ingresada
   int stringlen;
   // Número de bytes leídos desde el FIFO
   int read_bytes;
   // Buffer para almacenar mensajes leídos
   char readbuf[80];
   // Cadena que indica el fin del proceso
   char end_str[5];
   
   // Mensaje inicial al usuario
   printf("FIFO_CLIENT: Send messages, infinitely, to end enter \"end\"\n");
   
   // Abrir el FIFO con permisos para escribir y leer
   // O_CREAT: crea el archivo si no existe
   // O_RDWR: abre el archivo en modo lectura/escritura
   fd = open(FIFO_FILE, O_CREAT|O_RDWR);
   
   // Inicializa la cadena que se utilizará para comparar y terminar el proceso
   strcpy(end_str, "end");
   
   // Bucle infinito para el intercambio continuo de mensajes
   while (1) {
      // Solicita al usuario ingresar un mensaje
      printf("Enter string: ");
      
      // Leer lo que ingresan por consola y almacenarlo en readbuf
      fgets(readbuf, sizeof(readbuf), stdin);
      
      // Quitar el salto de línea final que agrega fgets
      stringlen = strlen(readbuf);
      readbuf[stringlen - 1] = '\0';
      
      // Comparar si el mensaje es "end" para terminar el proceso
      // Retorna 0 si son iguales, diferente de 0 si son distintas
      end_process = strcmp(readbuf, end_str);
      
      // Si el mensaje no es "end", enviarlo y esperar respuesta
      if (end_process != 0) {
         // Envia mensaje al servidor a través del FIFO
         write(fd, readbuf, strlen(readbuf));
         printf("FIFOCLIENT: Sent string: \"%s\" and string length is %d\n", readbuf, (int)strlen(readbuf));
         
         // Lee la respuesta del servidor desde el FIFO
         read_bytes = read(fd, readbuf, sizeof(readbuf));
         // Agrega el terminador nulo al final de la cadena leída
         readbuf[read_bytes] = '\0';
         printf("FIFOCLIENT: Received string: \"%s\" and length is %d\n", readbuf, (int)strlen(readbuf));
      } else { 
         // Si el mensaje es "end", enviarlo y salir del bucle
         write(fd, readbuf, strlen(readbuf));
         printf("FIFOCLIENT: Sent string: \"%s\" and string length is %d\n", readbuf, (int)strlen(readbuf));
         // Cierra el descriptor de archivo del FIFO
         close(fd);
         // Sale del bucle infinito
         break;
      }
   }
   
   // Retorna 0 indicando ejecución exitosa
   return 0;
}
