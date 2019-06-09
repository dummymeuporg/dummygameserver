#include <iostream>
#include "protocol/bridge.hpp"
#include "protocol/incoming_packet.hpp"
#include "protocol/outgoing_packet.hpp"
#include "server/command/create_character.hpp"
#include "server/command/select_character.hpp"
#include "server/response/create_character.hpp"
#include "server/response/select_character.hpp"
#include "network_session.hpp"
#include "network_session_state/manage_characters_state.hpp"
#include "network_session_state/loading_state.hpp"

namespace NetworkSessionState {

ManageCharactersState::ManageCharactersState(::NetworkSession& networkSession)
    : State(networkSession)
{

}

std::unique_ptr<const Dummy::Server::Command::Command>
ManageCharactersState::getCommand(Dummy::Protocol::IncomingPacket& pkt)
{
    std::uint16_t command;
    pkt >> command;
    switch(command) {
    case Dummy::Protocol::Bridge::CREATE_CHARACTER:
        /* Build 'create character' command. */
        return _createCharacter(pkt);
        break;
    case Dummy::Protocol::Bridge::SELECT_CHARACTER:
        /* Build 'select character' command. */
        return _selectCharacter(pkt);
        break;
    default:
        /* throw an exception */
        throw UnknownCommandError();
        break;
    }
}

std::unique_ptr<const Dummy::Server::Command::CreateCharacter>
ManageCharactersState::_createCharacter(Dummy::Protocol::IncomingPacket& pkt)
{
    std::string name, skin;
    pkt >> name >> skin;
    return std::make_unique<Dummy::Server::Command::CreateCharacter>(
        name, skin       
    );
}

std::unique_ptr<const Dummy::Server::Command::SelectCharacter>
ManageCharactersState::_selectCharacter(Dummy::Protocol::IncomingPacket& pkt)
{
    std::string name;
    pkt >> name;
    return std::make_unique<Dummy::Server::Command::SelectCharacter>(
        name
    );
}


void ManageCharactersState::visit(
    const Dummy::Server::Response::Response& response
) {
    response.accept(*this);
}

void ManageCharactersState::visitResponse(
    const Dummy::Server::Response::CreateCharacter& response
) {
    auto self(shared_from_this());
    std::cerr << "Create Character" << std::endl;
}

void ManageCharactersState::visitResponse(
    const Dummy::Server::Response::SelectCharacter& response
) {
    auto self(shared_from_this());
    if (response.status() == 0) {
        std::cerr << "Select Character O.K." << std::endl;
        m_networkSession.changeState(
            std::make_shared<LoadingState>(m_networkSession)
        );
    } else {
        std::cerr << "Error while selecting character" << std::endl;
    }
}


} // namespace NetworkSessionState
