#pragma once

template <typename T>
class static_singleton {
  public:
    static auto instance() -> T& {
        static T instance_;

        return instance_;
    }

  protected:
    static_singleton() = default;
};
