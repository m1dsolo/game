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
    std::vector<int> dis_;
    std::vector<int> prev_;
    int n_, m_;

    int heuristic(PII u, PII v);
    bool line_of_sight(const PII& from, const PII& to);
    bool have_obstacle(int x, int y);
};

}  // namespace wheel
