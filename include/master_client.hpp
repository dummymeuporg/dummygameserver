#pragma once

#include <cstdint>
#include <vector>

#include <boost/asio.hpp>
#include <boost/asio/io_service.hpp>
#include <boost/filesystem.hpp>

// Should I encapsulate this somehow?
#include <openssl/pem.h>

#include "crypto/rsa_keypair.hpp"
#include "master_client_state/master_client_state.hpp"

using boost::asio::ip::tcp;
namespace fs = boost::filesystem;


class MasterClient {
public:
    MasterClient(boost::asio::io_service&,
                 const tcp::resolver::results_type& endpoints,
                 fs::path, fs::path, fs::path,
                 const char*);
    ~MasterClient();
    void changeState(MasterClientState::MasterClientState* state);
    void next();

    ::EVP_PKEY const* masterPublicKey() const {
        return m_masterPublicKey;
    }

    Dummy::Crypto::RSAKeyPair& rsaKeyPair() {
        return m_rsaKeyPair;
    }

    const fs::path& configDir() const {
        return m_configDir;
    }

private:

    void _doConnect(const tcp::resolver::results_type& endpoints);
    void _loadMasterKey();
    void _encryptMasterKey();
    void _sendPreambleHeader();
    void _sendPreambleContent();
    void _doReadHeader();
    void _doReadContent();

    boost::asio::io_service& m_ioService;
    tcp::socket m_socket;
    fs::path m_projectDir, m_accountsDir, m_configDir;
    std::string m_serverName;
    EVP_PKEY* m_masterPublicKey;
    Dummy::Crypto::RSAKeyPair m_rsaKeyPair;
    std::unique_ptr<MasterClientState::MasterClientState> m_state;
    std::uint16_t m_header;
    std::vector<std::uint8_t> m_payload;
};
