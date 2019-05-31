#pragma once

#include <map>
#include <memory>

class AbstractGameServer;
class Player;

namespace Dummy {
namespace Core {
class Map;
} // namespace Core
} // namespace Dummy

class Player;

class ServerMap {
public:
    using PlayersList = std::map<std::string, std::shared_ptr<::Player>>;
    ServerMap(
        ::AbstractGameServer&,
        const Dummy::Core::Map&);
    void addPlayer(std::shared_ptr<::Player>);
    void removePlayer(std::shared_ptr<::Player>);
    bool isBlocking(std::uint16_t, std::uint16_t) const;
    const PlayersList& players() const {
        return m_players;
    }
private:
    ::AbstractGameServer& m_abstractGameServer;
    const Dummy::Core::Map& m_map;
    PlayersList m_players;

};
