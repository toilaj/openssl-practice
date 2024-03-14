#include <iostream>
#include <openssl/ssl.h>
#include <openssl/err.h>

#include "tcp_client.h"

class sslClient : protected tcpClient {
public:
	sslClient() = delete;
	sslClient(const char *ip, unsigned short port);
	void showServerCert();
	virtual ~sslClient();
	void doHandShake();
private:
	SSL_CTX *m_sslCtx;
	SSL *m_ssl;
	X509 *m_cert;
};	
