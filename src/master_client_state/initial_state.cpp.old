#define BOOST_LOG_DYN_LINK 1

#include <iostream>

 #include <openssl/evp.h>
 #include <openssl/rsa.h>
 #include <openssl/engine.h>

#include <boost/log/trivial.hpp>

#include "master_client.hpp"

#include "master_client_state/initial_state.hpp"

namespace MasterClientState
{

InitialState::InitialState(::MasterClient& masterClient)
    : MasterClientState(masterClient)
{
    // Do nothing, since we are not connected yet.
}

void InitialState::onRead(const std::vector<std::uint8_t>& buffer) {
    // Here, buffer should contain a boolean. True if our server name is
    // being recognized by the master server, false otherwise.
    // After that, we shall send the encrypted master key.
    if (buffer[0]) {
        std::cerr << "Server recognized. Send encrypted master key."
            << std::endl;

		// TODO: handle exceptions?
        std::vector<std::uint8_t> masterKey(std::move(_readMasterKey()));

		try {
			std::vector<std::uint8_t> encryptedMasterKey(
				std::move(_encryptMasterKey(masterKey))
			);

			_sendEncryptedMasterKey(encryptedMasterKey);
		} catch(const EncryptKeyError& e) {
			std::cerr << "Error while encrypting the master key: " <<
				e.what() << std::endl;
		}

    } else {
    }
    m_masterClient.next();
}

std::vector<std::uint8_t> InitialState::_readMasterKey() const {
    std::vector<std::uint8_t> buf;
	fs::path path(m_masterClient.configDir() / "master.pem");
	size_t filesize = fs::file_size(path);

	std::cerr << "filesize = " << filesize << std::endl;

	buf.resize(filesize);

    std::fstream fpub(path.string().c_str());

    if (!fpub.good()) {
        std::cerr << "Could not open the file." << std::endl;
        throw Dummy::Crypto::PEMFileError();
    }

	fpub.read(reinterpret_cast<char*>(buf.data()), filesize);

	fpub.close();

    return buf;
}

std::vector<std::uint8_t>
InitialState::_encryptMasterKey(const std::vector<uint8_t>& masterKey) const {
	size_t outLen;
	std::vector<std::uint8_t> encryptedMasterKey;
    ENGINE* engine = ::ENGINE_get_default_RSA();
    EVP_PKEY_CTX* ctx = 
        ::EVP_PKEY_CTX_new(m_masterClient.rsaKeyPair().privateKey(), engine);

    if (nullptr == ctx) {
        std::cerr << "Error with EPV_PKEY_CTX_new()." << std::endl;
        throw EncryptKeyError();
    }

    if(::EVP_PKEY_decrypt_init(ctx) <= 0) {
        std::cerr << "Error with EVP_PKEY_encrypt_init()." << std::endl;
        ::EVP_PKEY_CTX_free(ctx);
        throw EncryptKeyError();
    }

    if (::EVP_PKEY_CTX_set_rsa_padding(ctx, RSA_NO_PADDING) <= 0) {
		std::cerr << "Error with EVP_PKEY_CTX_set_rsa_padding. " << std::endl;
		::EVP_PKEY_CTX_free(ctx);
        throw EncryptKeyError();
	}

	// This first pass is only useful to get outlen.
    // Here we use the decrypt method since we use our private key.
    // We actually use our private key to "encrypt" the blob, so the master
    // server will "decrypt" this blob using the public key. This operation
    // will let us proove our identity to the master server.
    // ... I should move this comment elsewhere, anyway.
	if (::EVP_PKEY_decrypt(ctx,
						   NULL,
						   &outLen,
						   masterKey.data(),
						   masterKey.size()) <= 0)
	{
		std::cerr << "Error with EVP_PKEY_decrypt." << std::endl;
		::EVP_PKEY_CTX_free(ctx);
        throw EncryptKeyError();
	}

	encryptedMasterKey.resize(outLen);

	// Encrypt the master key eventually.
	if (::EVP_PKEY_decrypt(ctx,
						   encryptedMasterKey.data(),
						   &outLen,
						   masterKey.data(),
						   masterKey.size()) <= 0)
	{
		std::cerr << "Error with second EVP_PKEY_encrypt." << std::endl;
		::EVP_PKEY_CTX_free(ctx);
        throw EncryptKeyError();
	}

	// Here, the key should be encrypted.

	return encryptedMasterKey;
}

void
InitialState::_sendEncryptedMasterKey(
    const std::vector<std::uint8_t>& key
) const {
    std::cerr << "Will send a blob of " << key.size()
        << " bytes. " << std::endl;
}

} // namespace MasterClientState
