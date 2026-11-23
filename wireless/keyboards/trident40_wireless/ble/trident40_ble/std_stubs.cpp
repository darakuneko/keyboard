// The core links without the libstdc++ exception support that std::vector refers to. Running out
// of memory here is a firmware bug: reset the chip rather than run on with a broken container.
#include <Arduino.h>

namespace std {
void __throw_bad_alloc() { NVIC_SystemReset(); while (1) {} }
void __throw_length_error(const char *) { NVIC_SystemReset(); while (1) {} }
}  // namespace std
