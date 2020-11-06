/********************************
Mark Camarena


********************************/
#include "serverA.h"

#define IP_PROTOCOL 0
#define UDP_PORT_NO "30533"
#define IP_ADDR "127.0.0.1"

#define MAXBUFLEN 250

// Global Variables
int server_sd;	// server socket descriptor so we can close on SIGINT

// User Constructor
User::User() {
	int userID = 0;
	string country = "";
}

// User Contructor
// User::User(int userID, vector<int> &friend_IDs, string country) {
// 	this->userID = userID;
// 	this->country = country;
// 	this->friend_IDs = friend_IDs;
// }

void User::setUser(int id, string country, vector<int> &IDs) {
	this->userID = id;
	this->country = country;
	this->friend_IDs = IDs;
}
// User Destructor
// User::~User() {
// 	delete next_user;
// 	delete prev_user;
// }

// 1. Adds the given node as next of the current node
// inline void User::setNext(User *node) {
// 	this->next_user = node;
// }

// // Return pointer of the next node
// inline User* User::next() {
//     return this->next_user;
// }

int User::getUserID() {
	return userID;
}

string User::getUserCountry() {
	return country;
}

// void User::setFriendsIDs(vector<int> &IDs) {
// 	this->friend_IDs = IDs;
// }

vector<int> User::getFriendsIDs() {
	return friend_IDs;
}
// Country/Graph Constructor
// Country::Country() {
// 	head_user = NULL;
// }

// Country::~Country() {
// 	User* current = head_user;
// 	User* next_user;

// 	while(current != NULL) {
// 		next_user = current->next_user;
// 		free(current);
// 		current = next_user;
// 	}

// 	head_user = NULL;
// }

void Country::set_name(string country_name) {
	this->name = country_name;
}

string Country::get_name() {
	return name;
}

// void Country::pushBack(User *user) {
// 	// Iterate through the end of the list and set new node as next
//     // of last node
//     // (Current List)->(Node)

// 	// If list's empty, made new node the head and return
// 	if(head_user == NULL) {
// 		head_user = user;
// 	}

// 	User *current = head_user;

// 	while(current && current->next()) {
// 		current = current->next();
// 	}

// 	// At this point current is the last node
//     // Set node as the next of the last node

//     current->setNext(user);
// }

// void Country::pushFront(User* user) {
// 	// Set Next of Node to Head of the current list.
//     //  (Node)->(Current List)
//     user->setNext(head_user);
//     head_user = user;
// }

// User* Country::searchUsers(int userID, string country) {

// 	User *ptr = head_user;

// 	while(ptr != NULL) {

// 		if((userID == ptr->getUserID()) && (country == ptr->getUserCountry()))
// 			return ptr;
// 		else
// 			ptr = ptr->next_user;
// 	}

// 	return NULL;
// }

// Reference: https://www.geeksforgeeks.org/convert-character-array-to-string-in-c/
// use overloaded '=' operator from string class to convert character array to string 
string convertToString(char* a, int size) 
{ 
    string s = a; 
    return s; 
} 

// take in a string of a users' friends' IDs and convert into a vector of ints (IDs)
vector<int> convertStrToVect(string str) {

	stringstream ss(str);
	vector<int> friends;
	string ID;

	while(ss >> ID) {
		friends.push_back(stoi(ID));
	}

	return friends;
}

string listCountries(vector<Country> &countries) {

	string msg;
	string str;

	// gather all the country names into one giant str
	// separate them by a ","
	for (int i = 0; i < countries.size(); i++) {

		str = countries[i].get_name() + ",";
		msg += str;
	}

	return msg;
}

// get sockaddr, IPv4 or IPv6:
// copied from Beej's tutorial
void *get_in_addr(struct sockaddr *sa)
{
	if (sa->sa_family == AF_INET) {
		return &(((struct sockaddr_in*)sa)->sin_addr);
	}

	return &(((struct sockaddr_in6*)sa)->sin6_addr);
}

// Signal Handler Function
// Reference: https://beej.us/guide/bgipc/html/multi/signals.html
void sigint_handler(int signum) {

	close(server_sd);
	exit(signum);

}

