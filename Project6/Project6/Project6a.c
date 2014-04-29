/*
----------------------------------------------------
CS370 Project 6 - Sockets
Ross Rydman - (c) April 2014
----------------------------------------------------
To compile: gcc filename.c
To run: ./a.out <0|1>
----------------------------------------------------
*/

#include <stdio.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <stdlib.h>

// Constants
#define SERVER 0
#define CLIENT 1

// Globals
typedef enum { false, true } bool;
int phaseNum, nodeid, totalNodes, nodeUID, tempUID;
bool isActiveNode = true;
bool leader = false;

// Functions
void findLeader(int isActiveNode);
void createNode();
void communicate(int serverDescriptor, int clientDescriptor);
int createServerConnection(char *socketName);
int createClientConnection(char *socketName);
char *getSocketName(int index1, int index2);

int main() {
	// Input
	printf("Please enter nodeid: \n");
	scanf("%d", &nodeid);
	printf("Please enter total number of nodes: \n");
	scanf("%d", &totalNodes);
	printf("Please enter UID for current node: \n");
	scanf("%d", &nodeUID);

	int clientDescriptor1, clientDescriptor2, serverDescriptor;
	char *clientSocketName, *serverSocketName;
	if (nodeid == 0) {
		// Create two server connections: one for node 1, one for node n-1
		clientSocketName = getSocketName(SERVER, CLIENT); // todo: change
		serverSocketName = getSocketName(SERVER, CLIENT); // todo: change
		clientDescriptor1 = createServerConnection(serverSocketName);
		clientDescriptor2 = createServerConnection(clientSocketName);
		//printf("server connection created %d \n",clientDescriptor);
		communicate(clientDescriptor1, clientDescriptor2);
	} else {
		// Create one server and one client connection
		clientSocketName = getSocketName(SERVER, CLIENT); // todo: change
		serverSocketName = getSocketName(SERVER, CLIENT); // todo: change
		// Server is node n+1
		serverDescriptor = createClientConnection(clientSocketName);
		// Client is node n-1
		clientDescriptor1 = createServerConnection(serverSocketName);
		//printf("client connection created %d \n",serverDescriptor);
		communicate(serverDescriptor, clientDescriptor1);
	} 
	return 0;
}

void findLeader(int isActiveNode){
	while (!leader){
		if (isActiveNode){
		// Active node:
			// write temp uid
			// read one hop temp uid
			// write one hop temp uid

			// read two hop temp uid
			// if one hop temp uid == temp uid
				// this node is the leader
			// else if one hop temp uid > two hop temp uid && one hop temp uid > temp uid
				// this node continues to be an active node
				// temp uid = one hop temp uid
			// else
				// this node becomes a relay node
		} else {
		// relay node:
			// read temp uid
			// write temp uid
			// read temp uid
			// write temp uid
		}
	}
}

void createNode(){

}

// Todo: change this to be intelligent
char *getSocketName(int index1, int index2) {
	char *socketName = (char *)calloc(256, sizeof(char));
	socketName = "mySocket";
	return socketName;
}

void communicate(int serverDescriptor, int clientDescriptor) {
	int i = 0;
	while (i<totalNodes)
	{
		if (nodeid == 0){
			int neighborValue;
			//To receive message via socket
			recv(clientDescriptor, &neighborValue, sizeof(int), 0);
			printf("received %d\n", neighborValue);
		}
		else {
			int value = rand() % 100;
			//To send a message via socket
			send(serverDescriptor, &value, sizeof(int), 0);
			printf("sent %d\n", value);
		}
		i++;
	}
}

int createClientConnection(char* socketName) {
	int descriptor;
	//The call to the function ‘socket()’ creates an UN-named socket inside the kernel and returns an integer known as socket descriptor.
	//This function takes domain/family as its first argument. For Internet family of IPv4 addresses we use AF_INET.
	//The second argument ‘SOCK_STREAM’ specifies that the transport layer protocol that we want should be reliable ie it should have acknowledgement techniques. For example : TCP
	//The third argument is generally left zero to let the kernel decide the default protocol to use for this connection. For connection oriented reliable connections, the default protocol used is TCP.
	if ((descriptor = socket(AF_UNIX, SOCK_STREAM, 0)) < 0) {
		printf("Error creating socket [%s]\n", socketName);
		return -1;
	}
	struct sockaddr_un address;
	address.sun_family = AF_UNIX;
	strcpy(address.sun_path, socketName);
	unsigned int addressLength = sizeof(address);
	//Information like IP address of the remote host and its port is bundled up in a structure and a call to function connect() is made which tries to connect this socket with the socket (IP address and port) of the remote host.
	if (connect(descriptor, (struct sockaddr*)&address, addressLength) < 0) {
		printf("Error connecting socket [%s]\n", socketName);
		return -1;
	}
	return descriptor;
}

int createServerConnection(char* socketName) {
	int descriptor;
	//To create endpoint for communication
	if ((descriptor = socket(AF_UNIX, SOCK_STREAM, 0)) < 0) {
		printf("Error creating socket [%s]\n", socketName);
		return -1;
	}
	struct sockaddr_un address;
	address.sun_family = AF_UNIX;
	strcpy(address.sun_path, socketName);
	unsigned int addressLength = sizeof(address);
	//To remove a socket if it exists. Its called before bind()
	unlink(socketName);

	//To assign a local socket address.
	if (bind(descriptor, (struct sockaddr*)&address, addressLength) < 0) {
		printf("Error binding socket [%s]\n", socketName);
		return -1;
	}

	//To instruct a socket to listen for incoming
	//connections. It specifies the number of pending connections
	//that can be queued for a server socket.

	//After the call to listen(), this socket becomes a fully functional listening socket.
	if (listen(descriptor, 1) < 0) {
		printf("Error listening to socket [%s]\n", socketName);
		return -1;
	}
	int connectedDescriptor;
	//To accept the connection request from a client
	if ((connectedDescriptor = accept(descriptor, (struct sockaddr*) &address, &addressLength)) < 0) {
		printf("Error accepting socket connection [%s]\n", socketName);
		return -1;
	}
	return connectedDescriptor;
}


