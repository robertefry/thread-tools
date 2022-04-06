
#ifndef __HH__WAIT_LOCK_
#define __HH__WAIT_LOCK_

#include <memory>
#include <mutex>
#include <atomic>
#include <condition_variable>

namespace thd
{

    class gate
    {
    public:
        enum state
        {
            opened, closed,
        };

    private:
        struct data
        {
            std::mutex m_Mutex{};
            std::condition_variable m_CondOpen{};
            std::condition_variable m_CondClose{};
            std::atomic<state> m_State = state::closed;
        };

    public:
        virtual ~gate() = default;
        explicit gate(state state = state::closed);

        gate& operator=(state state);

    public:
        void open();
        void close();

        bool is_open() const;

    public:
        class async
        {
        public:
            virtual ~async() = default;
            explicit async(std::shared_ptr<data> const& data_ptr);

        public:
            bool is_alive() const;
            bool is_open() const;

            template <state s>
            void wait();
            template <state s, typename TimeSpan>
            bool wait_for(TimeSpan const&);
            template <state s, typename TimePoint>
            bool wait_until(TimePoint const&);

        private:
            std::weak_ptr<data> m_DataPtr;
        };
        async access() const;

    private:
        std::shared_ptr<data> m_DataPtr = std::make_shared<data>();
    };

    template <gate::state s>
    void gate::async::wait()
    {
        std::unique_lock lock {m_DataPtr.lock()->m_Mutex};

        if constexpr (s == state::opened) {
            m_DataPtr.lock()->m_CondOpen.wait(lock);
        } else {
            m_DataPtr.lock()->m_CondClose.wait(lock);
        }
    }

    template <gate::state s, typename TimeSpan>
    bool gate::async::wait_for(TimeSpan const& time_span)
    {
        std::unique_lock lock {m_DataPtr.lock()->m_Mutex};

        if constexpr (s == state::opened) {
            return std::cv_status::no_timeout ==
                m_DataPtr.lock()->m_CondOpen.wait_for(lock,time_span);
        } else {
            return std::cv_status::no_timeout ==
                m_DataPtr.lock()->m_CondClose.wait_for(lock,time_span);
        }
    }

    template <gate::state s, typename TimePoint>
    bool gate::async::wait_until(TimePoint const& time_point)
    {
        std::unique_lock lock {m_DataPtr.lock()->m_Mutex};

        if constexpr (s == state::opened) {
            return std::cv_status::no_timeout ==
                m_DataPtr.lock()->m_CondOpen.wait_until(lock,time_point);
        } else {
            return std::cv_status::no_timeout ==
                m_DataPtr.lock()->m_CondClose.wait_until(lock,time_point);
        }
    }

} // namespace thd

#endif /* __HH__WAIT_LOCK_ */
