#pragma once

#include <concepts>


#define ENUM_COMMON(Name, Type, ...)                                  \
    public:                                                           \
        enum Enum : Type { __VA_ARGS__ };                             \
                                                                      \
        constexpr Name() = default;                                   \
        /* implicit */ constexpr Name(Enum value) : value(value) {}   \
                                                                      \
        template<std::integral T>                                     \
        explicit constexpr Name(T value)                              \
            : value(static_cast<Enum>(value)) {}                      \
                                                                      \
        /* Allows comparisons with Enum constants and switch-case */  \
        constexpr operator Enum() const { return value; }             \
                                                                      \
        explicit constexpr operator Type() const {                    \
            return static_cast<Type>(value);                          \
        }                                                             \
                                                                      \
    private:                                                          \
        Enum value;                                                   \


// Put this inside a class definition to create an enum-like type that supports
// methods. In method definitions, access the underlying value using `value`.
// Use the resulting class like a normal enum.
//
// For example:
//
// class Color {
//      DEFINE_ENUM(Color, uint8_t, RED, GREEN, BLUE);
// };
#define DEFINE_ENUM(Name, Type, ...)                        \
    ENUM_COMMON(Name, Type, __VA_ARGS__)                    \
                                                            \
    public:                                                 \
        /* Needed to prevent if(c). */                      \
        constexpr explicit operator bool() const = delete;  \
    private:                                                \


// Similar to the above, except it also supports bit operations and bool
// conversion.
#define DEFINE_BITFLAG_ENUM(Name, Type, ...)                       \
    ENUM_COMMON(Name, Type, __VA_ARGS__);                          \
                                                                   \
    public:                                                        \
        constexpr Name& operator&=(Enum rhs) {                     \
            value = static_cast<Enum>(value & rhs);                \
            return *this;                                          \
        }                                                          \
        friend constexpr Name operator&(Enum lhs, Enum rhs) {      \
            return static_cast<Enum>(                              \
                static_cast<Type>(lhs) & static_cast<Type>(rhs));  \
        }                                                          \
                                                                   \
        constexpr Name& operator|=(Enum rhs) {                     \
            value = static_cast<Enum>(value | rhs);                \
            return *this;                                          \
        }                                                          \
        friend constexpr Name operator|(Enum lhs, Enum rhs) {      \
            return static_cast<Enum>(                              \
                static_cast<Type>(lhs) | static_cast<Type>(rhs));  \
        }                                                          \
                                                                   \
        friend constexpr Name operator~(Name rhs) {                \
            return static_cast<Enum>(~rhs.value);                  \
        }                                                          \
                                                                   \
        friend constexpr Enum operator~(Enum rhs) {                \
            return static_cast<Enum>(~static_cast<Type>(rhs));     \
        }                                                          \
                                                                   \
        constexpr operator bool() const {                          \
            return static_cast<bool>(value);                       \
        }                                                          \
    private:                                                       \

template<typename T>
concept special_enum = std::is_enum_v<typename T::Enum>;
