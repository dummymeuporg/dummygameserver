#include <iostream>
#include <boost/uuid/uuid_io.hpp>

#include "game_session.hpp"

#include "game_session_state/initial_state.hpp"

namespace GameSessionState {

InitialState::InitialState(std::shared_ptr<GameSession> gameSession)
    : GameSessionState(gameSession)
{
    std::cerr << "Initial state instantiated." << std::endl;
}


void InitialState::onRead(const std::vector<std::uint8_t>& buffer) {
    std::cerr << "Received " << buffer.size() << " bytes." << std::endl;
    // The buffer should contain an account name and an uuid.
    if (buffer.size() < 16 + 2)
    {
        // Throw an exception?
        return;
    }
    const std::uint16_t* accountLength =
        reinterpret_cast<const std::uint16_t*>(buffer.data());

    // Check against the accountLength
    if (*accountLength > buffer.size() - (16 + 2)) {
        // Throm an exception?
        return;
    }

    std::string accountName(
        reinterpret_cast<const char*>(buffer.data()) + sizeof(std::uint16_t),
                            *accountLength);
    boost::uuids::uuid sessionID;
    std::copy(
        buffer.begin() + sizeof(std::uint16_t) + *accountLength,
        buffer.end(),
        sessionID.data
    );

    std::cerr << "Account is: " << accountName << std::endl;
    std::cerr << "Session id is: " << sessionID << std::endl;
}

} // namespace GameSessionState
