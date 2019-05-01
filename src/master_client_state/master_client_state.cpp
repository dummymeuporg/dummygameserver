#include "master_client.hpp"

#include "master_client_state/master_client_state.hpp"

namespace MasterClientState
{

MasterClientState::MasterClientState(::MasterClient& masterClient)
    : m_masterClient(masterClient)
{
}

void MasterClientState::onRead(const std::vector<std::uint8_t>& buffer) {

}

} // namespace MasterClientState
