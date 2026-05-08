#ifndef ELEMENT_H
#define ELEMENT_H
#include <variant>
#include <string>
using Element = std::variant<int, double, std::string>;
inline bool isInt(const Element& e) { return std::holds_alternative<int>(e); }
inline bool isDouble(const Element& e) { return std::holds_alternative<double>(e); }
inline bool isString(const Element& e) { return std::holds_alternative<std::string>(e); }
#endif
