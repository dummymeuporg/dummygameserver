#include "master_client.hpp"

#include "master_client_state/master_client_state.hpp"

namespace MasterClientState
{

MasterClientState::MasterClientState(::MasterClient& masterClient)
    : m_masterClient(masterClient)
{
}

} // namespace MasterClientState
