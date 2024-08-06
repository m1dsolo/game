#include <game/a_star.hpp>

#include <algorithm>
#include <queue>

namespace wheel {

using PII = std::pair<int, int>;

AStar::AStar(const std::vector<std::vector<bool>>& map, int direction) : map_(map) {
    n_ = map.size();
    m_ = map[0].size();
    if (direction == 4) {
        adj_ = {{-1, 0}, {1, 0}, {0, -1}, {0, 1}};
    } else if (direction == 8) {
        adj_ = {{-1, 0}, {1, 0}, {0, -1}, {0, 1}, {-1, -1}, {-1, 1}, {1, -1}, {1, 1}};
    }
}

std::vector<PII> AStar::operator()(PII s, PII t) {
    if (s.first < 0 || s.first >= n_ || s.second < 0 || s.second >= m_ ||
        t.first < 0 || t.first >= n_ || t.second < 0 || t.second >= m_ ||
        map_[s.first][s.second] || map_[t.first][t.second]) {
        return {};
    }

    std::vector<PII> path;
    std::priority_queue<PII, std::vector<PII>, std::greater<PII>> open;
    open.push({0, s.first * m_ + s.second});
    std::vector<std::vector<bool>> vis(n_, std::vector<bool>(m_, false));
    std::vector<std::vector<int>> dis(n_, std::vector<int>(m_, 1e9));
    dis[s.first][s.second] = 0;
    std::vector<std::vector<PII>> prev(n_, std::vector<PII>(m_, {-1, -1}));
    while (open.size()) {
        auto [d, u] = open.top(); open.pop();
        int i = u / m_, j = u % m_;
        if (i == t.first && j == t.second) break;
        if (vis[i][j]) continue;
        vis[i][j] = true;
        for (auto [di, dj] : adj_) {
            int ii = i + di, jj = j + dj;
            if (ii < 0 || ii >= n_ || jj < 0 || jj >= m_ || map_[ii][jj]) continue;
            // int dd = d + ((i != ii && j != jj) ? 14 : 10);
            int dd = (i != ii && j != jj) ? 14 : 10;
            if (dis[i][j] + dd < dis[ii][jj]) {
                open.emplace(dis[ii][jj] = dis[i][j] + dd, ii * m_ + jj);
                prev[ii][jj] = {i, j};
            }
        }
    }

    int i = t.first, j = t.second;
    if (prev[i][j].first == -1) {
        return {};
    }
    while (i != s.first || j != s.second) {
        path.emplace_back(i, j);
        auto [pi, pj] = prev[i][j];
        i = pi, j = pj;
    }
    path.push_back(s);
    std::reverse(path.begin(), path.end());

    return path;
}

}  // namespace wheel
