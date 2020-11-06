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
#define SEND_RCV_FLAG 0

#define IP_ADDR "127.0.0.1"

// Global Variables
int sockfd;
struct addrinfo *servinfo;

// Ensures entire buffer/message is sent
// Reference: http://beej.us/guide/bgnet/html/#sendall
int sendall(int socket_fd, char *buf, int *len)
{
    int total = 0;        // how many bytes we've sent
    int bytesleft = *len; // how many we have left to send
    int n;

    while(total < *len) {
        n = send(socket_fd, buf+total, bytesleft, 0);
        if (n == -1) { break; }
        total += n;
        bytesleft -= n;
    }

    *len = total; // return number actually sent here

    return n==-1?-1:0; // return -1 on failure, 0 on success
} 

// Functions like strlen(), but this one accepts type pointer to char
unsigned int charstar_strlen(char *p) {
    
    unsigned int count = 0;
    while(*p!='\0')
    {
        count++;
        p++;
    }

    return count;
}

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
    struct sockaddr_storage their_addr;

    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_DGRAM;

    // prepare for messages to Server A
    // For now Main Server is talking to Backend
    if ((rv = getaddrinfo(IP_ADDR, UDP_SERVERA_PORT, &hints, &servinfo)) != 0) {
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

    if (p == NULL) {	// did socket creation fail?
        fprintf(stderr, "Failed to create socket\n");
        return 2;
    }

    cout << "The Main server is up and running." << endl;

    // create a message to cue Server A to send their country list
    const char *on_msg = "Server A, send me your country list.";
    int msg_len = strlen(on_msg);

    // send the message and check if its successful
    int bytes_sent;
    if ((bytes_sent = sendto(sockfd, on_msg, msg_len, 0,
             p->ai_addr, p->ai_addrlen)) == -1) {
        perror("servermain: sendto");
        exit(1);
    }


}