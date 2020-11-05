#ifndef SERVERA_H
#define SERVERA_H

#include <iostream>
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
using namespace std;

class User {

	friend class Country;

private:

	int userID;
	vector<int> friend_IDs;
	string country;
	User* next_user;
	User* prev_user;

public:

	User();
	User(int userID, string country, vector<int> friend_IDs);
	~User();
	void setUser(int id, string country);
	int getUserID();
	string getUserCountry();

	void setFriendsIDs(vector<int> &IDs);
	vector<int> getFriendsIDs();
};

class Country {

	// friend class Countries;

private:

	string name;
	User* head_user;
	User* tail_user;
	// Country* next_country;
	// Country* prev_country;

public:

	Country();
	~Country();
	void set_name(string country_name);
	string get_name();
	void append(User* newUser);
	User* searchUsers(int userID, string country);
};

// use overloaded '=' operator from string class to convert character array to string
string convertToString(char* a, int size);

// take in a string of a users' friends' IDs and convert into a vector for a user object later
vector<int> convertStrToVect(string str);

#endif