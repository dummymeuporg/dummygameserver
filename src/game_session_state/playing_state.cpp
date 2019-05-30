#include "player.hpp"
#include "game_session.hpp"
#include "game_session_state/playing_state.hpp"
#include "server_map.hpp"

namespace GameSessionState {

PlayingState::PlayingState(std::shared_ptr<GameSession> gameSession)
    : GameSessionState(gameSession)
{
}

void PlayingState::resume() {
    // XXX: send the map state.

}

void PlayingState::onRead(Dummy::Protocol::IncomingPacket& pkt) {
    std::uint16_t command;
    pkt >> command;
    std::cerr << "Command: " << static_cast<int>(command) << std::endl;
    switch(command) {
    case 1:
        /* Move */
        _onMove(pkt);
        break;
    default:
        break;
    }
}

void PlayingState::_onMove(Dummy::Protocol::IncomingPacket& pkt) {
    std::shared_ptr<::Player> player(m_gameSession->player());
    std::shared_ptr<::ServerMap> serverMap(player->serverMap());
    std::uint16_t x, y;
    std::uint8_t answer = 1;
    pkt >> x >> y;
    std::cerr << "Move to " << x << "," << y << std::endl; 
    
    if (serverMap->isBlocking(x, y)) {
        std::cerr << "[!] Wrong position!" << std::endl;
    } else {
        player->setPosition(x, y);
    }
    Dummy::Protocol::OutgoingPacket outPkt;
    outPkt << answer;
    _answer(outPkt);
}

void PlayingState::_answer(const Dummy::Protocol::OutgoingPacket& pkt) {
    auto self(shared_from_this());
    boost::asio::async_write(
        m_gameSession->socket(),
        boost::asio::buffer(pkt.buffer(), pkt.size()),
        [this, self](boost::system::error_code ec, std::size_t length) {
            if (!ec) {
                m_gameSession->next();
            }
        }
    );
}

} // namespace GameSessionState
