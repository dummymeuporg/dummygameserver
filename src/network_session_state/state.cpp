#include "network_session.hpp"
#include "network_session_state/state.hpp"

namespace NetworkSessionState {

State::State(::NetworkSession& networkSession) :
    m_networkSession(networkSession)
{
}

} // namespace NetworkSessionState
