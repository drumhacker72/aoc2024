#include <algorithm>
#include <iostream>
#include <map>
#include <set>
#include <vector>

using namespace std;

struct Computer
{
    uint16_t value;

    char first() const { return static_cast<char>(value >> 8); }
    char second() const { return static_cast<char>(value); }

    friend bool operator==(Computer, Computer) = default;
    friend auto operator<=>(Computer, Computer) = default;

    friend ostream& operator<<(ostream& os, Computer self)
    {
        return os << self.first() << self.second();
    }

    friend istream& operator>>(istream& is, Computer& self)
    {
        char c1, c2;
        is >> c1 >> c2;
        self.value = static_cast<uint16_t>(c1) << 8 | static_cast<uint16_t>(c2);
        return is;
    }
};

using Connections = map<Computer, set<Computer>>;

vector<set<Computer>> maximal_cliques(Connections const& n, set<Computer> r, set<Computer> p, set<Computer> x)
{
    if (p.empty() && x.empty())
        return {r};

    vector<Computer> px;
    ranges::set_union(p, x, back_inserter(px));
    ranges::sort(px, greater{}, [&n](Computer c) { return n.at(c).size(); });
    Computer u = px.front();

    vector<set<Computer>> cliques;
    while (!p.empty())
    {
        Computer v = *p.begin();
        if (n.at(u).contains(v))
        {
            p.erase(v);
            continue;
        }
        set<Computer> r2 = r;
        r2.insert(v);
        set<Computer> p2, x2;
        ranges::set_intersection(p, n.at(v), inserter(p2, p2.end()));
        ranges::set_intersection(x, n.at(v), inserter(x2, x2.end()));
        for (auto&& c : maximal_cliques(n, r2, p2, x2))
            cliques.push_back(c);
        p.erase(v);
        x.insert(v);
    }
    return cliques;
}

int main()
{
    Connections connections;
    {
        Computer c1, c2;
        char hyphen;
        while (cin >> c1 >> hyphen >> c2)
        {
            if (hyphen != '-')
                throw runtime_error{"bad input"};
            connections[c1].insert(c2);
            connections[c2].insert(c1);
        }
    }
    int t_count = 0;
    for (auto [c1, c1_conns] : connections)
    {
        if (c1.first() != 't')
            continue;
        for (auto c2 : c1_conns)
        {
            if (c2.first() == 't' && c2 < c1)
                continue;
            for (auto c3 : c1_conns)
            {
                if (c3 <= c2)
                    continue;
                if (c3.first() == 't' && c3 < c1)
                    continue;
                if (!connections[c2].contains(c3))
                    continue;
                ++t_count;
            }
        }
    }
    cout << t_count << '\n';

    vector<Computer> computers;
    for (auto [c, _] : connections)
        computers.push_back(c);
    ranges::sort(computers, greater{}, [&connections](Computer c) { return connections[c].size(); });
    set<Computer> largest;
    for (auto c : computers)
    {
        if (connections[c].size()+1 < largest.size())
            break;
        auto cliques = maximal_cliques(connections, {}, connections[c], {});
        if (!cliques.empty())
        {
            auto clique = ranges::max(cliques, {}, &set<Computer>::size);
            clique.insert(c);
            largest = ranges::max(largest, clique, {}, &set<Computer>::size);
        }
    }
    auto it = largest.begin();
    cout << *it++;
    for (; it != largest.end(); ++it)
        cout << ',' << *it;
    cout << '\n';
    return 0;
}
