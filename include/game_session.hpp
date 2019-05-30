#pragma once

#include <cstdlib>
#include <memory>
#include <vector>

#include <boost/asio.hpp>

#include "core/account.hpp"
#include "game_session_state/game_session_state.hpp"

class AbstractGameServer;
class Player;

class GameSession : public std::enable_shared_from_this<GameSession>
{
public:
    GameSession(boost::asio::ip::tcp::socket, AbstractGameServer&);
    void close();
    void next();
    void start();

    AbstractGameServer& gameServer() {
        return m_gameServer;
    }

    boost::asio::ip::tcp::socket& socket() {
        return m_socket;
    }

    std::shared_ptr<Dummy::Core::Account> account() const {
        return m_account;
    }

    std::shared_ptr<::Player> player() const {
        return m_player;
    }

    void changeState(std::shared_ptr<GameSessionState::GameSessionState>);
    void setAccount(std::shared_ptr<Dummy::Core::Account>);
    void setPlayer(std::shared_ptr<::Player>);

private:
    boost::asio::ip::tcp::socket m_socket;
    AbstractGameServer& m_gameServer;
    void _doReadHeader();
    void _doReadContent();

    std::uint16_t m_header;
    std::vector<uint8_t> m_payload;
    std::shared_ptr<GameSessionState::GameSessionState> m_state;
    std::shared_ptr<Dummy::Core::Account> m_account;
    std::shared_ptr<::Player> m_player;
};
