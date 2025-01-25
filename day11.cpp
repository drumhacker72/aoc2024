#include <charconv>
#include <iostream>
#include <map>

using namespace std;

optional<pair<uint64_t, uint64_t>> split(uint64_t stone)
{
    constexpr size_t buf_size = 15; // 999999999999999*2024 will still fit in 64-bit
    char buf[buf_size];
    auto res = to_chars(buf, buf + buf_size, stone);
    if (res.ec != errc{})
        throw runtime_error{"number too big"};
    auto len = res.ptr - buf;
    if (len % 2)
        return nullopt;
    uint64_t l, r;
    from_chars(buf, buf + len / 2, l);
    from_chars(buf + len / 2, buf + len, r);
    return make_pair(l, r);
}

map<uint64_t, uint64_t> step(map<uint64_t, uint64_t> stones)
{
    map<uint64_t, uint64_t> next;
    for (auto [stone, count] : stones)
    {
        if (stone == 0)
            next[1] += count;
        else
        {
            auto r = split(stone);
            if (r)
            {
                next[r->first] += count;
                next[r->second] += count;
            }
            else
                next[stone * 2024] += count;
        }
    }
    return next;
}

uint64_t count(map<uint64_t, uint64_t> stones)
{
    uint64_t n = 0;
    for (auto [stone, count] : stones)
        n += count;
    return n;
}

int main()
{
    map<uint64_t, uint64_t> stones;
    uint64_t n;
    while (cin >> n)
        ++stones[n];
    int blink = 0;
    for (; blink < 25; ++blink)
        stones = step(stones);
    cout << count(stones) << '\n';
    for (; blink < 75; ++blink)
        stones = step(stones);
    cout << count(stones) << '\n';
    return 0;
}
