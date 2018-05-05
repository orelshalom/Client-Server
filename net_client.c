#include <sys/types.h>
#include <unistd.h>    
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <signal.h>    
#include <sys/socket.h>
#include <sys/un.h>
#include <netinet/in.h>

#define SIM_LENGTH 10 
//#define IP_ADDRESS "127.0.0.1"
#define PORT 1337 

#include <arpa/inet.h>
#include <netdb.h>
#include <netinet/in.h>
#include <arpa/inet.h>

int main(int argc, char* argv[]){ 
  int sock; 
  struct sockaddr_in cli_name; 
  int count;
  int value; 
  struct addrinfo* res; //This struct contains some veriables that are the information about the address.
  char* hostname;
  struct sockaddr_in* saddr;

  if (argc != 2) {//Check if there is a hostname too.
    perror("Usage: hostnamelookup <hostname>\n");//Print an error
    exit(1); //Exit with an error
  }

  hostname = argv[1];//Get the hostname of the computer
  
  if (0 != getaddrinfo(hostname, NULL, NULL, &res)) {//This function lets us supply a hostname as a string, and it will resolve that hostname to a result of type struct sockaddr.
    fprintf(stderr, "Error in resolving hostname %s\n", hostname);//Sends formatted output (error) to stderr stream.
    exit(1);//Exit with an error
  }
  
  //hostaddr = inet_ntoa(saddr->sin_addr);//Converts the network address to IP.

  printf("Client is alive and establishing socket connection.\n");
  
  
  sock = socket(AF_INET, SOCK_STREAM, 0);//Upon successful completion, it return the socket file descriptor. Otherwise -1.
  if (sock < 0){//If socket() doesnt succeeded, do:
    perror ("Error opening channel");//Prints a descriptive error message to stderr.
    close(sock);//Close the socket
    exit(1);//Exit with error.
    }
      
  saddr = (struct sockaddr_in*)res->ai_addr;//Get the sockaddr of the addrinfo ,res.    
  memset((char *)&cli_name,0, sizeof(cli_name)); //We deleted in the sockaddr_in, cli_name, the first n tabs of it and instead write 0.
  cli_name.sin_family = AF_INET; //Address family = IPv4 of AF_INET.
  cli_name.sin_addr.s_addr =  inet_addr(inet_ntoa(saddr->sin_addr));//Convert the string IP_ADDRESS in the standard IPv4 dotted decimal notation, to an integer value suitable for use as an Internet address. Upon successful completion, it return the internet address, otherwise -1.
  cli_name.sin_port = htons(PORT); //Convert the port number to Host Byte Order representation with the function htons() which host to network.


  if (connect(sock, (struct sockaddr *)&cli_name, sizeof(cli_name)) < 0){ //Attempt to make a connection on a socket. Upon successful completion, it return 0, otherwise -1.
    perror ("Error establishing communications");//Prints a descriptive error message to stderr.
    close(sock);//Close the socket
    exit(1);//Exit
    }


  for (count = 1; count <= SIM_LENGTH; count++){ 
    read(sock, &value, 4);//Attempt to read 4 bytes from the file associated with the open file descriptor, sock, into the buffer pointed to by value. It return a non-negative integer indicating the number of bytes actually read. Otherwise, return -1.
    printf("Client has received %d from socket.\n", value);
    }

  printf("Exiting now.\n");

  close(sock);//Close the socket. 
  exit(0); //Exit with successful.

} 