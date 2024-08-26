#include <game/a_star.hpp>

#include <iostream>
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

    dis_.resize(n_ * m_);
    prev_.resize(n_ * m_);
}

std::vector<PII> AStar::operator()(PII s, PII t) {
    if (s.first < 0 || s.first >= n_ || s.second < 0 || s.second >= m_ ||
        t.first < 0 || t.first >= n_ || t.second < 0 || t.second >= m_ ||
        map_[s.first][s.second] || map_[t.first][t.second]) {
        return {};
    }

    int ss = s.first * m_ + s.second;
    int tt = t.first * m_ + t.second;

    // clear
    std::fill(dis_.begin(), dis_.end(), 1e9);
    dis_[ss] = 0;
    std::fill(prev_.begin(), prev_.end(), -1);

    std::priority_queue<PII, std::vector<PII>, std::greater<PII>> pq;
    pq.emplace(0, ss);
    while (pq.size()) {
        auto [d, u] = pq.top(); pq.pop();
        if (u == tt) break;
        int i = u / m_, j = u % m_;
        for (auto [di, dj] : adj_) {
            int ii = i + di, jj = j + dj;
            if (ii < 0 || ii >= n_ || jj < 0 || jj >= m_ || map_[ii][jj]) continue;
            int dd = (i != ii && j != jj) ? 14 : 10;
            int v = ii * m_ + jj;
            if (dis_[u] + dd < dis_[v]) {
                dis_[v] = dis_[u] + dd;
                pq.emplace(dis_[v] + heuristic({i, j}, t), v);
                prev_[v] = u;
            }
        }
    }

    if (prev_[tt] == -1) {
        return {};
    }

    int cur = tt;
    std::vector<PII> path;
    while (cur != ss) {
        int i = cur / m_, j = cur % m_;
        path.emplace_back(i, j);
        cur = prev_[cur];
    }
    path.emplace_back(s);
    std::reverse(path.begin(), path.end());

    return path;
}

int AStar::heuristic(PII u, PII v) {
    return std::abs(u.first - v.first) + std::abs(u.second - v.second);
}

}  // namespace wheel
