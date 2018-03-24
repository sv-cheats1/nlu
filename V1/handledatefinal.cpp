#include <bits/stdc++.h>
#include <dirent.h>

using namespace std;

vector<string> months;
vector<string> days;
vector<string> number_calendar;
vector<string> datetimeformats;
vector<string> dateformats;
vector<int> year;
vector<pair<string, string>> placeholder_values;
vector<pair<int, string>> number_orders;
int o_clock = false;

void preKMP(string pattern, int f[]) {
    int m = pattern.length(), k;
    f[0] = -1;
    for (int i = 1; i < m; i++) {
        k = f[i - 1];
        while (k >= 0) {
            if (pattern[k] == pattern[i - 1])
                break;
            else
                k = f[k];
        }

        f[i] = k + 1;
    }
}
 
int KMP(string pattern, string target) {
	transform(pattern.begin(), pattern.end(), pattern.begin(), ::tolower);
	transform(target.begin(), target.end(), target.begin(), ::tolower);
    int m = pattern.length();
    int n = target.length();
    int f[m];     
    preKMP(pattern, f);     
    int i = 0;
    int k = 0;        
    while (i < n) {
        if (k == -1) {
            i++;
            k = 0;
        }

        else if (target[i] == pattern[k]) {
            i++;
            k++;
            if (k == m)
                return i - k;
        }

        else
            k = f[k];
    }

    return -1;
}

void make_day_date_time_vectors() {
	vector<string> filenames;
	DIR *dir;
	struct dirent *ent;
	if ((dir = opendir ("DateTimeFormats")) != NULL) {
	  	while ((ent = readdir (dir)) != NULL) {
	    	filenames.push_back(ent->d_name);
	  	}	
	  	closedir (dir);
	} else {
	  	perror ("");
	  	return;
	}

	for (size_t i = 2; i < filenames.size(); ++i) {
		ifstream stream;
		if (filenames[i] == "month.txt") {
			char * file = new char[("DateTimeFormats/" + filenames[i]).length() + 1];
			std::strcpy(file, ("DateTimeFormats/" + filenames[i]).c_str());
			stream.open(file);
			char temp[250];
			while(stream) {
				stream.getline(temp, 250);
				if (stream) months.push_back(temp);
			}
			stream.close();
		}
		if (filenames[i] == "day.txt") {
			char * file = new char[("DateTimeFormats/" + filenames[i]).length() + 1];
			std::strcpy(file, ("DateTimeFormats/" + filenames[i]).c_str());
			stream.open(file);
			char temp[250];
			while(stream) {
				stream.getline(temp, 250);
				if (stream) days.push_back(temp);
			}
			stream.close();
		}
		if (filenames[i] == "datetimeformats.txt") {
			char * file = new char[("DateTimeFormats/" + filenames[i]).length() + 1];
			std::strcpy(file, ("DateTimeFormats/" + filenames[i]).c_str());
			stream.open(file);
			char temp[250];
			while(stream) {
				stream.getline(temp, 250);
				if (stream) datetimeformats.push_back(temp);
			}
			stream.close();
			reverse(datetimeformats.begin(), datetimeformats.end());
		}
		if (filenames[i] == "dateformats.txt") {
			char * file = new char[("DateTimeFormats/" + filenames[i]).length() + 1];
			std::strcpy(file, ("DateTimeFormats/" + filenames[i]).c_str());
			stream.open(file);
			char temp[250];
			while(stream) {
				stream.getline(temp, 250);
				if (stream) dateformats.push_back(temp);
			}
			stream.close();
			reverse(dateformats.begin(), dateformats.end());
		}
	}
	for (int i = 1; i <= 31; ++i) {
		number_calendar.push_back(to_string(i));
	}
}

string replace(string s, vector<string> v, string replacement) {
	vector<string>::iterator it;
	for (it = v.begin(); it != v.end(); ++it) {
		int flag = KMP(" " + *it + " ", s);
		if (flag != -1) {
			if (replacement == "<number>") {
				string placeholder = s.substr(flag + 1, (*it).length());
				number_orders.push_back(make_pair((flag + 1), placeholder));
			}
			else {
				string placeholder = s.substr(flag + 1, (*it).length());
				placeholder_values.push_back(make_pair(replacement, placeholder));
			}
			s.erase(flag, (*it).length() + 2);
			s.insert(flag, " " + replacement + " ");
		}
	}
	return s;
}

