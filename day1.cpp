#include <algorithm>
#include <iostream>
#include <vector>

using namespace std;

int main()
{
	vector<int> ls, rs;
	int l, r;
	while (cin >> l >> r)
	{
		ls.push_back(l);
		rs.push_back(r);
	}
	sort(ls.begin(), ls.end());
    sort(rs.begin(), rs.end());
    int d = 0;
    for (int i = 0; i < ls.size(); ++i)
        d += abs(ls[i] - rs[i]);
    cout << d << '\n';
    int s = 0;
    for (auto&& l : ls)
        s += l * count(rs.begin(), rs.end(), l);
    cout << s << '\n';
	return 0;
}
