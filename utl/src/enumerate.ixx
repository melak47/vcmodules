export module utl.enumerate;

import <iterator>;

namespace utl {

    template<typename Range>
    struct enumerated {
    private:
        static auto get_begin(Range& range) {
            using std::begin;
            return begin(range);
        }

        static auto get_end(Range& range) {
            using std::end;
            return end(range);
        }

        Range range_;
        using begin_t = decltype(get_begin(range_));
        using end_t = decltype(get_end(range_));
        begin_t begin_;
        end_t   end_;
        std::size_t idx = 0;

        using value_type = typename std::iterator_traits<begin_t>::value_type;
    public:
        struct sentinel {};

        struct result {
            std::size_t idx;
            value_type value;
        };

        enumerated(Range range)
            : range_{static_cast<Range&&>(range)}
            , begin_(get_begin(range_))
            , end_(get_end(range_))
        {}

        auto operator++() -> enumerated& {
            ++idx;
            ++begin_;
            return *this;
        }

        auto begin() -> enumerated& {
            return *this;
        }

        auto end() -> sentinel {
            return {};
        }

        auto operator==(const sentinel&) -> bool {
            return begin_ == end_;
        }

        auto operator!=(const sentinel&) -> bool {
            return begin_ != end_;
        }

        auto operator*() -> result {
            return {idx, *begin_};
        }
    };

    export
    template<typename Range>
    auto enumerate(Range&& range) {
        return enumerated<Range>(range);
    }
}

