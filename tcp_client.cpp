#include <iostream>
#include <cstring>
#include "tcp_client.h"


tcpClient::~tcpClient() {
	::close(m_fd);
}

void tcpClient::dnsLookup() {
	struct addrinfo hints;

	hints.ai_family = AF_INET;
	hints.ai_protocol = SOCK_STREAM;

	struct addrinfo *result = nullptr ;

	bzero(&m_serverIp, INET_ADDRSTRLEN);
	memset(&hints, 0, sizeof(struct addrinfo));
	int succ = getaddrinfo(m_host, nullptr, &hints, &result);
	if(succ != 0) {
		std::cout << "cannot resolve host: " << m_host << std::endl;
		return;
	}

	for(struct addrinfo *p = result; p != nullptr; p = p->ai_next) {
		struct sockaddr_in *sa = (struct sockaddr_in*)p->ai_addr;
		if(nullptr != sa) {
			inet_ntop(p->ai_family, &sa->sin_addr, m_serverIp, sizeof(m_serverIp));
			break;
		}
	}
	std::cout << "resolving host: " << m_host << " -> " << m_serverIp << std::endl;
	freeaddrinfo(result);
}


void tcpClient::connectServer() {
	dnsLookup();
	int sock = socket(AF_INET, SOCK_STREAM, 0);
	if(sock < 0) {
		std::cout << "cannot open socket" << std::endl;
		exit(EXIT_FAILURE);
	}
	const char *ip = (char *) &m_serverIp;
	struct sockaddr_in sin;
	sin.sin_family = AF_INET;
	sin.sin_addr.s_addr = inet_addr(ip);
	sin.sin_port = htons(m_serverPort);
	
	int ret = connect(sock, (struct sockaddr*) &sin, sizeof(sin));
	if(ret < 0) {
		std::cout << "connect() for" << m_serverIp << ":" << m_serverPort << " failed!" << std::endl;
		exit(EXIT_FAILURE);
	}
	m_fd = sock;
	std::cout <<  "connect to " << m_serverIp << ":" << m_serverPort << " succeed!" << std::endl;
}

