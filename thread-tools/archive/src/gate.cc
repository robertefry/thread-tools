
#include "thread-tools/gate.hh"

namespace thd
{

    gate::gate(state state)
    {
        m_DataPtr->m_State = state;
    }

    gate& gate::operator=(state state)
    {
        if (state == state::opened) {
            open();
        } else {
            close();
        }
        return *this;
    }

    void gate::open()
    {
        std::unique_lock lock {m_DataPtr->m_Mutex};
        m_DataPtr->m_State = state::opened;
        m_DataPtr->m_CondOpen.notify_all();
    }

    void gate::close()
    {
        std::unique_lock lock {m_DataPtr->m_Mutex};
        m_DataPtr->m_State = state::closed;
        m_DataPtr->m_CondClose.notify_all();
    }

    bool gate::is_open() const
    {
        std::unique_lock lock {m_DataPtr->m_Mutex};
        return m_DataPtr->m_State == state::opened;
    }

    gate::async gate::access() const
    {
        return async{m_DataPtr};
    }

    gate::async::async(std::shared_ptr<data> const& data_ptr)
        : m_DataPtr{data_ptr}
    {
    }

    bool gate::async::is_alive() const
    {
        return not m_DataPtr.expired();
    }

    bool gate::async::is_open() const
    {
        std::unique_lock lock {m_DataPtr.lock()->m_Mutex};
        return m_DataPtr.lock()->m_State == state::opened;
    }

} // namespace thd
