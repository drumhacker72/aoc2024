#include <iostream>
#include <map>
#include <queue>
#include <set>
#include <vector>

using namespace std;

struct Vec2
{
    int x;
    int y;
    bool operator==(Vec2 const& v) const { return x == v.x && y == v.y; }
    bool operator<(Vec2 const& v) const { return x < v.x || x == v.x && y < v.y; }
};

struct Node
{
    int score;
    Vec2 pos;
    Vec2 dir;
    set<Vec2> reachable;
    bool operator<(Node const& n) const { return score > n.score; }
};

int main()
{
    vector<string> grid;
    string line;
    Vec2 start, end;
    while (getline(cin, line))
    {
        int y = static_cast<int>(grid.size());
        if (auto pos = line.find('S'); pos != string::npos)
            start = {static_cast<int>(pos), y};
        if (auto pos = line.find('E'); pos != string::npos)
            end = {static_cast<int>(pos), y};
        grid.push_back(line);
    }
    optional<int> best_score;
    priority_queue<Node> pq;
    map<pair<Vec2, Vec2>, pair<int, set<Vec2>>> seen;
    pq.push({0, start, {1, 0}, {}});
    pq.push({1000, start, {0, -1}, {}});
    pq.push({1000, start, {0, 1}, {}});
    pq.push({2000, start, {-1, 0}, {}});
    while (!pq.empty())
    {
        auto node = pq.top();
        pq.pop();
        if (best_score && node.score > *best_score)
            break;
        auto key = make_pair(node.pos, node.dir);
        auto reachable = std::move(node.reachable);
        reachable.insert(node.pos);
        if (auto it = seen.find(key); it != seen.end())
        {
            if (node.score > it->second.first)
                continue;
            else if (node.score == it->second.first)
                reachable.merge(it->second.second);
        }
        seen[key] = make_pair(node.score, reachable);
        if (node.pos == end)
            best_score = node.score;
        Vec2 target {node.pos.x + node.dir.x, node.pos.y + node.dir.y};
        if (grid[target.y][target.x] != '#')
        {
            pq.push({node.score + 1, target, node.dir, reachable});
            pq.push({node.score + 1001, target, {node.dir.y, -node.dir.x}, reachable});
            pq.push({node.score + 1001, target, {-node.dir.y, node.dir.x}, reachable});
        }
    }
    cout << *best_score << '\n';
    set<Vec2> reachable;
    reachable.merge(seen[make_pair(end, Vec2{-1, 0})].second);
    reachable.merge(seen[make_pair(end, Vec2{1, 0})].second);
    reachable.merge(seen[make_pair(end, Vec2{0, -1})].second);
    reachable.merge(seen[make_pair(end, Vec2{0, 1})].second);
    cout << reachable.size() << '\n';
    return 0;
}
