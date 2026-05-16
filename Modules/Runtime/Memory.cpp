#include <Memory.hpp>

extern "C" void* memset(
    void* destination,
    const int value,
    const size_t count
) {
    auto* bytes = static_cast<unsigned char*>(destination);
    const auto byte = static_cast<unsigned char>(value);

    for (size_t index = 0; index < count; ++index) {
        bytes[index] = byte;
    }

    return destination;
}
