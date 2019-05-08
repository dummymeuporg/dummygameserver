#include <iostream>

#include "master_client_state/initial_state.hpp"

namespace MasterClientState {

InitialState::InitialState(::MasterClient& masterClient)
    : MasterClientState(masterClient)
{ }

void InitialState::onRead(const std::vector<std::uint8_t>& buffer) {
    // Here, buffer should contain a boolean. True if our server name is
    // being recognized by the master server, false otherwise.
    // After that...
    if (buffer[0]) {

    }

}

} // namespace MasterClientState
