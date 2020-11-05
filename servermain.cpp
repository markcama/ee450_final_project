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

#define UDP_SERVERA_PORT "30533"
#define UDP_SERVERB_PORT "31533"
#define UDP_MAINSERVER_PORT "32533"
#define TCP_CLIENT_PORT "33533"

#define IP_ADDR "127.0.0.1"

// Global Variables
int sockfd;
struct addrinfo *servinfo;

// Signal Handler Function
// Reference: https://beej.us/guide/bgipc/html/multi/signals.html
void sigint_handler(int signum) {
	freeaddrinfo(servinfo);
	close(sockfd);
	exit(signum);
}

int main() {

	// Setup UDP connections for backend server.
	// Reference: http://beej.us/guide/bgnet/html/#connectman
    struct addrinfo hints, *p;
    int rv;
    int numbytes;

    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_DGRAM;

    if ((rv = getaddrinfo(BACK_IP_ADDR, UDP_SERVERA_PORT, &hints, &servinfo)) != 0) {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
        return 1;
    }

    // loop through all the results and make a socket
    for(p = servinfo; p != NULL; p = p->ai_next) {
        if ((sockfd = socket(p->ai_family, p->ai_socktype,
                p->ai_protocol)) == -1) {
            perror("Socket");
            continue;
        }

        break;
    }

    if (p == NULL) {
        fprintf(stderr, "Failed to create socket\n");
        return 2;
    }

	cout << "The Main server is up and running." << endl;

}