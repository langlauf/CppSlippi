#pragma once

#include <array>

#include "CppSlippi/Model/Enums/Port.h"


namespace cpp_slippi::utils {

// A wrapper for std::array that takes Ports as indices, and default to size 4.
template<typename T>
class PortArray {
public:
	using value_type = T;

	constexpr T& at(Port p) {
		return _array_.at(p);
	}

	constexpr const T& at(Port p) const {
		return _array_.at(p);
	}

	constexpr T& operator[](Port p) {
		return _array_[p];
	}

	constexpr const T& operator[](Port p) const {
		return _array_[p];
	}

	constexpr T& front() {
		return _array_.front();
	}

	constexpr const T& front() const {
		return _array_.front();
	}

	constexpr T& back() {
		return _array_.back();
	}

	constexpr const T& back() const {
		return _array_.back();
	}

	constexpr T* data() {
		return _array_.data();
	}

	constexpr const T* data() const {
		return _array_.data();
	}

	constexpr auto begin() {
		return _array_.begin();
	}

	constexpr auto begin() const {
		return _array_.begin();
	}

	constexpr auto cbegin() const {
		return _array_.cbegin();
	}

	constexpr auto end() {
		return _array_.end();
	}

	constexpr auto end() const {
		return _array_.end();
	}

	constexpr auto cend() const {
		return _array_.cend();
	}

	constexpr auto rbegin() {
		return _array_.rbegin();
	}

	constexpr auto rbegin() const {
		return _array_.rbegin();
	}

	constexpr auto crbegin() const {
		return _array_.crbegin();
	}

	constexpr auto rend() {
		return _array_.rend();
	}

	constexpr auto rend() const {
		return _array_.rend();
	}

	constexpr auto crend() const {
		return _array_.crend();
	}

	constexpr bool empty() const {
		return _array_.empty();
	}

	constexpr size_t size() const {
		return _array_.size();
	}

	constexpr size_t max_size() const {
		return _array_.max_size();
	}

	constexpr void fill(const T& value) {
		_array_.fill(value);
	}

	constexpr void swap(PortArray& other) {
		_array_.swap(other._array_);
	}

	friend constexpr auto operator<=>(const PortArray& lhs, const PortArray& rhs) = default;

	// Public only to enable aggregate initialization. Treat as if private.
	std::array<T, 4> _array_;
};

}  // namespace cpp_slippi::utils
