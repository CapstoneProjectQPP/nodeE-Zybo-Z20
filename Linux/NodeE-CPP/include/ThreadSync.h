#pragma once

#include <condition_variable>
#include <utility>


namespace Xceed {

    namespace ThreadSync {
        template<class E>
        class ConcurrentBlockingDeque {
        public:
            ConcurrentBlockingDeque<E>() = default;

            E block_poll() {
                std::unique_lock<std::mutex> lock{m};
                while (d.empty()) {
                    not_empty_trigger.wait(lock, [&] {
                       return !d.empty();
                    });
                }

                E result = (E)d.front();
                d.pop_front();
                return result;
            }

            void push_back(E& item) {
                std::unique_lock<std::mutex> lock{m};
                d.push_back(item);
                not_empty_trigger.notify_one();
            }

        private:
            std::deque<E> d;
            std::mutex m;
            std::condition_variable not_empty_trigger;
        };

        template<class T>
        static T* safeAcquire(std::deque<T>* unsafe_buffer, std::mutex& m, std::condition_variable& trigger) {
            std::unique_lock<std::mutex> lock{m};
            while (unsafe_buffer->empty()) {
                trigger.wait(lock, [&] {
                    return !unsafe_buffer->empty();
                });
            }
            T* acquired = new T(unsafe_buffer->front());
            unsafe_buffer->pop_front();

            return acquired;
        }
    };

}
