#pragma once

#include <cstdint>
#include <vector>

class MasterClient;

namespace MasterClientState
{

class MasterClientState {
public:
    MasterClientState(::MasterClient&);
    virtual void onRead(const std::vector<std::uint8_t>&) = 0;
protected:
    ::MasterClient& m_masterClient;
};

} // namespace MasterClientState
