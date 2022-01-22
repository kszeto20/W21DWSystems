#include "pipe_networking.h"


int main() {

  int to_client;
  int from_client;

  from_client = server_handshake( &to_client );

  while(1) {
    char *mess = malloc(BUFFER_SIZE);

    read(from_client, mess, BUFFER_SIZE);
    printf("Received: %s\n", mess);

    char * temp = malloc(BUFFER_SIZE);

    int i;
    for (i = strlen(mess) - 1; i >= 0; i--){
      if (*(mess + i) != 20){
        strncat(temp, mess + i, 1);
        strcat(temp, "!");
      }
    }

    write(to_client, temp, BUFFER_SIZE);
    printf("Sending back now!\n");
    free(mess);
    free(temp);

  }
  return 0;
}
