#include <cassert>
#include <map>
#include <queue>
#include <vector>

bool hasCycles(int n, const std::vector<std::pair<int,int>>& edges) {

    std::vector<int> inDegree(n+1, 0);
    std::map<int, std::vector<int>> adjGraph;

    for (auto& e : edges) {

        inDegree[e.second]++;
        adjGraph[e.first].push_back(e.second);
    }

    std::queue<int> q;
    int processed = 0;

    for (int i=1; i<=n; i++) {
        if (inDegree[i] == 0) {
            q.push(i);
        }
    }

    while (!q.empty()) {
        auto node = q.front();
        q.pop();

        processed++;

        auto it = adjGraph.find(node);
        if (it == adjGraph.end()) continue;

        for (auto i : it->second) {
            inDegree[i]--;

            if (inDegree[i] == 0) {
                q.push(i);
            }
        }
    }
    if (processed != static_cast<int>(inDegree.size()-1)) {
        return true;
    }
    return false;
}

int main() {

    std::vector<std::pair<int,int>> graph = {{1,2}, {1,3}, {3,4}, {1,4}};

    assert(!hasCycles(4, graph));

    graph = {{1,2}, {1,3}, {3,4}, {1,4}, {3,1}};

    assert(hasCycles(5, graph));

};
