#pragma once

/// @brief a template base class for singleton
/// @tparam T the class as singleton, use like this: `class MySingleton : public Singleton<MySingleton>`
template <typename T>
class Singleton
{
public:
    static T *GetInstance()
    {
        static T _instance;
        return &_instance;
    }

protected:
    Singleton() {}
    ~Singleton() {}

public:
    Singleton(Singleton const &) = delete;
    Singleton &operator=(Singleton const &) = delete;
};

/// @brief a useful macro to define a singleton class, don't forget to use it with SINGLETON_END
#define SINGLETON_BEGIN(cls)          \
    class cls : public Singleton<cls> \
    {                                 \
        friend class Singleton<cls>;

#define SINGLETON_END \
    }                 \
    ;
