
#ifndef __HH_THD__WAIT_LOCK_
#define __HH_THD__WAIT_LOCK_

#include <memory>
#include <mutex>
#include <condition_variable>

/* ************************************************************************** */
//  [definition] thd::wait_lock
/* ************************************************************************** */

namespace thd
{

    class wait_lock
    {
        struct data
        {
            std::mutex m_Mutex{};
            std::condition_variable m_CV{};
        };

    public:
        virtual ~wait_lock() = default;
        explicit wait_lock() = default;

    public:
        class async
        {
            friend wait_lock;

            explicit async(std::weak_ptr<data> const&);

        public:
            virtual ~async() = default;

            bool alive() const;

            void wait();
            template <typename TimeSpan>
            bool wait_for(TimeSpan const&);
            template <typename TimePoint>
            bool wait_until(TimePoint const&);

        private:
            std::weak_ptr<data> m_DataPtr;
        };
        async access() const;

        void notify_one();
        void notify_all();

        void wait();
        template <typename TimeSpan>
        bool wait_for(TimeSpan const&);
        template <typename TimePoint>
        bool wait_until(TimePoint const&);

    private:
        std::shared_ptr<data> m_DataPtr = std::make_shared<data>();
    };

} // namespace thd

/* ************************************************************************** */
//  [implementation] thd::wait_lock
/* ************************************************************************** */

namespace thd
{

// thd::wait_lock

    inline wait_lock::async wait_lock::access() const
    {
        return async{m_DataPtr};
    }

    inline void wait_lock::notify_one()
    {
        std::unique_lock lock {m_DataPtr->m_Mutex};
        m_DataPtr->m_CV.notify_one();
    }

    inline void wait_lock::notify_all()
    {
        std::unique_lock lock {m_DataPtr->m_Mutex};
        m_DataPtr->m_CV.notify_all();
    }

    inline void wait_lock::wait()
    {
        std::unique_lock lock {m_DataPtr->m_Mutex};
        m_DataPtr->m_CV.wait(lock);
    }

    template <typename TimeSpan>
    inline bool wait_lock::wait_for(TimeSpan const& time_span)
    {
        std::unique_lock lock {m_DataPtr->m_Mutex};
        auto status = m_DataPtr->m_CV.wait_for(lock,time_span);
        return status = std::cv_status::no_timeout;
    }

    template <typename TimePoint>
    inline bool wait_lock::wait_until(TimePoint const& time_point)
    {
        std::unique_lock lock {m_DataPtr->m_Mutex};
        auto status = m_DataPtr->m_CV.wait_until(lock,time_point);
        return status = std::cv_status::no_timeout;
    }

// thd::wait_lock::async

    inline thd::wait_lock::async::async(std::weak_ptr<data> const& data_ptr)
        : m_DataPtr{data_ptr}
    {
    }

    inline bool thd::wait_lock::async::alive() const
    {
        return not m_DataPtr.expired();
    }

    inline void thd::wait_lock::async::wait()
    {
        std::unique_lock lock {m_DataPtr.lock()->m_Mutex};
        m_DataPtr.lock()->m_CV.wait(lock);
    }

    template <typename TimeSpan>
    inline bool thd::wait_lock::async::wait_for(TimeSpan const& time_span)
    {
        std::unique_lock lock {m_DataPtr.lock()->m_Mutex};
        auto status = m_DataPtr.lock()->m_CV.wait_for(lock,time_span);
        return status = std::cv_status::no_timeout;
    }

    template <typename TimePoint>
    inline bool thd::wait_lock::async::wait_until(TimePoint const& time_point)
    {
        std::unique_lock lock {m_DataPtr.lock()->m_Mutex};
        auto status = m_DataPtr.lock()->m_CV.wait_until(lock,time_point);
        return status = std::cv_status::no_timeout;
    }

} // namespace thd

#endif /* __HH_THD__WAIT_LOCK_ */
