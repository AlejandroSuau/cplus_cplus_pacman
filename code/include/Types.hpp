#pragma once

#include <array>
#include <vector>
#include <memory>
#include <optional>

class Ghost;
class Collectable;

using GhostList = std::array<std::unique_ptr<Ghost>, 4>;
using OptionalGhostReference = std::optional<std::reference_wrapper<const Ghost>>;

using CollectableList = std::vector<std::unique_ptr<Collectable>>;