int main() {

	// Close the socket on keyboard interrupt
	// Reference: https://beej.us/guide/bgipc/html/multi/signals.html
	struct sigaction sig_act;
	sig_act.sa_handler = sigint_handler;
	sig_act.sa_flags = 0;
	sigemptyset(&sig_act.sa_mask);

    if (sigaction(SIGINT, &sig_act, NULL) == -1) {
        perror("sigaction");
        exit(1);
    }

	// DECLARATIONS
	// prepare to process data1.txt
	vector<Country> countries;	// vector of countries/graphs each with their users linked list
	string buffer, country_str, user_ID_str, friend_IDs_str;
	int userID;
	vector<int> friend_IDs_ints;

	// open the file, "data1.txt"
	string filename = "data1.txt";
	ifstream infile (filename.c_str());

	// Can't Open?
	if(infile.fail()) {
		cout << filename << " not found." << endl;
		return 1;
	}

	while(getline(infile, buffer)) {	// keep reading the file


		char* buff_array;
		buff_array = &buffer[0];	// convert buffer string into a char array for isalpha()

		if(isalpha(buff_array[0])) {	// We have found the name of a new country!

			// convert the country's name (char array) back into string 
			int country_name_size = sizeof(buff_array) / sizeof(char);
			country_str = convertToString(buff_array, country_name_size);

			// create new country object and assign its name
			Country newCountry;
			newCountry.set_name(country_str);

			// look for new users without taking anything out of the stream
			char p = infile.peek();


			while(isdigit(p)) {	// do this for every new user we find under a country

				// take the next string out of the stream
				getline(infile, buffer);

				// split it by the user's ID and their friends' IDs (if any)
				user_ID_str = buffer.substr(0, buffer.find(' '));
				friend_IDs_str = buffer.substr(buffer.find(' ') + 1);

				// convert strings to an int and a vector<int> respectively
				userID = stoi(user_ID_str);
				friend_IDs_ints = convertStrToVect(friend_IDs_str);

				// Create new user
				User newUser;
				newUser.setUser(userID, country_str, friend_IDs_ints);

				// add it to the list of countries
				newCountry.users.push_back(newUser);

			    // peek and see if there's another user to process
			    p = infile.peek();

			}

			countries.push_back(newCountry);
		}
	}

	// close the file
	infile.close();

	// Connect to the Main Server
	// Reference for Remaining Code: http://beej.us/guide/bgnet/html/

	//Prepare to connect

	// Local Host must be 127.0.0.1
	// copied from Beej's tutorial
	struct sockaddr_in sa;
	inet_pton(AF_INET, IP_ADDR, &(sa.sin_addr)); // IPv4
	int status, numbytes;
	struct addrinfo hints, *servinfo, *p;

	char ip4_main[INET_ADDRSTRLEN];
	struct sockaddr_storage their_addr;
	char buf[MAXBUFLEN];
	socklen_t addr_len;

	memset(&hints, 0, sizeof hints); 	// make sure the struct is empty
	hints.ai_family = AF_INET;     		// use IPv4
	hints.ai_socktype = SOCK_DGRAM; 	// UDP sockets
	// Error-checking on getaddrinfo. Can we create this socket?
	// copied from Beej's tutorial
	if ((status = getaddrinfo(IP_ADDR, UDP_PORT_NO, &hints, &servinfo)) != 0) {
		fprintf(stderr, "getaddrinfo error: %s\n", gai_strerror(status));
		return 1;
	}

	// loop through all the results and bind to the first we can
	// copied from Beej's tutorial
	for(p = servinfo; p != NULL; p = p->ai_next) {

		if ((server_sd = socket(p->ai_family, p->ai_socktype,p->ai_protocol)) == -1) {
			perror("Socket");
			continue;
		}

		if (bind(server_sd, p->ai_addr, p->ai_addrlen) == -1) {
			close(server_sd);
			perror("Bind");
			continue;
		}

		break;
	}

	// copied from Beej's
	if (p == NULL) {	// could not bind to anything
		cerr << "Failed to bind socket" << endl;
		return 2;
	}

	freeaddrinfo(servinfo);	// free up the address info LL

	cout << "The server A is up and running using UDP on port " << UDP_PORT_NO << endl;

	// Server A looks out for this message
	// to signal it to send Main Server its list of countries.
	const char *on_msg = "Server A, send me your country list.";

	while(true) {

		addr_len = sizeof their_addr;
	    if ((numbytes = recvfrom(server_sd, buf, MAXBUFLEN-1 , 0,
	        (struct sockaddr *)&their_addr, &addr_len)) == -1) {
	        perror("recvfrom");
	        exit(1);
    	}

    	inet_ntop(their_addr.ss_family,
    		get_in_addr((struct sockaddr *)&their_addr),
            ip4_main, sizeof ip4_main);

    	//printf("listener: got packet from %s\n",
			// inet_ntop(their_addr.ss_family,
			// get_in_addr((struct sockaddr *)&their_addr),
			// ip4_main, sizeof ip4_main);

		buf[numbytes] = '\0';
		//printf("listener: packet contains \"%s\"\n", buf);


    	if(strcmp(buf, on_msg) == 0) {

    	}
	}

	return 0;
}