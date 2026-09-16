#include <iostream>
#include <vector>
#include <algorithm>
#include <utility>

/*
 * There are n items with various weights and values. Add them up to be the max value using weight w or less.
 */

int findOptimalUnbounded(std::vector<std::pair<int,int>> items, int maxWeight) {

    std::vector<int> m(maxWeight+1, 0);

    for (int i=1; i<maxWeight+1; i++) {
        for (auto [w, v] : items) {
            if (i-w < 0) continue;
            m[i] = std::max(m[i], v + m[i-w]);
        }
    }
    return m[maxWeight];
}

int findOptimalBounded(std::vector<std::pair<int,int>> items, int maxWeight, int bound) {

    std::vector<std::vector<int>> m(items.size(), std::vector<int>(maxWeight+1, 0));

    for (int i=0; i<items.size(); i++) {
        for (int j=0; j<maxWeight+1; j++) {
            if (items[i].first > j) {
                m[i][j] = m[i-1][j];
            } else {
                m[i][j] = std::max(m[i-1][j], m[i-1][j-items[i].first] + items[i].second);
            }
        }
    }

    return m[items.size()-1][maxWeight];
}

int main() {

    std::vector<std::pair<int,int>> items;
    items.emplace_back(5, 10);
    items.emplace_back(1, 1);
    items.emplace_back(10, 100);
    items.emplace_back(2, 3);

    int unbounded = findOptimalUnbounded(items, 50000000);
    std::cout << "Max value (Unbounded): " << unbounded << std::endl;

    int bounded = findOptimalBounded(items, 50000000, 1);
    std::cout << "Max value (Bounded):" << bounded << std::endl;
}
