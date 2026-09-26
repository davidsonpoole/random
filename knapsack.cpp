#include <iostream>
#include <vector>
#include <algorithm>
#include <utility>

/*
 * There are n items with various weights and values. Add them up to be the max value using weight w or less.
 */

int findOptimalUnbounded(std::vector<int>& weights, std::vector<int>& profits, int maxWeight) {

    std::vector<int> m(maxWeight+1, 0);

    for (int i=1; i<maxWeight+1; i++) {
        for (int j=0; j<weights.size(); j++) {
            int w = weights[j];
            int p = profits[j];
            if (i-w < 0) continue;
            m[i] = std::max(m[i], p + m[i-w]);
        }
    }
    return m[maxWeight];
}

int findOptimalBounded(std::vector<int>& weights, std::vector<int>& profits, int maxWeight, int bound) {

    std::vector<int> m(maxWeight+1, 0);

    for (int i=0; i<weights.size(); i++) {
        int w = weights[i];
        int p = profits[i];
        for (int c = 0; c < bound; c++) {
            for (int j = maxWeight; j >= w; j--) {
                m[j] = std::max(m[j], m[j-w] + p);
            }
        }
        // print out vector after each pass
        for (auto e : m) {
            std::cout << e << ",";
        }
        std::cout << std::endl;
    }

    return m[maxWeight];
}

/*
 * The key idea with this solution is building a table that represents different amounts
 * of items and capacities. At capacity[w], you check "what is greater, the best I got
 * only using items[0..(i-1)], or this new item[i] PLUS the best I got at capacity minus
 * the current weight. Then we use that."
 *
 * Ex: So if we're looking at capacity=5 i=4 and the 4th item looks like (weight=2, value=4)
 * we would check the score of capacity=5, i=3, and also capacity=((5-2)=3 + 4)
 */
int findOptimalBounded2D(std::vector<int>& weights, std::vector<int>& profits, int maxWeight, int bound, std::vector<int>& items) {

    std::vector<std::pair<int,int>> expanded;
    for (int i=0; i<weights.size(); i++) {
        int w = weights[i];
        int p = profits[i];
        for (int c = 0; c < bound; c++) {
            expanded.emplace_back(w, p);
        }
    }

    int n = expanded.size();
    std::vector<std::vector<int>> m(n+1, std::vector<int>(maxWeight+1, 0));

    for (int i = 1; i <= n; i++) {
        auto [w, v] = expanded[i-1];
        for (int j = 0; j <= maxWeight; j++) {
            if (w > j) {
                m[i][j] = m[i-1][j];
            } else {
                m[i][j] = std::max(m[i-1][j], m[i-1][j-w] + v);
            }
        }
    }
    //
    // go back up
    int w = maxWeight;
    for (int i=n; i>=1; i--) {
        if (m[i][w] != m[i-1][w]) {
            // this item must be included
            items.push_back(i-1);
            w -= weights[i-1];
        }
    }

    std::reverse(items.begin(), items.end());

    return m[n][maxWeight];
}

int main() {

    std::vector<int> weights = {3,2,4,5,1};
    std::vector<int> profits = {50,40,70,80,10};

    int unbounded = findOptimalUnbounded(weights, profits, 50000000);
    std::cout << "Max value (Unbounded): " << unbounded << std::endl;

    int bounded = findOptimalBounded(weights, profits, 5, 1);
    std::cout << "Max value (Bounded):" << bounded << std::endl;

    std::vector<int> result;
    int bounded2D = findOptimalBounded2D(weights, profits, 50000000, 1, result);
    std::cout << "Max value (Bounded 2D):" << bounded2D << std::endl;
    for (auto i : result) {
        std::cout << "Item " << i << ": weight: " << weights[i] << " val: " << profits[i] << std::endl;
    }
}
