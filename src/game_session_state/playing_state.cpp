#include "player.hpp"
#include "game_session.hpp"
#include "game_session_state/playing_state.hpp"
#include "server_map.hpp"

#include "protocol/map_update.hpp"
#include "protocol/character_position.hpp"
#include "protocol/character_off.hpp"

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
    case 2:
        /* Ping. */
        _onPing(pkt);
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
    std::cerr << "[" << player->name() << "] Move to "
        << x << "," << y << std::endl; 
    
    if (serverMap->isBlocking(x, y)) {
        std::cerr << "[!] Wrong position!" << std::endl;
    } else {
        player->setPosition(x, y);
    }
    Dummy::Protocol::OutgoingPacket outPkt;
    outPkt << answer;
    _updateLivings(player, serverMap, outPkt);
    _answer(outPkt);
}

void PlayingState::_onPing(Dummy::Protocol::IncomingPacket& pkt) {
    // XXX: We will handle the ping later. Send characters updates.
    std::uint8_t answer = 1;
    std::shared_ptr<::Player> player(m_gameSession->player());
    std::shared_ptr<::ServerMap> serverMap(player->serverMap());

    std::cerr << player->name() << " is at "
        << player->serverPosition().first << ", "
        << player->serverPosition().second << std::endl;

    Dummy::Protocol::OutgoingPacket outPkt;
    outPkt << answer;
    _updateLivings(player, serverMap, outPkt);
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

void PlayingState::_updateLivings(std::shared_ptr<::Player> player,
                                  std::shared_ptr<::ServerMap> map,
                                  Dummy::Protocol::OutgoingPacket& pkt) {

    std::vector<std::shared_ptr<Dummy::Protocol::MapUpdate>> mapUpdates;

    // Get the players around.
    std::cerr << "There are " << map->players().size() << " on the map."
        << std::endl;
    for (const auto otherPlayer: map->players()) {
        if (player->name() == otherPlayer.first) {
            continue;
        }

        std::cerr << "Player " << otherPlayer.first << std::endl;

        std::shared_ptr<const Dummy::Core::Character>
            chr(otherPlayer.second->character());
        if (m_livings.find(chr->name()) == std::end(m_livings)) {
            // A new character appeared.
            m_livings[otherPlayer.first] = 
                std::make_unique<Dummy::Protocol::Living>(
                    otherPlayer.second->serverPosition().first,
                    otherPlayer.second->serverPosition().second,
                    otherPlayer.second->character()->name(),
                    otherPlayer.second->character()->skin(),
                    otherPlayer.second->character()->direction()
                );
            std::cerr << "Hello " << otherPlayer.first << std::endl;
        } else {
            // Update the living status.
            Dummy::Protocol::Living& living(*m_livings[otherPlayer.first]);
            
            if (living.x() != otherPlayer.second->serverPosition().first ||
                living.y() != otherPlayer.second->serverPosition().second)
            {
                living.setPosition(
                    otherPlayer.second->serverPosition().first,
                    otherPlayer.second->serverPosition().second
                );
                std::cerr << "UPDATE POS TO: " << living.x() << ", "
                    << living.y() << std::endl;
            }

            if (living.chipset() !=
                otherPlayer.second->character()->skin())
            {
                living.setChipset(
                    otherPlayer.second->character()->skin()
                );
            }

            if (living.direction() !=
                otherPlayer.second->character()->direction())
            {
                living.setDirection(
                    otherPlayer.second->character()->direction()
                );
            }
            std::cerr << "Updated " << otherPlayer.first << std::endl;
        }

        // Create the event.
        Dummy::Protocol::Living& living(*m_livings[otherPlayer.first]);
        std::shared_ptr<Dummy::Protocol::CharacterPosition> event =
            std::make_shared<Dummy::Protocol::CharacterPosition>(
                living.x(),
                living.y(),
                living.name(),
                living.chipset(),
                living.direction()
            );
        mapUpdates.push_back(event);

    }

    std::vector<std::string> departures;
    // Check if any player left the map
    for(const auto& [name, living]: m_livings) {
        //const Dummy::Protocol::Living& living(*it.second);
        if (map->players().find(name) == std::end(map->players())) {
            std::shared_ptr<Dummy::Protocol::CharacterOff> event =
                std::make_shared<Dummy::Protocol::CharacterOff>(name);
            mapUpdates.push_back(event);
            departures.push_back(name);
        }
    }

    for (const auto& name: departures) {
        std::cerr << "Bye bye " << name << std::endl;
        m_livings.erase(name);
    }

    // Write map updates to outgoing packet.
    std::uint16_t count = mapUpdates.size();
    pkt << count;
    for (const auto& update: mapUpdates) {
        pkt << update->code() << *update;
    }
}

} // namespace GameSessionState
