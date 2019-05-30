#include "abstract_game_server.hpp"
#include "game_session.hpp"
#include "game_session_state/loading_state.hpp"
#include "game_session_state/playing_state.hpp"
#include "player.hpp"
#include "server_map.hpp"

namespace GameSessionState {

LoadingState::LoadingState(std::shared_ptr<GameSession> gameSession,
                           Dummy::Protocol::TeleportRequest&& teleportRequest)
    : GameSessionState(gameSession),
      m_teleportRequest(std::move(teleportRequest))
{

}

void LoadingState::resume() {
    std::cerr << "[LoadingState]" << std::endl;
}

void LoadingState::onRead(Dummy::Protocol::IncomingPacket& pkt) {
    ::AbstractGameServer& srv(m_gameSession->gameServer());
    std::shared_ptr<::Player> player(m_gameSession->player());
    std::string mapName;
    std::pair<std::uint16_t, std::uint16_t> position;
    Dummy::Protocol::OutgoingPacket outPkt;
    std::uint8_t answer;

    pkt >> mapName >> position.first >> position.second;

    std::cerr << "Teleport at " << mapName << "(" <<
        position.first << "," << position.second << ")" << std::endl;

    if (m_teleportRequest.destinationMap() == mapName &&
        m_teleportRequest.position() == position)
    {
        std::cerr << "Request correct. Teleport. " << std::endl;
        // XXX: insert player in the servermap.
        std::shared_ptr<::ServerMap> previousServerMap =
            player->serverMap();
        std::shared_ptr<::ServerMap> newServerMap =
            srv.serverMap(mapName);

        // Can happen, mostly when the player just connected.
        if (nullptr != previousServerMap) {
            previousServerMap->removePlayer(player);
        }
        newServerMap->addPlayer(player);
        
        answer = 1;
        outPkt << answer;
        _answerSuccess(outPkt);
        
    } else {
        std::cerr << "Request incorrect." << std::endl;
    }
}

void LoadingState::_answerSuccess(const Dummy::Protocol::OutgoingPacket& pkt)
{
    auto self(shared_from_this());
    boost::asio::async_write(
        m_gameSession->socket(),
        boost::asio::buffer(pkt.buffer(), pkt.size()),
        [self, this](boost::system::error_code ec, std::size_t length)
        {
            if (!ec) {
                /* Switch to game state. */
                m_gameSession->changeState(
                    std::make_shared<PlayingState>(m_gameSession)
                );
                m_gameSession->next();
            }
        }
    );
}

} // namespace GameSessionState
