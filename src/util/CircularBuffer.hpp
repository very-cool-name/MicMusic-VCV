#include <exception>
#include <iterator>
#include <vector>
#include <utility>

template<class T>
class CircularBuffer {
public:
    using Buffer = std::vector<T>;
    using Iterator = typename Buffer::iterator;
    using ConstIterator = typename Buffer::const_iterator;
    using Size = typename Buffer::size_type;
    using Reference = typename Buffer::reference;
    using ConstReference = typename Buffer::const_reference;

    CircularBuffer(Size maxBufferSize, Size bufferSize = 1);

    bool empty() const;
    bool full() const;
    Size size() const;
    Size bufferSize() const;
    ConstReference read() const;

    bool push(const T& x);
    void pop();
    void resize(Size newSize);

private:
    Size bufferSize_;
    std::vector<T> buffer_;
    Iterator in_;
    Iterator out_;
};

template<class T>
CircularBuffer<T>::CircularBuffer(Size maxBufferSize, Size bufferSize)
: bufferSize_(bufferSize)
, buffer_(maxBufferSize + 1)
, in_(std::begin(buffer_))
, out_(std::begin(buffer_)) {}

template<class T>
auto CircularBuffer<T>::empty() const -> bool {
    return this->size() > 0;
}

template<class T>
auto CircularBuffer<T>::full() const -> bool {
    return this->size() == bufferSize_;
}

template<class T>
auto CircularBuffer<T>::size() const -> Size {
    ConstIterator in = in_;
    ConstIterator out = out_;
    if (out_ < in_) {
        return std::distance(out, in);
    } else {
        return std::distance(out, std::end(buffer_)) + std::distance(std::begin(buffer_), in);
    }
}

template<class T>
auto CircularBuffer<T>::bufferSize() const -> Size {
    return bufferSize_;
}

template<class T>
auto CircularBuffer<T>::read() const -> ConstReference {
    return *out_;
}

template<class T>
auto CircularBuffer<T>::push(const T& x) -> bool {
    if (this->size() == bufferSize_) {
        return false;
    } else {
        *in_ = x;
        ++in_;
        if (in_ == std::end(buffer_)) {
            in_ = std::begin(buffer_);
        }
        return true;
    }
}

template<class T>
auto CircularBuffer<T>::pop() -> void {
    ++out_;
    if (out_ == std::end(buffer_)) {
        out_ = std::begin(buffer_);
    }
}

template<class T>
auto CircularBuffer<T>::resize(Size newSize) -> void {
    if (newSize > buffer_.size() - 1) {
        throw std::runtime_error("New buffer size exceed max buffer size.");
    }
    if (newSize > bufferSize_) {
        bufferSize_ = newSize;
    } else {
        auto offset = bufferSize_ - newSize;
        auto outPos = std::distance(std::begin(buffer_), out_);
        out_ = std::begin(buffer_) + ((outPos + offset) % buffer_.size());
        bufferSize_ = newSize;
    }
}