#include <sys/types.h>
#include <netinet/in.h>
#include <inttypes.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <strings.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>

#define PORT 3000


int main(int argc, char *argv[]){ //Get an array of chars (which we put an IP in it) and int. 
  int socket_fd, fsize;
  struct sockaddr_in  server, from, router; //sockaddr_in contain information about the IP of the socket for IPv4
  // struct hostent *hostptr; 
  char msg [1000];     
  

  socket_fd = socket (AF_INET, SOCK_DGRAM, 0); //Is the socket descriptor which return by the function socket() that get an IPv4, type of the socket and tcp as default (make empty).

  memset((char *) &server,0, sizeof(server)); //We deleted in the sockaddr_in, s_in, the first n tabs of s_in and instead write 0. 
  server.sin_family = (short)AF_INET; //Address family = IPv4 of AF_INET.
  server.sin_addr.s_addr = htonl(INADDR_ANY); //Convert the IPv4 address in binary representation, in one 4-bytes integer (s_addr), to network representation with the function htonl() which host to network long.
  server.sin_port = PORT; //Convert the port number to Host Byte Order representation with the function htons() which host to network short.

  memset((char *) &router,0, sizeof(router)); //We deleted in the sockaddr_in, s_in, the first n tabs of s_in and instead write 0.
  router.sin_family = (short) AF_INET; //Address family = IPv4 of AF_INET.
  router.sin_addr.s_addr = htonl(INADDR_ANY); //Convert the IPv4 address in binary representation, in one 4-bytes integer (s_addr), to network representation with the function htonl() which host to network long.
  router.sin_port = 2000; //Convert the port number to Host Byte Order representation with the function htons() which host to network short

  bind(socket_fd, (struct sockaddr *)&server, sizeof(server));  //This function connect address and port with socket. Socket_fd is the socket descriptor which return by the function socket() (before the division to server and client), s_in point to the structure sockaddr_in which contain address and port of server which we connect to, and get the size of s_in in bits. The function return -1 if there is any problem.
 
  while(1) {
    fsize = sizeof(from); //Get the size of form
    recvfrom(socket_fd, &msg, 100,0,(struct sockaddr *)&from,&fsize);  //Receive the messege. Through the socket_ft we read the messege, &msg point to the msg which we read into it, we get the max size of msg in bits, 0 is flag which help us see if connection is closed, from point to the structure which contain IP and port of the sender and fsize point to the local variable which will contain the size of the sockaddr_in structure. The function return the size of bits that received and if it returns -1 it means that there is a problem.
    printf("Got data: %s\n",msg);
    // if(strcmp(msg, "Disconnected.\n") == 0 || strcmp(msg, "Client disconnected.") == 0){
    //   break;
    // }
    // else{
    if(strcmp(msg, "Messege deleted.\n") != 0 && strcmp(msg, "Client disconnected.") != 0){
      printf("Enter messege:\n");
      fgets(msg, 1000, stdin);
      char msgs [1000];
      strcpy(msgs, msg);
      sendto(socket_fd,&msg,100,0,(struct sockaddr *)&router,sizeof(router));
    }
    fflush(stdout); //Flushes the output buffer of the stream.     
  }
  
  return 0;
}
