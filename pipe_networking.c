#include "pipe_networking.h"

/*=========================
  server_handshake
  args: int * to_client

  Performs the server side pipe 3 way handshake.
  Sets *to_client to the file descriptor to the downstream pipe.

  returns the file descriptor for the upstream pipe.
  =========================*/
int server_handshake(int *to_client) {

  mkfifo(WKP, 0644);
  int from_client = open(WKP, O_RDONLY);
  printf("Server: WKP opened\n");

  char in[BUFFER_SIZE];
  for (int i = 0; i < BUFFER_SIZE; i++) {
    in[i] = '\0';
  }
  read(from_client, in, BUFFER_SIZE);
  printf("Server: Reading from client: %s\n", in);

  remove(WKP);
  printf("Server: WKP removed\n");

  // setting up secret pipe
  *to_client = open(in, O_WRONLY);
  printf("Server: Secret Pipe opened\n");

  //printf("got here\n");

  write(*to_client, ACK, strlen(ACK));
  printf("Server: Sent back: %s\n", ACK);

  //confirm message
  char got[BUFFER_SIZE];
  for (int i = 0; i < BUFFER_SIZE; i++) {
    got[i] = '\0';
  }
  //printf("Got here 2\n");
  read(from_client, got, BUFFER_SIZE);
  //printf("Got here 3\n")
  if ((strcmp(got, ACK)) == 0) {
    printf("Server: Three way handshake established\n");
  }
  return from_client;
}


/*=========================
  client_handshake
  args: int * to_server

  Performs the client side pipe 3 way handshake.
  Sets *to_server to the file descriptor for the upstream pipe.

  returns the file descriptor for the downstream pipe.
  =========================*/
int client_handshake(int *to_server) {

  mkfifo(WKP, 0644);
  int from_server;

  char mess[BUFFER_SIZE];
  for (int i = 0; i < BUFFER_SIZE; i++) {
    mess[i] = '\0';
  }
  sprintf(mess, "%d", getpid());
  mkfifo(mess, 0644);


  *to_server = open(WKP, O_WRONLY);
  printf("Client: WKP opened\n");
  write(*to_server, mess, BUFFER_SIZE);
  printf("Client: Sent %s back to the server\n", mess);

  // confirm message
  from_server = open(mess, O_RDONLY);
  printf("Private pipe opened\n");
  char conf[BUFFER_SIZE];
  for (int i = 0; i < BUFFER_SIZE; i++) {
    conf[i] = '\0';
  }
  read(from_server, conf, BUFFER_SIZE);
  if ((strcmp(conf, ACK)) == 0) {
    printf("Client: Handshake success!\n");
  } else {
    printf("Client: Did not recieve %s, received %s\n", ACK, conf);
  }

  remove(mess);
  write(*to_server, conf, BUFFER_SIZE);
  printf("Client: Three way handshake established\n");
  return from_server;
}
