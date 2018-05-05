
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <netinet/in.h>
#include <arpa/inet.h>

int main(int argc, char* argv[]) {
  struct addrinfo* res; //This struct contains some veriables that are the information about the address.
  char* hostname;
  char* hostaddr;
  struct sockaddr_in* saddr;
  
  if (argc != 2) {//Check if there is a hostname too.
    perror("Usage: hostnamelookup <hostname>\n");
    exit(1); 
  }

  hostname = argv[1];//The hostname of the computer
  
  if (0 != getaddrinfo(hostname, NULL, NULL, &res)) {//This function lets us supply a hostname as a string, and it will resolve that hostname to a result of type struct sockaddr.
    fprintf(stderr, "Error in resolving hostname %s\n", hostname);//Sends formatted output (error) to stderr stream.
    exit(1);//Exit
  }
  
  saddr = (struct sockaddr_in*)res->ai_addr;//Get the sockaddr of the addrinfo ,res.
  hostaddr = inet_ntoa(saddr->sin_addr);//Converts the network address to IP.

  printf("Address for %s is %s\n", hostname, hostaddr);//Print the IP address of the hostname. 
  exit(0);
}