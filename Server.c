/*
* Ibrahim Khan
* CS 4220-001
* Project 2: TCP Program - Server
* 05/05/2021
* This is a TCP socket program for a server that can be compiled and executed on a Linux server.
* The program allows files to be transferred to a client program and also allows files to be received
* from a client program.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/fcntl.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

#define SERVER_PORT 2398
#define BUF_SIZE 4096
#define QUEUE_SIZE 10 //block transfer size

int main (int argc, char * argv[]) {
	
	int s, b, l, fd, sa, bytes, on = 1;
	char buf[BUF_SIZE]; //Buffer for outgoing file
	struct sockaddr_in channel; //Holds IP address
	
	//Build address structure to bind to socket
	memset(&channel, 0, sizeof(channel)); //zero channel
	channel.sin_family = AF_INET;
	channel.sin_addr.s_addr = htonl(INADDR_ANY);
	channel.sin_port = htons(SERVER_PORT);
	
	//Passive open - wait for connection
	s = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP); //Create socket
	if (s < 0) fatal("socket failed");
	setsockopt(s, SOL_SOCKET, SO_REUSEADDR, (char *) &on, sizeof(on));
	
	b = bind(s, (struct sockaddr *) &channel, sizeof(channel));
	if (b < 0) fatal("bind failed");
	
	l = listen(s, QUEUE_SIZE); //Specify queue size
	if (l < 0) fatal("listen failed");
	
	//Socket is not set up and bound - Wait for connection and process it
	while (1) {
		sa = accept(s, 0, 0); //Block for connection request
		if (sa < 0) fatal("accept failed");
		
		read(sa, buf, BUF_SIZE); //Read file name from socket
		
		//Get and return the file

		fd = open(buf, O_RDONLY); //Open the file to be sent back
		if (fd < 0) fatal("open failed");
		
		while (1) {
			bytes = read(fd, buf, BUF_SIZE); //Read from file
			if (bytes <= 0) break; //Check for end of file
			write(sa, buf, bytes); //Write bytes to socket
		}
		
		close(fd); //Close file
		close(sa); //Close connection
	}
}

fatal(char *string) {
	printf("%s\n", string);
	exit(1);
}
