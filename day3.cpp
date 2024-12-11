#include <iostream>
#include <regex>

using namespace std;

int main()
{
	regex mul_inst {"mul\\(([0-9]{1,3}),([0-9]{1,3})\\)"};

	vector<string> input;
	string line;
	while (getline(cin, line))
		input.push_back(line);

	int r = 0;

	for (auto line : input)
	{
		for (smatch sm; regex_search(line, sm, mul_inst);)
		{
			r += stoi(sm[1]) * stoi(sm[2]);
			line = sm.suffix();
		}
	}
	cout << r << '\n';

	int er = 0;
	bool en = true;
	for (auto line : input)
	{
		for (smatch sm; regex_search(line, sm, mul_inst);)
		{
			auto pre = sm.prefix().str();
			auto do_pos = pre.rfind("do()");
			auto dont_pos = pre.rfind("don't()");
			if (do_pos != string::npos && (dont_pos == string::npos || do_pos > dont_pos))
				en = true;
			else if (dont_pos != string::npos && (do_pos == string::npos || dont_pos > do_pos))
				en = false;
			if (en)
				er += stoi(sm[1]) * stoi(sm[2]);
			line = sm.suffix();
		}
	}
	cout << er << '\n';

	return 0;
}
