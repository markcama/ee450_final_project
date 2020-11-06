#ifndef SERVERA_H
#define SERVERA_H

#include <iostream>
#include <csignal>
#include <fstream>
#include <sstream>
#include <stdio.h>
#include <bits/stdc++.h>
#include <stdlib.h>
#include <vector>
#include <unistd.h>
#include <errno.h>
#include <string>
#include <netdb.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/wait.h>
#include <signal.h>
using namespace std;

class User {

	friend class Country;

private:

	int userID;
	vector<int> friend_IDs;
	string country;
	// User* next_user;
	// User* prev_user;

public:

	User();
	//User(int userID, vector<int> friend_IDs, string country);
	//~User();
	// Adds the given node as next of the current node
    //inline void setNext(User *node);
    // 3. Return pointer of the next node
    //inline User* next();
	void setUser(int id, string country, vector<int> &IDs);
	int getUserID();
	string getUserCountry();
	vector<int> getFriendsIDs();
};

class Country {

private:

	string name;
	// User* head_user;

public:

	//Country();
	//~Country();
	vector <User> users;
	void set_name(string country_name);
	string get_name();
	// void pushBack(User* user);
	// void pushFront(User* user);
	// User* searchUsers(int userID, string country);
};

// use overloaded '=' operator from string class to convert character array to string
string convertToString(char* a, int size);

// take in a string of a users' friends' IDs and convert into a vector for a user object later
vector<int> convertStrToVect(string str);

// get sockaddr, IPv4 or IPv6:
// copied from Beej's tutorial
void *get_in_addr(struct sockaddr *sa);

// Signal Handler Function
void sigint_handler(int signum);

#endif