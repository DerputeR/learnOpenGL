#pragma once
#include <string>
struct Printable {
	virtual std::string toString() const = 0;
};