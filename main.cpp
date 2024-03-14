#include <iostream>
#include <vector>
#include <string>
#include <cstring>
#include "tcp_client.h"
#include "ssl_client.h"

using namespace std;
vector<string> splitHost(char *in) {
	char *token;
	char *next;
	vector<string> out;
	token = strtok_r(in, ":", &next);
	while(token != NULL) {
		out.push_back(string(token));
		token = strtok_r(NULL, ":", &next);	
	}	
	return out;
}
int main(int argc, char **argv) {
	unsigned short port;
	const char *host;
	if(argc < 2) {
		cout << "command: " << argv[0] << " <host>[:port]" << endl;
		return 1;
	}
	vector<string> v = splitHost(argv[1]);
	if(v.size() > 1) {
		port = atoi(v[1].c_str());
		host = v[0].c_str();
	} else {
		host = v[0].c_str();
		port = 443;
	}
	sslClient *sc = new sslClient(host, port);
	sc->doHandShake();
	return 0;
}
