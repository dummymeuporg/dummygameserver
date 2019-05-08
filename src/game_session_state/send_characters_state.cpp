#include "abstract_game_server.hpp"

#include "game_session.hpp"
#include "game_session_state/send_characters_state.hpp"

namespace GameSessionState {

SendCharactersState::SendCharactersState(
    std::shared_ptr<GameSession> gameSession
) : GameSessionState(gameSession)
{
}

void SendCharactersState::resume() {
    // Send the list of characters.
    const ::AbstractGameServer& svr(m_gameSession->gameServer());
    fs::path accountPath(svr.serverPath() /
                         "accounts" / m_gameSession->account()->name());
}

void SendCharactersState::onRead(const std::vector<std::uint8_t>& buffer) {
    // From here, the player has either created a character or selected one.
}

} // namespace GameSessionState
