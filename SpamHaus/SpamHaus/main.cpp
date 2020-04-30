#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <iostream>
#include <string>
#include <vector>
#include "winsock2.h"
#include <ws2tcpip.h>

using namespace std;

#pragma comment (lib, "ws2_32.lib")
const char* spamhaus = "zen.spamhaus.org";

string convertIPtoHost(const char* ip) {
	string result = "";
	vector<string> tmp;
	string octet = "";
	for (size_t i = 0; ip[i]; ++i) {
		if (ip[i] == '.') {
			tmp.push_back(octet);
			octet = "";
			continue;
		}
		octet += ip[i];
	}
	tmp.push_back(octet);
	for (size_t i = 0; i < tmp.size(); ++i) {
		result += tmp[tmp.size() - i - 1] + '.';
	}
	result += spamhaus;
	return result;
}

//the difference between return codes are in the last octet
string getDNSBLDescription(const char* ip) {
	string result;
	const char* lastOctet = ip + strlen(ip) - 1;
	while (*(lastOctet - 1) != '.') {
		--lastOctet;
	}
	if (!strcmp(lastOctet, "2")) {
		result = "SBL - Spamhaus SBL Data";
	}
	else if (!strcmp(lastOctet, "3")) {
		result = "SBL - Spamhaus SBL CSS Data";
	}
	else if (!strcmp(lastOctet, "4")) {
		result = "XBL - CBL Data";
	}
	else if (!strcmp(lastOctet, "9")) {
		result = "SBL - Spamhaus DROP/EDROP Data";
	}
	else if (!strcmp(lastOctet, "10")) {
		result = "PBL - ISP Maintained";
	}
	else if (!strcmp(lastOctet, "11")) {
		result = "PBL - Spamhaus Maintained";
	}
	else {
		result = "";
	}

	return result;
}

int main(int argc, char* argv[]) {
	char answ[NI_MAXHOST + 1];
	if (argc < 2) {
		cerr << "Not enough arguments";
		return 0;
	}


	WORD wVersion = MAKEWORD(2, 0);
	WSADATA data;

	if (WSAStartup(wVersion, &data) != 0) {
		return 1;
	}

	for (int i = 1; argv[i]; ++i) {
		addrinfo addr;
		addrinfo * result = nullptr;
		std::memset(&addr, 0, sizeof(addrinfo));

		addr.ai_family = AF_INET;
		addr.ai_socktype = SOCK_DGRAM;

		string s = convertIPtoHost(argv[i]);
		//string s = convertIPtoHost("114.231.105.37");
		const char* a = s.c_str();
		int info = getaddrinfo(a, nullptr, &addr, &result);
		if (info) {
			cerr << "The IP adress: " << argv[i] << " is NOT found in the Spamhaus blacklists." << endl;
			continue;
		}
		
		cout << "The IP address: " << argv[i] << " is found in the following Spamhaus public IP zone: ";
		for (addrinfo* r = result; r != nullptr; r = r->ai_next) {
			int res = getnameinfo(r->ai_addr, r->ai_addrlen, answ, NI_MAXHOST, nullptr, 0, NI_NUMERICHOST);
			if (res) {
				cerr << "An error getting information occured" << endl;
				continue;
			}
			cout << answ << " - " << getDNSBLDescription(answ) << endl;
		}
		freeaddrinfo(result);
	}

	WSACleanup();
	return 0;
}