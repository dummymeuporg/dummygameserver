#pragma once

#include "project/map.hpp"

class Player;

class ServerMap {
public:
    ServerMap(::AbstractGameServer&, const Dummy::Core::Map&);
private:
    std::set<std::shared_ptr<Player>> m_characters;

};
