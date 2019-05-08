#pragma once

#include <cstdint>
#include <vector>

#include "master_client_state/master_client_state.hpp"

namespace MasterClientState
{

class EncryptKeyError : public std::exception {
public:
    virtual const char* what() const throw() {
        return "Error while encrypting master key.";
    }
};

class InitialState : public MasterClientState {
public:
    InitialState(::MasterClient&);
    virtual void onRead(const std::vector<std::uint8_t>&) override;

private:
    std::vector<std::uint8_t> _readMasterKey() const;
    std::vector<std::uint8_t>
        _encryptMasterKey(const std::vector<std::uint8_t>&) const;
    void _sendEncryptedMasterKey(const std::vector<std::uint8_t>&) const;

};

}
