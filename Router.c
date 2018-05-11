#include <sys/types.h>
#include <unistd.h>    
#include <strings.h>   
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <signal.h>    
#include <sys/socket.h>
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
#include <stdlib.h>

#define PORT 2000


double randfrom(double min, double max) {
    double range = (max - min); 
    double div = RAND_MAX / range;
    return min + (rand() / div);
} 

int main(int argc, char *argv[]){ //Get an array of chars (which we put an IP in it) and int. 
  
  double x;
  srand(time(NULL));
  double r = randfrom(0.0, 1.0);

  int socket_fd, fsize;
  struct sockaddr_in router ,from, server, client; //sockaddr_in contain information about the IP of the socket for IPv4
  char msg [1000];


  socket_fd = socket (AF_INET, SOCK_DGRAM, 0); //Is the socket descriptor which return by the function socket() that get an IPv4, type of the socket and tcp as default (make empty).

  memset((char *) &router,0, sizeof(router)); //We deleted in the sockaddr_in, s_in, the first n tabs of s_in and instead write 0. 
  router.sin_family = (short)AF_INET; //Address family = IPv4 of AF_INET.
  router.sin_addr.s_addr= htonl(INADDR_ANY);
  router.sin_port = PORT; //Convert the port number to Host Byte Order representation with the function htons() which host to network short.

  bind(socket_fd, (struct sockaddr *)&router, sizeof(router));  //This function connect address and port with socket. Socket_fd is the socket descriptor which return by the function socket() (before the division to server and client), s_in point to the structure sockaddr_in which contain address and port of server which we connect to, and get the size of s_in in bits. The function return -1 if there is any problem.

    
  memset((char *) &server,0, sizeof(server)); //We deleted in the sockaddr_in, s_in, the first n tabs of s_in and instead write 0.
  server.sin_family = (short) AF_INET; //Address family = IPv4 of AF_INET.
  server.sin_addr.s_addr= htonl(INADDR_ANY);
  server.sin_port = 3000; //Convert the port number to Host Byte Order representation with the function htons() which host to network short
   
  memset((char *) &client,0, sizeof(client)); //We deleted in the sockaddr_in, s_in, the first n tabs of s_in and instead write 0.
  client.sin_family = (short) AF_INET; //Address family = IPv4 of AF_INET.
  client.sin_addr.s_addr= htonl(INADDR_ANY);
  client.sin_port = 1500; //Convert the port number to Host Byte Order representation with the function htons() which host to network short


  while(1) {
    fsize = sizeof(from); //Get the size of form
    recvfrom(socket_fd, &msg, 100,0,(struct sockaddr *)&from,&fsize);  //Receive the messege. Through the socket_ft we read the messege, &msg point to the msg which we read into it, we get the max size of msg in bits, 0 is flag which help us see if connection is closed, from point to the structure which contain IP and port of the sender and fsize point to the local variable which will contain the size of the sockaddr_in structure. The function return the size of bits that received and if it returns -1 it means that there is a problem.    
    if(from.sin_port == 1500){
        if(strcmp(msg, "exit\n") == 0){
            // printf("Exiting.");
            strcpy(msg, "Client disconnected.");
            sendto(socket_fd,&msg,100,0,(struct sockaddr *)&server,sizeof(server));
            strcpy(msg, "Disconnected.\n");
            sendto(socket_fd,&msg,100,0,(struct sockaddr *)&client,sizeof(client));   
            // break;
        }
        else{
            printf("Enter number between 0 to 1:\n");  
            scanf("%lf", &x);
            printf("x = %f\n", x);
            printf("r = %f\n", r);
            if(r > x){
                printf("1500 is the port\n");
                printf("Got data: %s\n",msg);
                sendto(socket_fd,&msg,100,0,(struct sockaddr *)&server,sizeof(server));
            }
            else{ 
                // printf("Exiting.");
                strcpy(msg, "Messege deleted.\n");
                printf("%s", msg);
                sendto(socket_fd,&msg,100,0,(struct sockaddr *)&client,sizeof(client));
                // strcpy(msg, "Disconnected.\n");
                sendto(socket_fd,&msg,100,0,(struct sockaddr *)&server,sizeof(server));
                // break;
            }
        }
    }
    else{ 
        if(from.sin_port == 3000 ){ 
            if(strcmp(msg, "exit\n") == 0){
                printf("Exiting.");
                strcpy(msg, "Server disconnected.\n");
                sendto(socket_fd,&msg,100,0,(struct sockaddr *)&client,sizeof(client));
                strcpy(msg, "Disconnected.\n");
                sendto(socket_fd,&msg,100,0,(struct sockaddr *)&server,sizeof(server));
                break;   
            }
            else{
                printf("3000 is the port\n");
                printf("Got data: %s\n",msg);
                sendto(socket_fd,&msg,100,0,(struct sockaddr *)&client,sizeof(client)); 
            }
        }

    }
    fflush(stdout); //Flushes the output buffer of the stream. 
  }
  return 0;
}