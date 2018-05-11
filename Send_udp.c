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


int main(int argc, char *argv[]){ //Get an array of chars (which we put an IP in it) and int.
  int socket_fd , fsize;
  struct sockaddr_in  dest , from;
  struct hostent *hostptr;
//struct { char head; u_long body; char tail; } msgbuf; //Define the messege with veriables
  char myName [] = "Orel Shalom";
  char yourName [100];

  socket_fd = socket (AF_INET, SOCK_DGRAM, 0); //Is the socket descriptor which return by the function socket() that get an IPv4, type of the socket and tcp as default (make empty).

  memset((char *) &dest,0, sizeof(dest)); //We deleted in the sockaddr_in, s_in, the first n tabs of s_in and instead write 0.
  
  hostptr = gethostbyname(argv[1]); //Return the name of the computer which we run a software in it. In case of problem it returns -1. it get the IP of the computer which we put in the array.
  dest.sin_family = (short) AF_INET; //Address family = IPv4 of AF_INET.
  memcpy(hostptr->h_addr, (char *)&dest.sin_addr,hostptr->h_length); //Copies n characters from memory area hostptr->h_addr (host address) to memory area &dest.sin_addr (which converted the IPv4 address in binary representation to network representation), and hostptr->h_length is the number of bytes to be copied.
  dest.sin_port = htons((u_short)0x3333); //Convert the port number to Host Byte Order representation with the function htons() which host to network short

  //msgbuf.head = '<';
  //msgbuf.body = htonl(getpid()); 
  //msgbuf.tail = '>';

  sendto(socket_fd,&myName, sizeof(myName), 0, (struct sockaddr *)&dest, sizeof(dest));//Send the messege. Through the socket_fd we send the info, &myName point to th info we send, we get the lenght of it in bits, &dest point to the struct sockaddr which contain the ip and he port of the destination and get the lenght of it. 

  while(1) {
    fsize = sizeof(from); //Get the size of form
    recvfrom(socket_fd, &yourName, 100, 0,(struct sockaddr *)&from, &fsize);  //Receive the messege. Through the socket_ft we read the messege, &msg point to the msg which we read into it, we get the max size of msg in bits, 0 is flag which help us see if connection is closed, from point to the structure which contain IP and port of the sender and fsize point to the local variable which will contain the size of the sockaddr_in structure. The function return the size of bits that received and if it returns -1 it means that there is a problem.
    printf("Got data ::%s\n", yourName);
    fflush(stdout); //Flushes the output buffer of the stream.    
  }
  return 0;
}