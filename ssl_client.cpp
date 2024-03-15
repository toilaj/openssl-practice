#include <iostream>
#include "ssl_client.h"

sslClient::sslClient(const char *ip, unsigned short port) : tcpClient(ip, port) {
	const char *ca = "/etc/ssl/certs";
	SSL_library_init();
	SSL_load_error_strings();
	m_sslCtx = SSL_CTX_new(TLS_client_method());
	if(m_sslCtx == nullptr) {
		ERR_print_errors_fp(stdout);
		exit(EXIT_FAILURE);
	}	
	m_ssl = SSL_new(m_sslCtx);
	SSL_CTX_set_verify(m_sslCtx, SSL_VERIFY_PEER, nullptr);
	if(SSL_CTX_load_verify_locations(m_sslCtx, nullptr, ca) != 1){
		SSL_CTX_free(m_sslCtx);
		ERR_print_errors_fp(stdout);
		exit(EXIT_FAILURE);
	}
	connectServer();
	SSL_set_fd(m_ssl, m_fd);
}

sslClient::~sslClient() {
	SSL_shutdown(m_ssl);
	SSL_free(m_ssl);
	SSL_CTX_free(m_sslCtx);
}

void sslClient::doHandShake() {
	int ret = SSL_connect(m_ssl);
	if(ret != 1) {
		std::cout << "SSL_connect failed!" << std::endl;
		return;
	}
	if(SSL_get_verify_result(m_ssl) != X509_V_OK) {
		fprintf(stderr, "Verification Error: %s\n", X509_verify_cert_error_string(SSL_get_verify_result(m_ssl)));
		ERR_print_errors_fp(stderr);
        	SSL_CTX_free(m_sslCtx);
        	return;
	}
	m_cert = SSL_get_peer_certificate(m_ssl);
	std::cout << "SSL_connect succeed!" << std::endl;
	showServerCert();
//	char buf[1024] = {0};
//	SSL_write(m_ssl, "hello", 5);
//	int recv = SSL_read(m_ssl, buf, sizeof(buf) - 1);
//	if(recv > 0) {
//		std::cout << buf << std::endl;
//	}
}

void sslClient::showServerCert() {
	if(m_cert) {
		std::cout << "subject name: " << X509_NAME_oneline(X509_get_subject_name(m_cert), 0, 0) << std::endl; 
		std::cout << "issuser name: " << X509_NAME_oneline(X509_get_issuer_name(m_cert), 0, 0) << std::endl;

	}
}	
