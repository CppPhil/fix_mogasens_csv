#include <algorithm>

#include "remove_zeros_from_field.hpp"

namespace fmc {
void removeZerosFromField(std::string* field)
{
  const auto it{std::find(field->rbegin(), field->rend(), '.')};

  if (it == field->rend()) { // Contains no dot.
    return;
  }

  if (std::all_of(it.base(), field->end(), [](char c) { return c == '0'; })) {
    field->erase(it.base() - 1, field->end());
  }
}
} // namespace fmc