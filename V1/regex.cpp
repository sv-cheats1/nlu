#include <bits/stdc++.h>
#include <regex>

using namespace std;

int main() {
	regex r1("( )([[:digit:]]{1})(:)([[:digit:]]{2})");
	regex r2("( )([[:digit:]]{2})(:)([[:digit:]]{2})");
	string s("assdgtahuas 15:30");
	smatch sm;
	cout << regex_search(s, sm, r1) << endl;
	cout << sm.position(1) << "\n";
	cout << regex_search(s, sm, r2) << endl;
	cout << sm.position(1) << "\n";
	// cout << regex_match(s, r);
}