string replace_year(string s) {
	int count = 0;
	for (int i = 0; i < s.length(); ++i) {
		if (s[i] >= 48 && s[i] <= 57 || s[i] == 32) {
			if (s[i] != 32)
				year.push_back(s[i] - 48);
			count++;
		} else {
			count = 0;
			year.clear();
		}
		if (count == 6) {
			stringstream result;
			copy(year.begin(), year.end(), ostream_iterator<int>(result, ""));
			placeholder_values.push_back(make_pair("<year>", result.str()));
			s.erase(i-5, 6);
			s.insert(i-5, " <year> ");
			break;
		}
	}
	return s;
}

string replace_colon_time(string s) {
	regex r1("( )([[:digit:]]{1})(:)([[:digit:]]{2})");
	regex r2("( )([[:digit:]]{2})(:)([[:digit:]]{2})");
	smatch sm;
	if (regex_search(s, sm, r1)) {
		string placeholder = s.substr(sm.position(1) + 1, 4);
		number_orders.push_back(make_pair((sm.position(1)), placeholder));
		s.erase(sm.position(1) + 1, 4);
		s.insert(sm.position(1) + 1, "<number>");
	}
	if (regex_search(s, sm, r2)) {
		string placeholder = s.substr(sm.position(1) + 1, 5);
		number_orders.push_back(make_pair((sm.position(1)), placeholder));
		s.erase(sm.position(1) + 1, 5);
		s.insert(sm.position(1) + 1, "<number>");
	}
	return s;
}

string replace_o_clock(string s) {
	int pos1 = KMP("O clock morning", s);
	int pos2 = KMP("O'clock morning", s);
	if (pos1 != -1) { 
		s.erase(pos1, 15);
		s.insert(pos1, "am");
		o_clock = 1;
	}
	if (pos2 != -1) {
		s.erase(pos2, 15);
		s.insert(pos2, "am");
		o_clock = 1;
	}
	return s;
}

void handle_number_orders() {
	sort(number_orders.begin(), number_orders.end());
	vector<pair<int, string>>::iterator it;
	for (it = number_orders.begin(); it != number_orders.end(); ++it) {
		placeholder_values.push_back(make_pair("<number>", it->second));
	}
}

string reconstruct_datetime() {
	vector<pair<string, string>>::iterator it;
	if (placeholder_values.size() > 0) {
		it = placeholder_values.end() - 1;
		if (it->first == "<dateTime>") {
			string s = it->second;
			it--;
			if (it->first == "<date>") {
				int o_flag = KMP("<date>", s);
				s.erase(o_flag, 6);
				s.insert(o_flag, it->second);
				for (int i = 0; i < placeholder_values.size() - 2; ++i) {
					int flag = KMP(placeholder_values[i].first, s);
					if (flag != -1) {
						s.erase(flag, (placeholder_values[i].first).length());
						s.insert(flag, placeholder_values[i].second);
					}
				}
			} else {
				for (int i = 0; i < placeholder_values.size() - 1; ++i) {
					int flag = KMP(placeholder_values[i].first, s);
					if (flag != -1) {
						s.erase(flag, (placeholder_values[i].first).length());
						s.insert(flag, placeholder_values[i].second);
					}
				}
			}
			return s;
		}
		return "";
	}
	return "";
}

string date_time_string() {
	make_day_date_time_vectors();
	string s = "on 5 a.m. tomorrow make build body alarm please ";
	string save = s;
	s = replace_o_clock(s);
	s = replace(s, number_calendar, "<number>");
	s = replace_colon_time(s);
	handle_number_orders();
	s = replace(s, days, "<day>");
	s = replace(s, months, "<month>");
	s = replace_year(s);
	s = replace(s, dateformats, "<date>");
	s = replace(s, datetimeformats, "<dateTime>");
	if (KMP("<dateTime>", s) != -1) {
		return s;
	}
	return save;
}

string date_time_value() {
	string reconstructed = reconstruct_datetime();
	reconstructed += " ";
	if (reconstructed.length() > 1) {
		if (o_clock == 1) {
			int flag = KMP(" am ", reconstructed);
			if (flag != -1) {
				reconstructed.erase(flag + 1, 2);
				reconstructed.insert(flag + 1, "O clock");
			}
		}
		return reconstructed;
	}
	return "";
}

int main() {
	string date_time_str = date_time_string();
	string date_time_val = date_time_value();
	cout << date_time_str << endl;
	cout << date_time_val << endl;
}
