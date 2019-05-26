#include <iostream>

#include <boost/asio.hpp>

#include "game_session_state/initial_state.hpp"
#include "abstract_game_server.hpp"
#include "abstract_game_server.hpp"
#include "game_session.hpp"

GameSession::GameSession(boost::asio::ip::tcp::socket s,
                         AbstractGameServer& gameServer)
    : m_socket(std::move(s)),
      m_gameServer(gameServer),
      m_state(nullptr),
      m_account(nullptr)      
{

}

void GameSession::start()
{
	m_state = std::make_shared<GameSessionState::InitialState>(
		shared_from_this()
	);
    _doReadHeader();
}

void GameSession::next() {
    _doReadHeader();
}

void GameSession::_doReadHeader() {
    auto self(shared_from_this());
    boost::asio::async_read(
        m_socket,
        boost::asio::buffer(&m_header, sizeof(std::uint16_t)),
        [this, self](boost::system::error_code ec, std::size_t length)
        {
            if (!ec)
            {
                m_payload.resize(m_header);
                _doReadContent();
            }
        }
    );
}

void GameSession::_doReadContent() {
    auto self(shared_from_this());
    boost::asio::async_read(
        m_socket,
        boost::asio::buffer(m_payload, m_header),
        [this, self](boost::system::error_code ec, std::size_t length)
        {
            if (!ec)
            {
                std::cerr << "Read " << length << " bytes.";
                Dummy::Protocol::IncomingPacket pkt(m_payload);
                m_state->onRead(pkt);
            }
        }
	);
}

void GameSession::changeState(
    std::shared_ptr<GameSessionState::GameSessionState> state
)
{
    m_state = state;
    m_state->resume();
}

void GameSession::setPlayer(std::shared_ptr<::Player> player) {
    m_player = player;
}

void GameSession::setAccount(std::shared_ptr<Dummy::Core::Account> account) {
   m_account = account; 
}
