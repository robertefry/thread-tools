
#ifndef __HH__WAIT_VARIABLE_
#define __HH__WAIT_VARIABLE_

#include <mutex>
#include <memory>

namespace thd
{

    class wait_variable
    {
        struct _i_impl
        {
            virtual ~_i_impl() = default;
            virtual void wait() = 0;
        };
        template <typename Var>
        class _impl : public _i_impl
        {
        public:
            virtual ~_impl() = default;

            explicit _impl(Var& var, std::mutex& mutex)
                : m_Var{var}
                , m_Mutex{mutex}
            {
            }

        public:
            virtual void wait() override
            {
                std::unique_lock lock {m_Mutex};
                m_Var.wait(lock);
            }

        private:
            Var& m_Var;
            std::mutex& m_Mutex;
        };

    public:
        virtual ~wait_variable() = default;

        template <typename Var>
        explicit wait_variable(Var& var, std::mutex& mutex)
            : m_Impl{std::make_unique<_impl<Var>>(var,mutex)}
        {
        }

    public:
        void wait()
        {
            m_Impl->wait();
        }

    private:
        std::unique_ptr<_i_impl> m_Impl;
    };

} // namespace thd

#endif /* __HH__WAIT_VARIABLE_ */
