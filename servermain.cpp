#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <netdb.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/wait.h>
using namespace std;

#define UDP_SERVER_PORT "32533"
#define TCP_CLIENT_PORT "33533"

int main() {

	// Setup UDP connections for backend server.

	cout << "The Main server is up and running." << endl;

}