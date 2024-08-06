#pragma once

#include <vector>
#include <utility>

namespace wheel {

class AStar {
public:
    AStar(const std::vector<std::vector<bool>>& map, int direction = 4);
    ~AStar() = default;

    using PII = std::pair<int, int>;

    std::vector<PII> operator()(PII s, PII t);

private:
    const std::vector<std::vector<bool>>& map_;
    std::vector<PII> adj_;
    int n_, m_;
};

}  // namespace wheel
