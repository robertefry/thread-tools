
#ifndef __HH_THD__TRANSFER_QUEUE_
#define __HH_THD__TRANSFER_QUEUE_

#include <cstddef>
#include <utility>
#include <mutex>

/* ************************************************************************** */
//  [definition] thd::transfer_queue
/* ************************************************************************** */

namespace thd
{

    template <typename T>
    class transfer_queue
    {
        struct node
        {
            T m_Data;
            node* m_Next;

            explicit node(T const&);
            explicit node(T&&);

            template <typename... Args>
            explicit node(Args&&...);

            node(node const&) = delete;
            node& operator=(node const&) = delete;
        };
        void _destroy() noexcept;

    public:
        virtual ~transfer_queue();
        transfer_queue();

        transfer_queue(transfer_queue const&);
        transfer_queue& operator=(transfer_queue const&);

        transfer_queue(transfer_queue&&);
        transfer_queue& operator=(transfer_queue&&);

        T& front();
        T const& front() const;

        T& back();
        T const& back() const;

        bool empty() const;
        size_t size() const;

        void push(T const&);
        void push(T&&);

        template <typename... Args>
        void emplace(Args&&...);

        T pop();

    private:
        node* m_Front;
        node* m_rBack;

        mutable std::mutex m_Mutex{};
    };

} // namespace thd

/* ************************************************************************** */
//  [implementation] thd::transfer_queue
/* ************************************************************************** */

namespace thd
{

// thd::transfer_queue::node

    template <typename T>
    transfer_queue<T>::node::node(T const& t)
        : m_Data{t}
        , m_Next{nullptr}
    {
    }

    template <typename T>
    transfer_queue<T>::node::node(T&& t)
        : m_Data{std::move(t)}
        , m_Next{nullptr}
    {
    }

    template <typename T>
    template <typename... Args>
    transfer_queue<T>::node::node(Args&&... args)
        : m_Data{std::forward<Args>(args)...}
        , m_Next{nullptr}
    {
    }

// thd::transfer_queue

    template <typename T>
    void transfer_queue<T>::_destroy() noexcept
    {
        while (m_Front != nullptr)
        {
            node* front = m_Front;
            m_Front = m_Front->m_Next;
            delete front;
        }
        m_Front = nullptr;
        m_rBack = nullptr;
    }

    template <typename T>
    transfer_queue<T>::~transfer_queue()
    {
        _destroy();
    }

    template <typename T>
    transfer_queue<T>::transfer_queue()
        : m_Front{nullptr}
        , m_rBack{nullptr}
    {
    }

    template <typename T>
    transfer_queue<T>::transfer_queue(transfer_queue const& other)
        : m_Front{nullptr}
        , m_rBack{nullptr}
    {
        *this = other;
    }

    template <typename T>
    transfer_queue<T>& transfer_queue<T>::operator=(transfer_queue const& other)
    {
        std::scoped_lock lock {m_Mutex,other.m_Mutex};
        _destroy();

        if (other.m_Front)
        {
            m_Front = new node{other.m_Front->m_Data};
            m_rBack = m_Front;
        }
        for (node* front = other.m_Front->m_Next; front != nullptr; front = front->m_Next)
        {
            m_rBack->m_Next = new node{front->m_Data};
            m_rBack = m_rBack->m_Next;
        }
        return *this;
    }

    template <typename T>
    transfer_queue<T>::transfer_queue(transfer_queue&& other)
        : m_Front{nullptr}
        , m_rBack{nullptr}
    {
        *this = std::move(other);
    }

    template <typename T>
    transfer_queue<T>& transfer_queue<T>::operator=(transfer_queue<T>&& other)
    {
        std::scoped_lock lock {m_Mutex,other.m_Mutex};

        std::swap(m_Front,other.m_Front);
        std::swap(m_rBack,other.m_rBack);
        return *this;
    }

    template <typename T>
    T& transfer_queue<T>::front()
    {
        std::scoped_lock lock {m_Mutex};
        return m_Front->m_Data;
    }

    template <typename T>
    T const& transfer_queue<T>::front() const
    {
        std::scoped_lock lock {m_Mutex};
        return m_Front->m_Data;
    }

    template <typename T>
    T& transfer_queue<T>::back()
    {
        std::scoped_lock lock {m_Mutex};
        return m_rBack->m_Data;
    }

    template <typename T>
    T const& transfer_queue<T>::back() const
    {
        std::scoped_lock lock {m_Mutex};
        return m_rBack->m_Data;
    }

    template <typename T>
    bool transfer_queue<T>::empty() const
    {
        std::scoped_lock lock {m_Mutex};
        return m_Front == nullptr;
    }

    template <typename T>
    size_t transfer_queue<T>::size() const
    {
        std::scoped_lock lock {m_Mutex};

        size_t size = 0;

        for (node* n = m_Front; n != nullptr; n = n->m_Next)
        {
            size += 1;
        }
        return size;
    }

    template <typename T>
    void transfer_queue<T>::push(T const& t)
    {
        std::scoped_lock lock {m_Mutex};

        if (m_Front == nullptr)
        {
            m_Front = new node{t};
            m_rBack = m_Front;
        }
        else
        {
            m_rBack->m_Next = new node{t};
            m_rBack = m_rBack->m_Next;
        }
    }

    template <typename T>
    void transfer_queue<T>::push(T&& t)
    {
        std::scoped_lock lock {m_Mutex};

        if (m_Front == nullptr)
        {
            m_Front = new node{std::move(t)};
            m_rBack = m_Front;
        }
        else
        {
            m_rBack->m_Next = new node{std::move(t)};
            m_rBack = m_rBack->m_Next;
        }
    }

    template <typename T>
    template <typename... Args>
    void transfer_queue<T>::emplace(Args&&... args)
    {
        std::scoped_lock lock {m_Mutex};

        if (m_Front == nullptr)
        {
            m_Front = new node{std::forward<Args>(args)...};
            m_rBack = m_Front;
        }
        else
        {
            m_rBack->m_Next = new node{std::forward<Args>(args)...};
            m_rBack = m_rBack->m_Next;
        }
    }

    template <typename T>
    T transfer_queue<T>::pop()
    {
        std::scoped_lock lock {m_Mutex};

        T data = std::move(m_Front->m_Data);
        {
            node* front = m_Front;
            m_Front = m_Front->m_Next;
            delete front;
        }
        return data;
    }

} // namespace thd

#endif /* __HH_THD__TRANSFER_QUEUE_ */
