/*
* Ibrahim Khan
* CS 4220-001
* Project 2: TCP Program - Client
* 05/05/2021
* This is a TCP socket program for a client that can be compiled and executed on a Linux server.
* The program allows files to be transferred to a server program and also allows files to be received
* from a server program.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

#define SERVER_PORT 2398
#define BUF_SIZE 4096 //Block transfer size

int main (int argc, char * argv[]) {
	
	int c, s, bytes;
	char buf[BUF_SIZE]; //Buffer for incoming file
	struct hostent *h; //Info about server
	struct sockaddr_in channel; //Holds IP address
	
	if (argc != 3) fatal("Usage: client server-name file-name");
	h = gethostbyname(argv[1]); //Look up host's IP address
	if (!h) fatal("gethostbyname failed");
	
	s = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (s < 0) fatal("socket");
	memset(&channel, 0, sizeof(channel));
	channel.sin_family = AF_INET;
	memcpy(&channel.sin_addr.s_addr, h->h_addr, h->h_length);
	channel.sin_port = htons(SERVER_PORT);
	
	c = connect(s, (struct sockaddr *) &channel, sizeof(channel));
	if (c < 0) fatal("connect failed");
	
	//Connection is not established. Send file name including 0 byte at end
	write(s, argv[2], strlen(argv[2])+1);
	
	//Go get the file and write it to standard output
	while (1) {
		bytes = read(s, buf, BUF_SIZE); //Read from socket
		if (bytes <= 0) exit(0); //Check for end of file
		write(1, buf, bytes); //Write to standard output
	}
}

fatal(char *string) {
	printf("%s\n", string);
	exit(1);
}
