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
private:
    ::AbstractGameServer& m_abstractGameServer;
    const Dummy::Core::Map& m_map;
    std::set<std::shared_ptr<::Player>> m_players;

};
