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
	m_state = std::make_shared<GameSessionState::ItinialState>(
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
                m_state->onRead(m_payload);
            }
        }
	);
}

void GameSession::changeState(GameSessionState::GameSessionState* state) {
    m_state.reset(state);
}