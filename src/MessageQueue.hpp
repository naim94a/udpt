#pragma once

#include <queue>
#include <mutex>

namespace UDPT
{
    namespace Utils {

        template<class T>
        class MessageQueue {
        public:
            MessageQueue() {}

            virtual ~MessageQueue() {}

            bool IsEmpty() const {
                return m_queue.empty();
            }

            T Pop() {
                m_queueMutex.lock();
                T val = m_queue.front();
                m_queue.pop();
                m_queueMutex.unlock();

                return val;
            }

            void Push(T obj) {
                m_queueMutex.lock();
                m_queue.push(obj);
                m_queueMutex.unlock();
            }

            size_t Count() const {
                return m_queue.size();
            }

        private:
            std::queue<T> m_queue;
            std::mutex m_queueMutex;
        };

    }
}
