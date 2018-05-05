#include <sys/types.h>
#include <netinet/in.h>
#include <inttypes.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <strings.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>

#define PORT 1500

int main(int argc, char *argv[]){ //Get an array of chars (which we put an IP in it) and int.
  int socket_fd , fsize;
  struct sockaddr_in  router, client , from;
  // struct hostent *hostptr;
  printf("Enter messege:\n");
  char msg [1000];
  fgets(msg, 1000, stdin);
  
  socket_fd = socket (AF_INET, SOCK_DGRAM, 0); //Is the socket descriptor which return by the function socket() that get an IPv4, type of the socket and tcp as default (make empty).

  memset((char *) &client,0, sizeof(client)); //We deleted in the sockaddr_in, s_in, the first n tabs of s_in and instead write 0.
  gethostbyname(argv[1]); //Return the name of the computer which we run a software in it. In case of problem it returns -1. it get the IP of the computer which we put in the array.
  client.sin_family = (short) AF_INET; //Address family = IPv4 of AF_INET.
  client.sin_addr.s_addr= htonl(INADDR_ANY);
  client.sin_port = PORT; //Convert the port number to Host Byte Order representation with the function htons() which host to network short
 
  bind(socket_fd, (struct sockaddr *)&client, sizeof(client));  //This function connect address and port with socket. Socket_fd is the socket descriptor which return by the function socket() (before the division to server and client), s_in point to the structure sockaddr_in which contain address and port of server which we connect to, and get the size of s_in in bits. The function return -1 if there is any problem.
  
  gethostbyname(argv[1]); //Return the name of the computer which we run a software in it. In case of problem it returns -1. it get the IP of the computer which we put in the array.
  memset((char *) &router,0, sizeof(router)); //We deleted in the sockaddr_in, s_in, the first n tabs of s_in and instead write 0.
  router.sin_family = (short) AF_INET; //Address family = IPv4 of AF_INET.
  router.sin_addr.s_addr= htonl(INADDR_ANY);
  router.sin_port = 2000; //Convert the port number to Host Byte Order representation with the function htons() which host to network short

  sendto(socket_fd,&msg,100,0,(struct sockaddr *)&router,sizeof(router));

  while(1) {
    fsize = sizeof(from); //Get the size of form
    recvfrom(socket_fd, &msg, 100,0,(struct sockaddr *)&from,&fsize);  //Receive the messege. Through the socket_ft we read the messege, &msg point to the msg which we read into it, we get the max size of msg in bits, 0 is flag which help us see if connection is closed, from point to the structure which contain IP and port of the sender and fsize point to the local variable which will contain the size of the sockaddr_in structure. The function return the size of bits that received and if it returns -1 it means that there is a problem.
    printf("Got data: %s\n",msg);
    if(strcmp(msg, "Disconnected.\n") == 0 || strcmp(msg, "Messege deleted.\n") == 0 || strcmp(msg, "Server disconnected.\n") == 0){
      break;
    }
    fflush(stdout); //Flushes the output buffer of the stream.    
  }
  return 0;
}