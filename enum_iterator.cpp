#include <iostream>
#include <iterator>

template<typename E, E first, E last>
struct EnumRange
{
    struct Iterator
    : public std::iterator<
          std::random_access_iterator_tag,
          E,
          ptrdiff_t,
          const E *,
          const E &
      >
    {
        Iterator()
        : p_(static_cast<ptrdiff_t>(last) + 1)
        {
        }

        Iterator(E value)
        : p_(static_cast<ptrdiff_t>(value))
        {
        }

        Iterator(const Iterator &) = default;

        ~Iterator() = default;

        Iterator &
        operator+=(ptrdiff_t n)
        {
            p_ += n;
            return *this;
        }

        Iterator &
        operator++()
        {
            return *this += 1;
        }

        Iterator
        operator++(int)
        {
            Iterator other(*this);
            ++(*this);
            return other;
        }

        Iterator &
        operator-=(ptrdiff_t n)
        {
            p_ -= n;
            return *this;
        }

        Iterator &
        operator--()
        {
            return *this -= 1;
        }

        Iterator
        operator--(int)
        {
            Iterator other(*this);
            --(*this);
            return other;
        }

        E
        operator*() const
        {
            return static_cast<E>(p_);
        }

        bool
        operator==(const Iterator &other) const
        {
            return p_ == other.p_;
        }

        bool
        operator!=(const Iterator &other) const
        {
            return p_ != other.p_;
        }

        bool
        operator<(const Iterator &other) const
        {
            return p_ < other.p_;
        }

        bool
        operator>(const Iterator &other) const
        {
            return p_ > other.p_;
        }

        bool
        operator<=(const Iterator &other) const
        {
            return p_ <= other.p_;
        }

        bool
        operator>=(const Iterator &other) const
        {
            return p_ >= other.p_;
        }

        E
        operator[](ptrdiff_t i) const
        {
            return static_cast<E>(p_ + i);
        }

        friend Iterator
        operator+(const Iterator &it, ptrdiff_t n)
        {
            return Iterator(it.p_ + n);
        }

        friend Iterator
        operator+(ptrdiff_t n, const Iterator &it)
        {
            return Iterator(it.p_ + n);
        }

        friend Iterator
        operator-(const Iterator &it, ptrdiff_t n)
        {
            return Iterator(it.p_ - n);
        }

        friend ptrdiff_t
        operator-(const Iterator &l, const Iterator &r)
        {
            return l.p_ - r.p_;
        }

    private:

        Iterator(ptrdiff_t p) : p_(p) {};

        ptrdiff_t p_;
    };

    static Iterator
    begin()
    {
        return Iterator(first);
    };

    static Iterator
    end()
    {
        return Iterator();
    };
};

// Example

enum class Rm
{
#define DO_X_FOR_FOOS(X) X(I) X(II) X(III) X(IV) X(V) X(VI) X(VII)

#define DEFINE_ENUM(FOO) FOO,

    DO_X_FOR_FOOS(DEFINE_ENUM)

#undef DEFINE_ENUM
};

std::ostream &
operator<<(std::ostream &os, const Rm &foo)
{
    switch(foo)
    {

#define OUTPUT_TO_STREAM(FOO)   \
        case Rm::FOO:          \
            os << "Rm::"#FOO ; \
            break;

    DO_X_FOR_FOOS(OUTPUT_TO_STREAM)

#undef OUTPUT_TO_STREAM

        default:
            os << "something else (ptrdiff_t = "
               << static_cast<ptrdiff_t>(foo) << ")";
            break;
    }
    return os;
}

typedef EnumRange<Rm, Rm::I, Rm::VII> RmRange;

int
main()
{
    {
        std::cout << "Plain range based iteration:" << std::endl;
        for (const auto &x : RmRange())
        {
            std::cout << x << std::endl;
        }
        std::cout << std::endl;
    }
    {
        std::cout << "Some random operations on iterator:" << std::endl;
        auto it = RmRange::begin();
        std::cout << "*it           = " << *it           << std::endl
                  << "*(--it)       = " << *(--it)       << std::endl
                  << "*(it += 3)    = " << *(it += 3)    << std::endl
                  << "*it           = " << *it           << std::endl
                  << "*(it + 2)     = " << *(it + 2)     << std::endl
                  << "it[3]         = " << it[3]         << std::endl
                  << "(it + 2) - it = " << (it + 2) - it << std::endl
                  << "std::distance(it, it + 2) = "
                  << std::distance(it, it + 2) << std::endl
                  << std::endl;
    }
}
