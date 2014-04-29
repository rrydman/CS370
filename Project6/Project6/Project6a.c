/*
----------------------------------------------------------------------
CS370 Project 6 - Peterson's Leader Election w/ Sockets
Ross Rydman - (c) April 2014
----------------------------------------------------------------------
To compile: gcc project6a.c
To run: ./a.out
Open multiple shells and enter the nodeid, total nodes, and 
node UID on each shell. Once all nodes have had input done, 
leader election will commence, and shell 0 will show final output.
----------------------------------------------------------------------
This code contains no error checking. Please be correct with your input.
----------------------------------------------------------------------
*/

#include <stdio.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <stdlib.h>

// Globals
typedef enum { false, true } bool;
int phaseNum, nodeid, totalNodes, nodeUID, tempUID;
bool isActiveNode, leader;

// Functions
void findLeader(int uid, int prevDescriptor, int nextDescriptor);
int createServerConnection(char *socketName);
int createClientConnection(char *socketName);

int main() {
	printf("Please enter nodeid: \n");
	scanf("%d", &nodeid);
	printf("Please enter total number of nodes: \n");
	scanf("%d", &totalNodes);
	printf("Please enter UID for current node: \n");
	scanf("%d", &nodeUID);

	// Socket named "zeroNodePrev"
	char zeroNodePrev[] = "zeroNodePrev";
	// Socket named "zeroNodeNext"
	char zeroNodeNext[] = "zeroNodeNext";
	// Socket named "nodeid - 1"
	char nodeMinus1[100];
	sprintf(nodeMinus1, "%d", nodeid - 1);
	// Socket named "nodeid"
	char node[100];
	sprintf(node, "%d", nodeid);

	int prevDescriptor, nextDescriptor;
	if (nodeid == 0) {
		// Node is 0 (first node in ring)
		// Create two server connections: one for node 1, one for node n-1
		nextDescriptor = createServerConnection(zeroNodeNext);
		prevDescriptor = createServerConnection(zeroNodePrev);
	} else if (nodeid == totalNodes - 1) {
		// Node is n-1 (last node in ring)
		// Create two client connections: one for node n-1, one for node 0
		prevDescriptor = createClientConnection(nodeMinus1);
		nextDescriptor = createClientConnection(zeroNodePrev);
	}
	else if (nodeid == 1){
		// Node is 1 (second node in ring)
		// Create one server and one client connection
		// Client points to previous node (node 0)
		// Server points to next node (n)
		prevDescriptor = createClientConnection(zeroNodeNext);
		nextDescriptor = createServerConnection(node);
	} else {
		// Node is any other node (middle nodes in ring)
		// Create one server and one client connection
		// Client points to previous node (n-1)
		// Server points to next node (n)
		prevDescriptor = createClientConnection(nodeMinus1);
		nextDescriptor = createServerConnection(node);
	}
	findLeader(nodeUID, prevDescriptor, nextDescriptor);
	return 0;
}

void findLeader(int nodeUID, int prevDescriptor, int nextDescriptor){
	phaseNum = 1;
	tempUID = nodeUID;
	leader = false;
	isActiveNode = true;
	int oneHopTempUID, twoHopTempUID;

	while (!leader){
		if (isActiveNode){
		// active node:
			// Print phase info
			printf("[%d] [%d] [%d] \n", phaseNum, nodeUID, tempUID);
			// write temp uid
			send(nextDescriptor, &tempUID, sizeof(int), 0);
			// read one hop temp uid
			recv(prevDescriptor, &oneHopTempUID, sizeof(int), 0);
			// write one hop temp uid
			send(nextDescriptor, &oneHopTempUID, sizeof(int), 0);
			// read two hop temp uid
			recv(prevDescriptor, &twoHopTempUID, sizeof(int), 0);

			if (oneHopTempUID == tempUID){
				// this node is the leader
				leader = true;
				printf("Leader found: %d \n", nodeUID);
			} else if (oneHopTempUID > twoHopTempUID && oneHopTempUID > tempUID) {
				// this node continues to be an active node
				// temp uid = one hop temp uid
				tempUID = oneHopTempUID;
			} else {
				// this node becomes a relay node
				isActiveNode = false;
			}
		} else {
		// relay node:
			// read temp uid
			recv(prevDescriptor, &tempUID, sizeof(int), 0);
			// write temp uid
			send(nextDescriptor, &tempUID, sizeof(int), 0);
			// read temp uid
			recv(prevDescriptor, &tempUID, sizeof(int), 0);
			// write temp uid
			send(nextDescriptor, &tempUID, sizeof(int), 0);
		}
		phaseNum++;
	}
}

// From example code
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

// From example code
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


