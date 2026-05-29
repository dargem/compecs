#include <limits>
#include <new>

namespace compecs {

template <typename E, std::size_t BYTE_ALIGN = std::hardware_destructive_interference_size>
class AlignedAllocator {
    static_assert(alignof(E) <= BYTE_ALIGN, "Aligned to a value smaller than element");

   public:
    using value_type = E;
    static constexpr std::align_val_t ALIGNMENT{BYTE_ALIGN};
};

}  // namespace compecs