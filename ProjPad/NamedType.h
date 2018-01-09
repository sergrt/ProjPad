#pragma once

template<typename T, typename PhantomParameter>
class NamedType {
public:
    explicit NamedType(const T& value)
        : value_{ value } {
    }
    explicit NamedType(T&& value)
        : value_{ std::move(value) } {
    }
    T& get() {
        return value_;
    }
    const T& get() const {
        return value_;
    }
private:
    T value_;
};