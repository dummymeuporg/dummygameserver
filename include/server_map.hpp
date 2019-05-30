#pragma once

#include <memory>
#include <set>

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
    ServerMap(
        ::AbstractGameServer&,
        const Dummy::Core::Map&);
    void addPlayer(std::shared_ptr<::Player>);
    void removePlayer(std::shared_ptr<::Player>);
    bool isBlocking(std::uint16_t, std::uint16_t) const;
    std::set<std::shared_ptr<::Player>> players() const {
        return m_players;
    }
private:
    ::AbstractGameServer& m_abstractGameServer;
    const Dummy::Core::Map& m_map;
    std::set<std::shared_ptr<::Player>> m_players;

};
