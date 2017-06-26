/**********************************************************************************************************************************************************************************************
**Monica Pineda 
**CS 372
**May 7, 2017
**Program 1: This is a client-server chat program where the server side is implemented in
**Python and the client side is implemented in C++.
**clietserve.cpp: is the client side of this program. Once the server is started the client side
**can connect to the server and start a conversation. The client side has to start the conversation.
** 
**************************************************************************************************************************************************************************************************/
/***************************************************************************************
**   Title: Beej's Guide to Network Programming
**   Author: Brian “Beej Jorgensen” Hall
**   Date: June 8, 2016
**   Code version: <code version>
**   Availability: https://beej.us/guide/bgnet/output/print/bgnet_USLetter.pdf
**	 Used for Addrinfo and Sockets
***************************************************************************************/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>
/**************************************************************************************************************************************************************
 ** void getUser(char**)
 ** 
 ** Gets the username from the standard input from the user
 ** Args: an empty char array of 10 elements
 ** Returns: nothing
 ************************************************************************************************************************************************************/
void getUser(char * input)

{
		printf("Please enter a username (up to 10 characters):");
		scanf("%s", input);
}
/**************************************************************************************************************************************************************
 ** struct addrinfo ** create_address_info(char**, char**)
 ** 
 ** Creates a pointer to an address info linked list with a address and port
 ** Args: two strings: the address and port number
 ** Returns: An address info linked list
 ************************************************************************************************************************************************************/

struct addrinfo * create_addrinfo(char* input_addr, char* port)
{
	// create satus indicator and structures  for addrinfo 
	int status;
	struct addrinfo clientSide;
	struct addrinfo *res;

	// clear all fields of clientSide
	memset(&clientSide, 0, sizeof clientSide);
	clientSide.ai_family = AF_INET;
	clientSide.ai_socktype = SOCK_STREAM;
// if status is not 0 then an  error exists 
//print error otherwise return the addrinfo  
	if((status = getaddrinfo(input_addr, port, &clientSide, &res))!= 0)
	{
		fprintf(stderr,
				"getaddrinfo has an error: %s\nDid you enter correct IP/Port?\n",
					gai_strerror(status));
		exit(1);
	}
	return res;
}
/**************************************************************************************************************************************************************
 ** int create_socket(struct addrinfo **)
 ** 
 ** Creates a socket from an address info linked list
 ** Args: The address info linked list
 ** Returns: socket file descriptor
 ************************************************************************************************************************************************************/
int create_socket(struct addrinfo * res)
{
	int sockfd;
	// if the socketfd=-1, exit, otherwise return it
	if ((sockfd = socket(res->ai_family, res->ai_socktype, res->ai_protocol)) == -1)
	{
		fprintf(stderr, "Error in creating socket\n");
		exit(1);
	}
	return sockfd;
}
/**************************************************************************************************************************************************************
 ** void connect_socket(int, struct addrinfo **)
 ** 
 ** Connects the socket to the address specified in the address info linked list
 ** Args: a socket file descriptor and an address info linked list
 ** Returns: nothing
 ************************************************************************************************************************************************************/
void connect_socket(int sockfd, struct addrinfo * res)
{
	int status;
	// if the status is -1, we were unable to connect if its not -1 connect the socket to the addrinfo information
	if ((status = connect(sockfd, res->ai_addr, res->ai_addrlen)) == -1)
	{
		fprintf(stderr, "Error in connecting socket\n");
		exit(1);
	}
}
/**************************************************************************************************************************************************************
 ** void chat(int, char**, char**)
 ** 
 ** Initiates a chat between two hosts connected by a socket. The client (this
 ** file) has to initiates the converstion.
 ** Args: a socket file descriptor, a username and a peer's name
 ************************************************************************************************************************************************************/
void chat(int sockfd, char * username, char * servername)
{
	// create buffers for input and output
	char input[503];
	char output[501];
	//clear buffers
	memset(input,0,sizeof(input));
	memset(output,0,sizeof(output));
	//set error dectectors to 0
	int numSent = 0;
	int status;
	// clear out newlines from standard input
	fgets(input, 500, stdin);
	while(1)
	{
		// grab the input from the user
		printf("%s> ", username);
		fgets(input, 502, stdin);
		// if the string is \quit, close the connection
		if (strcmp(input, "\\quit\n") == 0)
		{
			break;
		}
		// if the string is not \quit send information to the peer
		numSent = send(sockfd, input, strlen(input) ,0);
		// if there was an error, exit
		if(numSent == -1)
		{
				fprintf(stderr, "Error sending message to host\n");
				exit(1);
		}
		// grab the message from the peer
		status = recv(sockfd, output, 500, 0);
		// if there was an error receiving a message, exit 
		if (status == -1)
		{
			fprintf(stderr, "Error receiving message from host\n");
			exit(1);
		}
		else if (status == 0)
		{ 
		// the server closed the connection
			printf("Connection closed by server\n");
			break;
		}
		else
		{ 
		// The message was recieved ok and will print out to the screen
			printf("%s> %s\n", servername, output);
		}
		// clear out the input and output buffers for the next iteration
		memset(input,0,sizeof(input));
		memset(output,0,sizeof(output));
	}
	// if we break, we close the connection
	close(sockfd);
	printf("Connection Closed\n");
}
/**************************************************************************************************************************************************************
 ** void handshake(int, char**, char**)
 ** 
 ** intiates an exhange of information between the client and the server. Gives 
 ** the server the client's name and gives the client the server's name
 ** Args: the user's name and an empty array to store the server's name
 ** Return: Nothing
 ************************************************************************************************************************************************************/
void handshake(int sockfd, char * username, char * servername){
	// send our username to the peer
	int sendName = send(sockfd, username, strlen(username), 0);
	// grab the username of our peer and place into servername
	int recieveName = recv(sockfd, servername, 10, 0);
}
/**************************************************************************************************************************************************************
 ** int main(int, char**)
 ** 
 ** main method. must be run with command line arguments:
 **         '<executablename> <hostname> <portnumber>'
 ************************************************************************************************************************************************************/
int main(int argc, char *argv[]){
	// if the user enters too few or too many args, exit and print error
	if(argc != 3)
	{
		fprintf(stderr, "Invalid number of arguments\n");
		exit(1);
	}
	// create a username buffer and get the username from user
	char username[10];
	getUser(username);
	// create addrinfo from the arguments passed in by the user
	struct addrinfo * res = create_addrinfo(argv[1], argv[2]);
	// create a socket from addrinfo
	int sockfd = create_socket(res);
	// connect the socket and the addrinfo 
	connect_socket(sockfd, res);
	// create a peername buffer and exhange username and peername with peer
	char servername[10];
	handshake(sockfd, username, servername);
	// chat 
	chat(sockfd, username, servername);
	// free up the space from addrinfo 
	freeaddrinfo(res);
}
