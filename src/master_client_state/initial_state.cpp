#include "master_client_state/initial_state.hpp"

namespace MasterClientState
{

InitialState::InitialState(::MasterClient& masterClient)
    : MasterClientState(masterClient)
{

}

void InitialState::onRead(const std::vector<std::uint8_t>& buffer) {

}

} // namespace MasterClientState
