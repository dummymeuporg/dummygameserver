#pragma once

#include <boost/asio.hpp>
#include <boost/asio/io_service.hpp>
#include <boost/filesystem.hpp>

// Should I encapsulate this somehow?
#include <openssl/pem.h>

#include "crypto/rsa_keypair.hpp"

using boost::asio::ip::tcp;
namespace fs = boost::filesystem;


class MasterClient {
public:
    MasterClient(boost::asio::io_service&, fs::path, fs::path, fs::path);

private:

    void _loadRSAKeyPair();

    boost::asio::io_service& m_ioService;
    tcp::socket m_socket;
    fs::path m_projectDir, m_accountsDir, m_configDir;
    EVP_PKEY* m_masterPublicKey;
    Dummy::Crypto::RSAKeyPair m_rsaKeyPair;
};
