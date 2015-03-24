#ifndef COMPARATOR_H
#define COMPARATOR_H

#include <QObject>

template <typename T>
struct BaseComparator
{
    virtual bool compare(const T& arg1, const T& arg2) const = 0;
};

template <typename T>
struct LogicComparator : public BaseComparator<T>
{
    virtual bool compare(const T& arg1, const T& arg2) const
    {
        Q_UNUSED(arg1)
        Q_UNUSED(arg2)
        return false;
    }
};

template <typename T>
struct AndComparator : public LogicComparator<T>
{
    virtual bool compare(const T& arg1, const T& arg2) const
    {
        return arg1 && arg2;
    }
};

template <typename T>
struct OrComparator : public LogicComparator<T>
{
    virtual bool compare(const T& arg1, const T& arg2) const
    {
        return arg1 || arg2;
    }
};

template <typename T>
struct LargerThan : public BaseComparator<T>
{
    bool compare(const T& arg1, const T& arg2) const
    {
        return arg1 >= arg2;
    }
};

template <typename T>
struct Less : public BaseComparator<T>
{
    bool compare(const T& arg1, const T& arg2) const
    {
        return arg1 < arg2;
    }
};

template <typename T>
struct LessEqual : public BaseComparator<T>
{
    bool compare(const T& arg1, const T& arg2) const
    {
        return arg1 <= arg2;
    }
};

template <typename T>
struct Larger : public BaseComparator<T>
{
    bool compare(const T& arg1, const T& arg2) const
    {
        return arg1 > arg2;
    }
};

template <typename T>
struct LargerEqual : public BaseComparator<T>
{
    bool compare(const T& arg1, const T& arg2) const
    {
        return arg1 >= arg2;
    }
};

template <typename T>
struct Equal : public BaseComparator<T>
{
    bool compare(const T& arg1, const T& arg2) const
    {
        return arg1 == arg2;
    }
};

template <typename T>
struct NotEqual : public BaseComparator<T>
{
    bool compare(const T& arg1, const T& arg2) const
    {
        return arg1 != arg2;
    }
};

#endif // COMPARATOR_H
