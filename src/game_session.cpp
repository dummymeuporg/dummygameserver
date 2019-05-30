#include <iostream>

#include <boost/asio.hpp>

#include "game_session_state/initial_state.hpp"
#include "abstract_game_server.hpp"
#include "abstract_game_server.hpp"
#include "game_session.hpp"
#include "player.hpp"

GameSession::GameSession(boost::asio::ip::tcp::socket s,
                         AbstractGameServer& gameServer)
    : m_socket(std::move(s)),
      m_gameServer(gameServer),
      m_state(nullptr),
      m_account(nullptr)      
{

}

void GameSession::close() {
    m_state = nullptr;
    if (nullptr != m_player) {
        // XXX: disconnect player from its current map.
        m_player->setServerMap(nullptr);
        saveCharacter();
    }
    m_socket.close();
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
            if (!ec) {
                m_payload.resize(m_header);
                _doReadContent();
            } else {
                std::cerr << "Error while reading size." << std::endl;
                close();
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
            if (!ec) {
                std::cerr << "Read " << length << " bytes.";
                try {
                    Dummy::Protocol::IncomingPacket pkt(m_payload);
                    m_state->onRead(pkt);
                } catch(const Dummy::Protocol::Error& e) {
                    std::cerr << e.what();
                    close();
                }
            } else {
                std::cerr << "Error while reading content." << std::endl;
                close();
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

void GameSession::saveCharacter() {
    std::cerr << "Save character." << std::endl;
    m_gameServer.saveCharacter(*m_account, *(m_player->character()));
}
