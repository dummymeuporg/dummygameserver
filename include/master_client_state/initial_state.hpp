#pragma once

namespace MasterClientState
{

class InitialState : public MasterClientState {
public:
    InitialState(::MasterClient&);
    virtual void onRead(const std::vector<std::uint8_t>&) override;

};

}
