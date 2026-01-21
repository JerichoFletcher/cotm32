#pragma once

#include <fmt/core.h>

#include <vector>

template <typename T>
class RingBuffer {
public:
    RingBuffer(size_t cap);
    bool push_back(T&& item);
    bool pop_front(T* item);
    void clear();

    inline size_t size() const { return this->m_size; }
    inline bool empty() const { return this->m_size == 0; }
    inline bool full() const { return this->m_size == this->m_cap; }

    T& operator[](size_t index);
    const T& operator[](size_t index) const;

private:
    std::vector<T> m_buf;
    size_t m_head;
    size_t m_tail;
    size_t m_size;
    size_t m_cap;
};

#include <stdexcept>

template <typename T>
RingBuffer<T>::RingBuffer(size_t cap) : m_buf(cap),
                                        m_head(0),
                                        m_tail(0),
                                        m_size(0),
                                        m_cap(cap) {}

template <typename T>
bool RingBuffer<T>::push_back(T&& item) {
    if (this->m_size == this->m_cap) {
        return false;
    }

    this->m_buf[this->m_head] = std::move(item);
    this->m_head = (this->m_head + 1) % this->m_cap;
    this->m_size++;

    return true;
}

template <typename T>
bool RingBuffer<T>::pop_front(T* item) {
    if (this->m_size == 0) {
        return false;
    }

    if (item != nullptr) {
        *item = this->m_buf[this->m_tail];
    }
    this->m_tail = (this->m_tail + 1) % this->m_cap;
    this->m_size--;

    return true;
}

template <typename T>
void RingBuffer<T>::clear() {
    this->m_head = 0;
    this->m_tail = 0;
    this->m_size = 0;
}

template <typename T>
T& RingBuffer<T>::operator[](size_t index) {
    if (index >= this->m_size) {
        throw std::range_error(fmt::format("Index out of range {} vs. {}", index, this->m_size));
    }
    return this->m_buf[(this->m_tail + index) % this->m_cap];
}

template <typename T>
const T& RingBuffer<T>::operator[](size_t index) const {
    if (index >= this->m_size) {
        throw std::range_error(fmt::format("Index out of range {} vs. {}", index, this->m_size));
    }
    return this->m_buf[(this->m_tail + index) % this->m_cap];
}
