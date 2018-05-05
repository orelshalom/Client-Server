
#include <sys/types.h>
#include <unistd.h>          
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <signal.h>    
#include <sys/socket.h>
#include <sys/un.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>

//#define IP_ADDRESS "127.0.0.1" 
#define MAXBUF  1024

int main(int argc, char* argv[])
{ 
  struct sockaddr_in cli_name; 
  int sock, bytes_read;
  int port = 80;
  char ip[100];
  char* hostname;
  char path[100];
  char* url = argv[1];
  struct addrinfo* res;
  struct sockaddr_in* saddr;
  char buffer[MAXBUF];
  
  memset(ip,0,100);
  memset(path,0,100);

  if(sscanf(url, "http://%99[^:]:%99d/%99[^\n]", ip, &port, path) == 3){
    printf("ip = %s\n" ,ip);
    printf("port = %d\n" ,port);
    printf("path = \"%s\"\n" ,path);
  }
  else{
    sscanf(url, "http://%99[^/]/%99[^\n]", ip, path);
    printf("ip = %s\n" ,ip);
    printf("port = %d\n" ,port);
    printf("path = %s\n" ,path);
  }
    
    
  hostname = ip;
  
  if (argc != 2) {
    perror("Usage: hostnamelookup <hostname>\n");
    exit(1);
  }

  // check if its a valid hostname
  if (0 != getaddrinfo(hostname, NULL, NULL, &res)) {
    fprintf(stderr, "Error in resolving hostname %s\n", hostname);
    exit(1);
  }

  printf("Client is alive and establishing socket connection.\n\n");
  
  
  // Create a new socket of type ipv4 and TCP, without stating the protocol and returning its ptr
  sock = socket(AF_INET, SOCK_STREAM, 0);
  if (sock < 0)
    { perror ("Error opening channel");
      close(sock);
      exit(1);
    }
    
  saddr = (struct sockaddr_in*)res->ai_addr;//Get the sockaddr of the addrinfo ,res.
  
  // set all values in the buffer to zero
  bzero(&cli_name, sizeof(cli_name)); 
  cli_name.sin_family = AF_INET; 
  //cli_name.sin_addr.s_addr = inet_addr(IP_ADDRESS); 
  cli_name.sin_addr.s_addr = inet_addr(inet_ntoa(saddr->sin_addr)); 
  cli_name.sin_port = htons(port);

    // Connect to server
    if (connect(sock, (struct sockaddr *)&cli_name, sizeof(cli_name)) < 0)
    { perror ("Error establishing communications");
      close(sock);
      exit(1);
    }
    

    sprintf(buffer, "GET %s HTTP/1.0\r\nHOST: %s\r\n\r\n", url, hostname);
    send(sock, buffer, strlen(buffer), 0);

    do
    {
        bzero(buffer, sizeof(buffer));
        bytes_read = recv(sock, buffer, sizeof(buffer), 0);
        if ( bytes_read > 0 )
            printf("%s", buffer);
    }
    while ( bytes_read > 0 );
    

  printf("\nExiting now.\n");

  close(sock); 
  exit(0);

}