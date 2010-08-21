#ifndef SINGLETON_HPP
#define SINGLETON_HPP

template <typename T>
class Singleton
{
    public:
        static T &getInstance()
        {
            static T instance;
            return instance;
        }

    protected:
        Singleton() {}
        Singleton(const Singleton &s) { }
        Singleton &operator=(const Singleton &) { return *this; }
};

#endif /* SINGLETON_HPP */
