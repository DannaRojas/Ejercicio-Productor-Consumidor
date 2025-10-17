/*******************************************************************************************************
*                                     Pontificia Universidad Javeriana
*                                                ----------                                             *
*       Autor: Danna Gabriela Rojas Bernal                                                              *
*       Fecha: 16 Octubre de 2025                                                                       *
*       Docente: J. Corredor                                                                            *
*       Materia: Sistemas Operativos                                                                    *
*       Tema: Implementación de Productor Consumidor a partir de Pipe Named                             *
*       Descripción: Archivo servidor que permite la comunicación entre procesos mediante un pipe FIFO. *
******************************************************************************************************/
//Servidor//

// dECLARACION DE BIBLIOTECA 
#include <stdio.h>       
#include <unistd.h>      
#include <stdlib.h>      
#include <string.h>      
#include <fcntl.h>       
#include <sys/types.h>  
#include <sys/stat.h>    

// Definición de la ruta del archivo FIFO para comunicación bidireccional
#define FIFO_FILE "/tmp/fifo_twoway"

//FUnción que invierte la cadena
// Prototipo de la función que invierte una cadena de caracteres
void reverse_string(char *);

int main() {
   // Descriptor de archivo para manejar el FIFO
   int fd;
   // Buffer para almacenar los datos leídos desde el FIFO
   char readbuf[80];
   // Cadena que contiene el comando de finalización
   char end[10];
   // Variable para almacenar el resultado de la comparación con "end"
   int to_end;
   // Número de bytes leídos desde el FIFO
   int read_bytes;
   
   /* Create the FIFO if it does not exist */
   // Crea el FIFO con permisos específicos
   // S_IFIFO: Indica que es un archivo FIFO
   // 0640: Permisos (lectura/escritura para propietario, lectura para grupo)
   mkfifo(FIFO_FILE, S_IFIFO|0640);
   
   // Copia "end" en la variable end para usarla como referencia de terminación
   strcpy(end, "end");
   
   //Abrir el FIFO en modo lectura y escritura
   // O_RDWR: Permite leer y escribir en el FIFO
   fd = open(FIFO_FILE, O_RDWR);
   
   // Bucle infinito para procesar mensajes continuamente
   while(1) {
//Leer datos del cliente
      // Lee los datos enviados por el cliente desde el FIFO
      read_bytes = read(fd, readbuf, sizeof(readbuf));
      // Agrega el terminador nulo al final de la cadena recibida
      readbuf[read_bytes] = '\0';
      // Muestra la cadena recibida y su longitud
      printf("FIFOSERVER: Received string: \"%s\" and length is %d\n", readbuf, (int)strlen(readbuf));
      
      //Verificar si el mensaje es end
      // Compara la cadena recibida con "end"
      // strcmp retorna 0 si las cadenas son iguales
      to_end = strcmp(readbuf, end);
      
      // Si el mensaje recibido es "end", termina el servidor
      if (to_end == 0) {
         // Cierra el descriptor de archivo del FIFO
         close(fd);
         // Sale del bucle infinito
         break;
      }
      
      //Invertir la cadena y enviarlo
      // Llama a la función que invierte la cadena recibida
      reverse_string(readbuf);
      // Muestra la cadena invertida antes de enviarla
      printf("FIFOSERVER: Sending Reversed String: \"%s\" and length is %d\n", readbuf, (int) strlen(readbuf));
      // Escribe la cadena invertida en el FIFO para que el cliente la lea
      write(fd, readbuf, strlen(readbuf));
      
      sleep(2);//Pausa
      // Pausa de 2 segundos antes de procesar el siguiente mensaje
   }
   
   // Retorna 0 indicando terminación exitosa del programa
   return 0;
}

//FUnción para invertir la cadena
// Función que invierte el orden de los caracteres en una cadena
void reverse_string(char *str) {
   // Índice del último caracter de la cadena
   int last, limit, first;
   // Variable temporal para intercambiar caracteres
   char temp;
   
   // Calcula el índice del último caracter (longitud - 1)
   last = strlen(str) - 1;
   // Calcula el punto medio de la cadena (solo necesita invertir hasta la mitad)
   limit = last/2;
   // Inicializa el índice del primer caracter
   first = 0;
   
   // Bucle que intercambia caracteres desde los extremos hacia el centro
   while (first < last) {
      // Guarda temporalmente el caracter en la posición first
      temp = str[first];
      // Coloca el caracter de la posición last en la posición first
      str[first] = str[last];
      // Coloca el caracter temporal en la posición last
      str[last] = temp;
      // Avanza al siguiente caracter desde el inicio
      first++;
      // Retrocede al siguiente caracter desde el final
      last--;
   }
   
   // Retorna sin valor (la cadena se modifica directamente en memoria)
   return;
}
