#include <boost/asio.hpp>

#include "game_server.hpp"

#include "game_session.hpp"

GameSession::GameSession(boost::asio::ip::tcp::socket s,
                         GameServer& gameServer)
    : m_socket(std::move(s)),
      m_gameServer(gameServer),
      m_state(nullptr)
{

}

void GameSession::start()
{
    _doReadHeader();
}

void GameSession::next() {
    _doReadHeader();
}

void GameSession::_doReadHeader() {
}

void GameSession::_doReadContent() {

}

void GameSession::changeState(GameSessionState::GameSessionState* state) {
    m_state.reset(state);
}
