
#ifndef __HH_THD__TASK_QUEUE_
#define __HH_THD__TASK_QUEUE_

#include <thread-tools/transfer_queue.hh>
#include <thread-tools/wait_lock.hh>

#include <functional>
#include <future>
#include <tuple>
#include <memory>

namespace thd
{

    class task_queue
    {
        struct i_executor
        {
            virtual ~i_executor() = default;
            virtual void operator()() = 0;
        };

        template <typename t_func>
        class executor;

        template <typename t_return, typename... t_args>
        class executor<t_return(t_args...)> : public i_executor
        {
        public:
            template <typename t_func>
            explicit executor(t_func&& func, t_args&&... args)
                : m_PackagedTask{std::forward<t_func>(func)}
                , m_PackagedArgs{std::forward<t_args>(args)...}
            {}
            std::future<t_return> future()
            {
                return m_PackagedTask.get_future();
            }
            virtual void operator()() override
            {
                std::apply(m_PackagedTask,m_PackagedArgs);
            }
        private:
            std::packaged_task<t_return(t_args...)> m_PackagedTask;
            std::tuple<t_args...> m_PackagedArgs;
        };

    public:
        virtual ~task_queue() = default;
        explicit task_queue() = default;

        template <typename t_func, typename... t_args>
        auto push_task(t_func&& func, t_args&&... args)
        {
            using t_return = typename std::invoke_result<t_func,t_args...>::type;
            using t_executor = executor<t_return(t_args...)>;

            auto executor = std::make_unique<t_executor>(
                std::forward<t_func>(func), std::forward<t_args>(args)...
            );
            auto future = executor->future();

            m_TaskQueue.push(std::move(executor));
            m_Notifier.notify_one();
            return future;
        }

        template <typename t_func, typename... t_args>
        auto wait_task(t_func&& func, t_args&&... args)
        {
            auto future = push_task(
                std::forward<t_func>(func), std::forward<t_args>(args)...
            );
            future.wait();
            return future.get();
        }

        auto notifier() const
        {
            return m_Notifier;
        }

        auto empty() const
        {
            return m_TaskQueue.empty();
        }

        auto size() const
        {
            return m_TaskQueue.size();
        }

        void execute_one()
        {
            auto front = std::move(m_TaskQueue.pop());
            front->operator()();
        }

        void execute_all()
        {
            while (not empty())
            {
                execute_one();
            }
        }

    private:
        thd::transfer_queue<std::unique_ptr<i_executor>> m_TaskQueue{};
        thd::wait_lock m_Notifier{};
    };

} // namespace thd

#endif /* __HH_THD__TASK_QUEUE_ */
