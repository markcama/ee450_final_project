/********************************
Mark Camarena


********************************/
#include "serverB.h"

#define IP_PROTOCOL 0
#define UDP_PORT_NO 31533
#define NET_BUF_SIZE 32 
#define cipherKey 'S' 
#define sendrecvflag 0

// Global Variables
int server_sd;	// server socket descriptor so we can close on SIGINT

// User Constructor
User::User() {
	next_user = NULL;
	prev_user = NULL;
}

// User Contructor
User::User(int userID, string country, vector<int> friend_IDs) {
	this->userID = userID;
	this->country = country;
	this->friend_IDs = friend_IDs;
}
// User Destructor
User::~User() {
	delete next_user;
	delete prev_user;
}

int User::getUserID() {
	return userID;
}

string User::getUserCountry() {
	return country;
}

void User::setFriendsIDs(vector<int> &IDs) {
	this->friend_IDs = IDs;
}

vector<int> User::getFriendsIDs() {
	return friend_IDs;
}
// Country/Graph Constructor
Country::Country() {
	head_user = NULL;
	tail_user = NULL;
}

Country::~Country() {
	User* current = head_user;
	User* next_user;

	while(current != NULL) {
		next_user = current->next_user;
		free(current);
		current = next_user;
	}

	head_user = NULL;
}

void Country::set_name(string country_name) {
	this->name = country_name;
}

string Country::get_name() {
	return name;
}

void Country::append(User* newUser) {

	// Add a new user to the country's linked list
	User* last = head_user;

	newUser->next_user = NULL;

	// if the list is empty
	if(head_user == NULL) {
		newUser->prev_user = NULL;
		head_user = newUser;
		return;
	}

	// else reach the end of the country's list
	while(last->next_user != NULL)
		last = last->next_user;

	// add the user to the end 
	last->next_user = newUser;

}

User* Country::searchUsers(int userID, string country) {

	User *ptr = head_user;

	while(ptr != NULL) {

		if((userID == ptr->getUserID()) && (country == ptr->getUserCountry()))
			return ptr;
		else
			ptr = ptr->next_user;
	}

	return NULL;
}

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

// Signal Handler Function
// Reference: https://beej.us/guide/bgipc/html/multi/signals.html
void sigint_handler(int signum) {
	close(server_sd);
	exit(signum);
}

int main() {

	// Close the socket on keyboard interrupt
	// Reference: https://beej.us/guide/bgipc/html/multi/signals.html
	struct sigaction sa;
	sa.sa_handler = sigint_handler;
	sa.sa_flags = 0;
	sigemptyset(&sa.sa_mask);

    if (sigaction(SIGINT, &sa, NULL) == -1) {
        perror("sigaction");
        exit(1);
    }

	// DECLARATIONS
	// prepare to process data1.txt
	vector<Country*> countries;	// vector of countries/graphs each with their users linked list
	string buffer, country_str, user_ID_str, friend_IDs_str;
	int userID;
	vector<int> friend_IDs_ints;

	// open the file, "data1.txt"
	string filename = "data2.txt";
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

			    // create pointer pointing to a new user, add to country's list
			    User* newUser = new User(userID, 
			    	newCountry.get_name(), 
			    	friend_IDs_ints);
			    newCountry.append(newUser);

			    // peek and see if there's another user to process
			    p = infile.peek();
			}
		}
	}

	// close the file
	infile.close();

	// Connect to the Main Server
	// Reference for Remaining Code: http://beej.us/guide/bgnet/html/

	//Prepare to connect


	// Local Host must be 127.0.0.1
	struct sockaddr_in sa;
	inet_pton(AF_INET, "127.0.0.1", &(sa.sin_addr)); // IPv4

	int status;
	struct addrinfo hints;
	struct addrinfo *servinfo;

	memset(&hints, 0, sizeof hints); 	// make sure the struct is empty
	hints.ai_family = AF_INET;     		// use IPv4
	hints.ai_socktype = SOCK_DGRAM; 	// UDP sockets

	// Error-checking on getaddrinfo. Can we create this socket?
	if ((status = getaddrinfo("127.0.0.1", "31533", &hints, &servinfo)) != 0) {
		fprintf(stderr, "getaddrinfo error: %s\n", gai_strerror(status));
		exit(EXIT_FAILURE);
	}

	// Create the server's socket descriptor
	if ((server_sd = socket(servinfo->ai_family, servinfo->ai_socktype, servinfo->ai_protocol)) == 0) 
    { 
        cerr << "Socket failed." << endl;
        exit(EXIT_FAILURE);
    }

	// Bind it to the port passed into getaddrinfo
	// IP 127.0.0.1 on Port no 30533 (line 214)
	if(bind(server_sd, servinfo->ai_addr, servinfo->ai_addrlen) < 0) {
		cerr << "Could not successfully bind." << endl;
		exit(EXIT_FAILURE);
	}

	freeaddrinfo(servinfo);

	cout << "The server B is up and running using UDP on port " << UDP_PORT_NO << endl;

	while(true) {

	}

	return 0;
}