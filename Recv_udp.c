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

void printsin(struct sockaddr_in *sin, char *pname, char* msg){
  char *ip = inet_ntoa(sin->sin_addr);
  puts(pname); 
  printf("%s IP: %s, Port: %d\n", msg, ip, sin->sin_port);
}

int main(int argc, char *argv[]){ //Get an array of chars (which we put an IP in it) and int. 
  int socket_fd, fsize;
  struct sockaddr_in  s_in, from; //sockaddr_in contain information about the IP of the socket for IPv4
  //struct { char head; u_long  body; char tail;} msg; //Define the messege ('<' + prosses id (network to host long) + '>') with variables
  char myName [100];
  char yourName [] = "Your name is: Orel Shalom";

  socket_fd = socket (AF_INET, SOCK_DGRAM, 0); //Is the socket descriptor which return by the function socket() that get an IPv4, type of the socket and tcp as default (make empty).

  memset((char *) &s_in,0, sizeof(s_in)); //We deleted in the sockaddr_in, s_in, the first n tabs of s_in and instead write 0. 

  s_in.sin_family = (short)AF_INET; //Address family = IPv4 of AF_INET.
  s_in.sin_addr.s_addr = htonl(INADDR_ANY); //Convert the IPv4 address in binary representation, in one 4-bytes integer (s_addr), to network representation with the function htonl() which host to network long.
  s_in.sin_port = htons((u_short)0x3333); //Convert the port number to Host Byte Order representation with the function htons() which host to network short.

  printsin(&s_in, "RECV_UDP", "Local socket is:"); 
  fflush(stdout);//Flushes the output buffer of the stream. 

  bind(socket_fd, (struct sockaddr *)&s_in, sizeof(s_in));  //This function connect address and port with socket. Socket_fd is the socket descriptor which return by the function socket() (before the division to server and client), s_in point to the structure sockaddr_in which contain address and port of server which we connect to, and get the size of s_in in bits. The function return -1 if there is any problem.

  
  while(1) {
    fsize = sizeof(from); //Get the size of form
    recvfrom(socket_fd, &myName, 100,0,(struct sockaddr *)&from,&fsize);  //Receive the messege. Through the socket_ft we read the messege, &msg point to the msg which we read into it, we get the max size of msg in bits, 0 is flag which help us see if connection is closed, from point to the structure which contain IP and port of the sender and fsize point to the local variable which will contain the size of the sockaddr_in structure. The function return the size of bits that received and if it returns -1 it means that there is a problem.
    printsin( &from, "recv_udp: ", "Packet from:");
    //printf("Got data ::%c%ld%c\n",msg.head,(long) ntohl(msg.body),msg.tail); //Print the messege 
    printf("Got data ::%s\n",myName);
    fflush(stdout); //Flushes the output buffer of the stream. 
    sendto(socket_fd,&yourName,sizeof(yourName),0,(struct sockaddr *)&from,sizeof(from));//Send the messege. Through the socket_fd we send the info, &yourName point to th info we send, we get the lenght of it in bits, &from point to the struct sockaddr which contain the ip and he port of the destination and get the lenght of it.
  }
  
  return 0;
}
