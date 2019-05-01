#define BOOST_LOG_DYN_LINK 1

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
    } else {
    }
    m_masterClient.next();
}

} // namespace MasterClientState
