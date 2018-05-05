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
#define PORT 1337 

void clean_up(int cond, int *sock){ //Closing function.
    printf("Exiting now.\n");
    close(*sock); //Close socket.
    exit(cond); // If cond = 0 ,exit if the program completed successfully. If cond = 1, exit with an error.
} 

int main(void){ 
  int sock; 
  int connect_sock; 
  struct sockaddr_in serv_name; 
  socklen_t len; 
  int count;
  
  sock = socket(AF_INET, SOCK_STREAM, 0);//Upon successful completion, it return the socket file descriptor. Otherwise -1.  
  if (sock < 0){ //If socket() doesnt succeeded, do:
      perror ("Error opening channel");//Prints a descriptive error message to stderr.
      clean_up(1, &sock);//Calling closing function.
    }
  
  memset((char *)&serv_name,0, sizeof(serv_name)); //We deleted in the sockaddr_in, serv_name, the first n tabs of it and instead write 0. 
  serv_name.sin_family = AF_INET; //Address family = IPv4 of AF_INET.                              
  serv_name.sin_port = htons(PORT); //Convert the port number to Host Byte Order representation with the function htons() which host to network.

  if (bind(sock, (struct sockaddr *)&serv_name, sizeof(serv_name)) < 0){ //Connect address and port with socket. Sock is the socket descriptor which return by the function socket(), serv_name point to the structure sockaddr_in which contain address and port of server which we connect to, and get the size of it in bits. The function return -1 if there is any problem.
      perror ("Error naming channel");//Prints a descriptive error message to stderr.
      clean_up(1, &sock);//Calling closing function.
    }
      
  printf("Server is alive and waiting for socket connection from client.\n");
  listen(sock, 1); //Mark a connection-mode socket, specified by the socket argument, as accepting connections. 1 = limit number of connections in the socket's listen queue. Upon successful completions, it return 0, otherwise -1.

  len = sizeof(serv_name);
  connect_sock = accept(sock, (struct sockaddr *)&serv_name, &len);//Extract the first connection on the queue of pending connections, create a new socket with the same socket type protocol and address family as the specified socket, and allocate a new file descriptor for that socket.

  for (count = 1; count <= SIM_LENGTH; count++){ 
	  write(connect_sock, &count, 4);//Writes data out of a buffer, with a file descriptor of where to write the output, a pointer to a buffer of at least 4 bytes which will be written to the file, and the number of bytes to write (4).
      printf("Server has written %d to socket.\n", count);
    }

  close(connect_sock); //Close connect_sock.
  clean_up(0, &sock); //Calling closing function. Exit with program completed successfully.

}