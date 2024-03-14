#ifndef _TCP_CLIENT_H_
#define _TCP_CLIENT_H_
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <netdb.h>

class tcpClient {

public:
	tcpClient() = delete;
	tcpClient(const char *host, unsigned port) : m_host(host), m_serverPort(port) {}
	virtual ~tcpClient();
	void connectServer();
private:
	void dnsLookup();

private:
	const char *m_host;
	char m_serverIp[INET_ADDRSTRLEN];
	unsigned short m_serverPort;
protected:
	int m_fd;
};


#endif
