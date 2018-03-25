#include <bits/stdc++.h>
#include <dirent.h>

using namespace std;

struct compare {
    bool operator()(const std::string& first, const std::string& second) {
        return first.size() > second.size();
    }
} c;

map<string, vector<string> > concepts;
map<string, vector<string> > placeholder;
map<string, pair<set<string>, pair< set<string>, set<string> > > > grammar;
unordered_set<string> reject_words;
unordered_set<string> conjunctions;
unordered_set<string> open_phrase_names;
map<string, int> counters;
map<string, vector<string> > grammar_words;
map<string, set<string> > placeholder_values;
map<string, vector<string> > grammar_lines;
vector<string> months;
vector<string> days;
vector<string> number_calendar;
vector<string> datetimeformats;
vector<string> dateformats;
vector<int> year;
int o_clock = false;

void prekmp(string pattern, int f[]);
int kmp(string pattern, string target);
void make_day_date_time_vectors();
string replace(string s, vector<string> v, string replacement, vector<pair<int, string>> &number_orders, vector<pair<string, string>> &placeholder_values1);
string replace_year(string s, vector<pair<string, string>> &placeholder_values1);
string replace_colon_time(string s, vector<pair<int, string>> &number_orders);
void handle_number_orders(vector<pair<int, string>> &number_orders, vector<pair<string, string>> &placeholder_values1);
string replace_o_clock(string s);
string reconstruct_datetime(vector<pair<string, string>> &placeholder_values1);
string date_time_value(vector<pair<string, string>> &placeholder_values1);
string date_time_string(string s);
bool replace(std::string& str, const std::string& from, const std::string& to);
int give_grammar_line_score(string gram, string input_str, int index) ;
string replace_concepts(string input_str, int indexofline, string gram);
void make_open_phrases();
void return_open_phrases(string input_str, int indexofline, string gram);
string return_placeholders(string gram, string input_str, vector<string> v_c);
void make_grammar_words();
bool is_equal(string s1, string s2);
void print_vec(vector<string> a);
void make_placeholder();
void make_grammar_lines();
void make_grammar();
void make_custom_resource();
void make_concept();
void make_conjunctions();
void preKMP(string pattern, int f[]);
bool KMP(string pattern, string target);
bool is_concept_present(vector<string> v, string s);
vector<string> concepts_that_are_present(string s);
bool is_placeholder_present(vector<string> v, string s);
vector<string> placeholders_that_are_present(string s);
void make_counters();
void find_grammar_scores_concept(vector<string> v);
void find_grammar_scores_placeholder(vector<string> v);
void find_grammar_scores_keywords(string input_str);
pair<string, string> split(string str, string conjunction);
string command(string input_str);
string give_command(string input_str);

void print_set(set<string> set_to_print) {
	set<string>::iterator set_iter;
	for(set_iter = set_to_print.begin(); set_iter != set_to_print.end(); ++set_iter) {
		cout << *set_iter << " ";
	}
	cout << endl;
}

void prekmp(string pattern, int f[]) {
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
 
int kmp(string pattern, string target) {
	transform(pattern.begin(), pattern.end(), pattern.begin(), ::tolower);
	transform(target.begin(), target.end(), target.begin(), ::tolower);
    int m = pattern.length();
    int n = target.length();
    int f[m];     
    prekmp(pattern, f);     
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

string replace(string s, vector<string> v, string replacement, vector<pair<int, string>> &number_orders, vector<pair<string, string>> &placeholder_values1) {
	vector<string>::iterator it;
	for (it = v.begin(); it != v.end(); ++it) {
		int flag = kmp(" " + *it + " ", s);
		if (flag != -1) {
			if (replacement == "<number>") {
				string placeholder = s.substr(flag + 1, (*it).length());
				number_orders.push_back(make_pair((flag + 1), placeholder));
			}
			else {
				string placeholder = s.substr(flag + 1, (*it).length());
				placeholder_values1.push_back(make_pair(replacement, placeholder));
			}
			s.erase(flag, (*it).length() + 2);
			s.insert(flag, " " + replacement + " ");
		}
	}
	return s;
}

string replace_year(string s, vector<pair<string, string>> &placeholder_values1) {
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
			placeholder_values1.push_back(make_pair("<year>", result.str()));
			s.erase(i-5, 6);
			s.insert(i-5, " <year> ");
			break;
		}
	}
	return s;
}

string replace_colon_time(string s, vector<pair<int, string>> &number_orders) {
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
	int pos1 = kmp("O clock morning", s);
	int pos2 = kmp("O'clock morning", s);
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

void handle_number_orders(vector<pair<int, string>> &number_orders, vector<pair<string, string>> &placeholder_values1) {
	sort(number_orders.begin(), number_orders.end());
	vector<pair<int, string>>::iterator it;
	for (it = number_orders.begin(); it != number_orders.end(); ++it) {
		placeholder_values1.push_back(make_pair("<number>", it->second));
	}
}

string reconstruct_datetime(vector<pair<string, string>> &placeholder_values1) {
	vector<pair<string, string>>::iterator it;
	if (placeholder_values1.size() > 0) {
		it = placeholder_values1.end() - 1;
		if (it->first == "<dateTime>") {
			string s = it->second;
			it--;
			if (it->first == "<date>") {
				int o_flag = kmp("<date>", s);
				s.erase(o_flag, 6);
				s.insert(o_flag, it->second);
				for (int i = 0; i < placeholder_values1.size() - 2; ++i) {
					int flag = kmp(placeholder_values1[i].first, s);
					if (flag != -1) {
						s.erase(flag, (placeholder_values1[i].first).length());
						s.insert(flag, placeholder_values1[i].second);
					}
				}
			} else {
				for (int i = 0; i < placeholder_values1.size() - 1; ++i) {
					int flag = kmp(placeholder_values1[i].first, s);
					if (flag != -1) {
						s.erase(flag, (placeholder_values1[i].first).length());
						s.insert(flag, placeholder_values1[i].second);
					}
				}
			}
			return s;
		}
		return "";
	}
	return "";
}

string date_time_value(vector<pair<string, string>> &placeholder_values1) {
	string reconstructed = reconstruct_datetime(placeholder_values1);
	reconstructed += " ";
	if (reconstructed.length() > 1) {
		if (o_clock == 1) {
			int flag = kmp(" am ", reconstructed);
			if (flag != -1) {
				reconstructed.erase(flag + 1, 2);
				reconstructed.insert(flag + 1, "O clock");
			}
		}
		return reconstructed;
	}
	return "";
}

string date_time_string(string s) {
	make_day_date_time_vectors();
	string save = s;
	vector<pair<int, string>> number_orders;
	vector<pair<string, string>> placeholder_values1;
	s = replace_o_clock(s);
	s = replace(s, number_calendar, "<number>", number_orders, placeholder_values1);
	s = replace_colon_time(s, number_orders);
	handle_number_orders(number_orders, placeholder_values1);
	s = replace(s, days, "<day>", number_orders, placeholder_values1);
	s = replace(s, months, "<month>", number_orders, placeholder_values1);
	s = replace_year(s, placeholder_values1);
	s = replace(s, dateformats, "<date>", number_orders, placeholder_values1);
	s = replace(s, datetimeformats, "<dateTime>", number_orders, placeholder_values1);

	string date_time_val = date_time_value(placeholder_values1);
	set<string> temp;
	temp.insert(date_time_val);
	if(date_time_val.length() > 0) placeholder_values.insert(make_pair(string("<dateTime>"), temp));
	if (kmp("<dateTime>", s) != -1) {
		return s;
	}
	return save;
}

bool replace(std::string& str, const std::string& from, const std::string& to) {
    size_t start_pos = str.find(from);
    string why = to;

    if(start_pos == std::string::npos)
        return false;

    if(why.at(0) == '<') placeholder_values[to].insert(from);
    str.replace(start_pos, from.length(), to);
    return true;
}

int give_grammar_line_score(string gram, string input_str, int index) {
	vector<string> :: iterator itr;
	itr = grammar_lines[gram].begin();
	itr += index;
	string gramline = *itr;
	vector<string> v_c_l;
	transform(gramline.begin(), gramline.end(), gramline.begin(), ::tolower);
	int score = 0;

    istringstream iss(gramline);
    string word;
    unordered_set<string> :: iterator reject;
    int strindex = 0;
    while(iss >> word) {
    	reject = reject_words.find(word);
    	if(reject != reject_words.end()) {
    		continue;
    	}

    	else {
    		if(word.at(0) == '{') {
    			vector<string> :: iterator overconcepts;
    			for(overconcepts = concepts[word].begin(); overconcepts != concepts[word].end(); ++overconcepts) {
    				int f = input_str.find(*overconcepts, strindex);
    				if(f != string::npos) {
    					strindex = f + word.length() - 1;
    					score++;
    					break;
    				}
    			}
    		}

    		int F = input_str.find(word, strindex);
    		if(F != string::npos) {
    			strindex = F + word.length() - 1;
    			score++;
    		}
    	}
    }

    return score;
}

string replace_concepts(string input_str, int indexofline, string gram) {
	vector<string> :: iterator line = grammar_lines[gram].begin();
	line += indexofline;
	string gramline = *line;
	transform(gramline.begin(), gramline.end(), gramline.begin(), ::tolower);
	istringstream iss(gramline);
    string word;
    unordered_set<string> :: iterator reject;
    int strindex = 0;
    while(iss >> word) {
    	reject = reject_words.find(word);
    	if(reject != reject_words.end()) {
    		continue;
    	}

    	else {
    		if(word.at(0) == '{') {
    			vector<string> :: iterator overconcepts;
    			for(overconcepts = concepts[word].begin(); overconcepts != concepts[word].end(); ++overconcepts) {
    				bool flag = replace(input_str, *overconcepts, word);
    				if(flag) {
    					strindex = input_str.find(word);
    					strindex += word.length() - 1;
    					break;
    				}
    			}
    		}
    	}
    }

    return input_str;
}

void make_open_phrases() {
	ifstream stream;
	char * file = new char[string("PlaceHolder/PlaceHolderDetail.txt").length() + 1];
	strcpy(file, string("PlaceHolder/PlaceHolderDetail.txt").c_str());
	stream.open(file);
	char temp[500];
	while(stream) {
		stream.getline(temp, 500);
		if(stream) {
			string line = string(temp);
			if(KMP(string("openphrase"), line)) {
				istringstream iss(line);
   				string word;
   				iss >> word;
   				open_phrase_names.insert(word);
			}
		}
	}
}

void return_open_phrases(string input_str, int indexofline, string gram) {
	unordered_set <string> :: iterator us;
	vector<string> :: iterator vs = grammar_lines[gram].begin() + indexofline;
	string gramline = *vs;
	vector<string> words_in_grammar;
	istringstream iss(gramline);
	string word;
	while(iss >> word) {
		words_in_grammar.push_back(word);
	}

	for(us = open_phrase_names.begin(); us != open_phrase_names.end(); ++us) {
		if(KMP(*us, gramline)) {
			string before = "", after = "";
			vector<string> :: iterator iter = find(words_in_grammar.begin(), words_in_grammar.end(), *us);
			if(iter == words_in_grammar.begin() + 1) {
				before = *(iter - 1);
				before = string(" ") + before + string(" ");
				after = *(iter + 1) + string(" ") + *(iter + 2);
				after = string(" ") + after + string(" ");
				transform(before.begin(), before.end(), before.begin(), ::tolower);
				transform(after.begin(), after.end(), after.begin(), ::tolower);
			}

			else if(iter == words_in_grammar.end() - 2) {
				after = *(iter + 1);
				after = string(" ") + after + string(" ");
				before = *(iter - 2) + string(" ") + *(iter - 1);
				before = string(" ") + before + string(" ");
				transform(before.begin(), before.end(), before.begin(), ::tolower);
				transform(after.begin(), after.end(), after.begin(), ::tolower);
			}

			else if(iter - words_in_grammar.begin() > 0 && iter + 1 < words_in_grammar.end() && iter - words_in_grammar.begin() != 1) {
				before = *(iter - 2) + string(" ") + *(iter - 1);
				before = string(" ") + before + string(" ");
				after = *(iter + 1) + string(" ") + *(iter + 2);
				after = string(" ") + after + string(" ");
				transform(before.begin(), before.end(), before.begin(), ::tolower);
				transform(after.begin(), after.end(), after.begin(), ::tolower);
				// cout << "*iter = " << *iter << "\nbefore = " << before << "\nafter = " << after << endl;
			}

			else if(iter == words_in_grammar.begin()) {
				after = *(iter + 1);
				after = string(" ") + after + string(" ");
				transform(after.begin(), after.end(), after.begin(), ::tolower);
				// cout << "*iter = " << *iter << "\nafter = " << after << endl;
			}

			else if(iter == words_in_grammar.end() - 1) {
				before = *(iter - 1);
				before = string(" ") + before + string(" ");
				transform(before.begin(), before.end(), before.begin(), ::tolower);
				// cout << "*iter = " << *iter << "\nbefore = " << before << endl;
			}
		
			string open;
			if(before.length() > 0 && after.length() > 0) {

				int b_index = input_str.find(before);
				int a_index = input_str.find(after);
				// cout << "b_index = " << b_index << "\na_index = " << a_index << endl;
				open = input_str.substr(b_index + before.length(), a_index - (b_index + before.length()));
			}

			else if(before.length() == 0) {
				int a_index = input_str.find(after);
				open = input_str.substr(0, a_index - 1);
			}

			else if(after.length() == 0) {
				int b_index = input_str.find(before);
				open = input_str.substr(b_index + before.length(), input_str.length() - (b_index + before.length()));
			}

			placeholder_values[*iter].insert(open);
			replace(input_str, open, *us);
		}
	}
}

string return_placeholders(string gram, string input_str, vector<string> v_c) {
	input_str += " ";
	transform(input_str.begin(), input_str.end(), input_str.begin(), ::tolower);
	vector<pair<string, string> > replacement;
	map<string, vector<string> > :: iterator itr;

	for(itr = placeholder.begin(); itr != placeholder.end(); ++itr) {
		vector<string> :: iterator it;
		for(it = itr->second.begin(); it != itr->second.end(); ++it) {
			string pattern = *it;
			transform(pattern.begin(), pattern.end(), pattern.begin(), ::tolower);
			replace(input_str, pattern, itr->first);
		}
	}

	input_str = date_time_string(input_str);
	transform(input_str.begin(), input_str.end(), input_str.begin(), ::tolower);

	int maxscore = 0;
	int indexofline = 0;
	vector<string> :: iterator gramline;
	for(gramline = grammar_lines[gram].begin(); gramline != grammar_lines[gram].end(); ++gramline) {
		int temp = give_grammar_line_score(gram, input_str, gramline - grammar_lines[gram].begin());
		if(maxscore <= temp) {
			maxscore = temp;
			indexofline = gramline - grammar_lines[gram].begin();
		}
	}

	string temp = replace_concepts(input_str, indexofline, gram);
	return_open_phrases(temp, indexofline, gram);

	return input_str;
}

void make_grammar_words() {
	map<string, pair<set<string>, pair< set<string>, set<string> > > > :: iterator it;
	for(it = grammar.begin(); it != grammar.end(); ++it) {
		vector<string> v;
		string name = it->first;
		string topush;
		for(int i = 0; i < name.length(); ++i) {
			topush.push_back(name.at(i));
			if(i + 1 < name.length()) {
				if(isupper(name.at(i + 1))) {
					v.push_back(topush);
					topush.clear();
				}
			}
		}

		v.push_back(topush);
		grammar_words.insert(make_pair(name, v));
	}
}

bool is_equal(string s1, string s2){
	transform(s1.begin(), s1.end(), s1.begin(), ::tolower);		
	transform(s2.begin(), s2.end(), s2.begin(), ::tolower);
	return s1 == s2;
}

void print_vec(vector<string> a) {
	for (size_t i = 0; i < a.size(); ++i) {
		cout << a[i] << endl;
	}
	cout << endl;
}

void make_placeholder() {
	vector<string> filenames;
	DIR *dir;
	struct dirent *ent;
	if ((dir = opendir ("PlaceHolder")) != NULL) {
	  	while ((ent = readdir (dir)) != NULL) {
	    	filenames.push_back(ent->d_name);
	  	}	
	  	closedir (dir);
	} else {
	  	perror ("");
	  	return;
	}

	vector<string>::iterator it_v;
	for (it_v = filenames.begin(); it_v != filenames.end(); ++it_v) {
		if (*it_v == "PlaceHolderDetail.txt") {
			filenames.erase(it_v);
			break;
		}
	}

	for (size_t i = 2; i < filenames.size(); ++i) {
		ifstream stream;
		char * file = new char[("PlaceHolder/" + filenames[i]).length() + 1];
		strcpy(file, ("PlaceHolder/" + filenames[i]).c_str());
		stream.open(file);
		char temp[250];
		vector <string> v;
		while(stream) {
			stream.getline(temp, 250);
			if (stream) {
				v.push_back(temp);
				if(temp == string("bengaluru")) v.push_back("bangalore");
			}
		}

		if(filenames[i] == string("places.txt")) {
			v.push_back("london");
			v.push_back("new york");
		}

		if(filenames[i] == string("contact_name.txt")) {
			v.push_back("romi taj");
		}

		stream.close();
		if (1) {
			stream.open(file);
			string s;
			while(stream >> s) {
				v.push_back(s);
			}
			stream.close();
		}

		filenames[i] = filenames[i].substr(0, filenames[i].length() - 4);
		filenames[i] = '<' + filenames[i] + '>';
		if(filenames[i] == "<places>") placeholder.insert(make_pair("<place>", v));
		placeholder.insert(make_pair(filenames[i], v));
	}
}

void make_grammar_lines() {
	vector<string> filenames;
	DIR *dir;
	struct dirent *ent;
	if ((dir = opendir ("Grammar")) != NULL) {
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
		char * file = new char[("Grammar/" + filenames[i]).length() + 1];
		strcpy(file, ("Grammar/" + filenames[i]).c_str());
		stream.open(file);
		char temp[250];
		vector <string> v;
		while(stream) {
			stream.getline(temp, 250);
			if (stream) {
				v.push_back(temp);
			}
		}

		stream.close();
		filenames[i] = filenames[i].substr(0, filenames[i].length() - 4);
		grammar_lines.insert(make_pair(filenames[i], v));
	}
}

void make_grammar() {
	vector<string> filenames;
	DIR *dir;
	struct dirent *ent;
	if ((dir = opendir ("Grammar")) != NULL) {
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
		char * file = new char[("Grammar/" + filenames[i]).length() + 1];
		strcpy(file, ("Grammar/" + filenames[i]).c_str());
		stream.open(file);
		char temp[250];
		set <string> v_c, v_p, v_key;
		
		string s;
		while(stream >> s) {
			if(s.at(0) == '{') {
				string toput;
				for(int i = 0; i < s.length(); ++i) {
					toput.push_back(s.at(i));
					if(s.at(i) == '}') {
						v_c.insert(toput);
						toput.clear();
					}
				}
			}

			else if(s.at(0) == '<') {
				string toput;
				for(int i = 0; i < s.length(); ++i) {
					toput.push_back(s.at(i));
					if(s.at(i) == '>') {
						if(toput == "<time>") toput = "<dateTime>";
						v_p.insert(toput);
						toput.clear();
					}
				}
			}

			else {
				transform(s.begin(), s.end(), s.begin(), ::tolower);
				unordered_set<string> :: iterator itr1;
				itr1 = reject_words.find(s);
				if(itr1 == reject_words.end() && s.length() > 2) v_key.insert(s);
			}
		}
		
		stream.close();
		filenames[i] = filenames[i].substr(0, filenames[i].length() - 4);
		grammar.insert(make_pair(filenames[i], make_pair(v_c, make_pair(v_p, v_key))));
	}
}

void make_concept() {
	vector<string> filenames;
	DIR *dir;
	struct dirent *ent;
	if ((dir = opendir ("Concept")) != NULL) {
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
		char * file = new char[("Concept/" + filenames[i]).length() + 1];
		std::strcpy(file, ("Concept/" + filenames[i]).c_str());
		stream.open(file);
		char temp[250];
		vector <string> v;
		while(stream) {
			stream.getline(temp, 250);
			if (stream) v.push_back(temp);
		}

		stream.close();
		filenames[i] = filenames[i].substr(0, filenames[i].length() - 4);
		filenames[i] = '{' + filenames[i] + '}';
		concepts.insert(make_pair(filenames[i], v));
	}
}

void make_custom_resource() {
	vector<string> filenames;
	DIR *dir;
	struct dirent *ent;
	if ((dir = opendir ("CustomResource")) != NULL) {
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
		char * file = new char[("CustomResource/" + filenames[i]).length() + 1];
		std::strcpy(file, ("CustomResource/" + filenames[i]).c_str());
		stream.open(file);
		char temp[250];
		while(stream) {
			stream.getline(temp, 250);
			if (stream) reject_words.insert(temp);
		}
		stream.close();
	}
}

void make_conjunctions() {
	vector<string> filenames;
	DIR *dir;
	struct dirent *ent;
	if ((dir = opendir ("Conjunctions")) != NULL) {
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
		char * file = new char[("Conjunctions/" + filenames[i]).length() + 1];
		std::strcpy(file, ("Conjunctions/" + filenames[i]).c_str());
		stream.open(file);
		char temp[250];
		while(stream) {
			stream.getline(temp, 250);
			if (stream) conjunctions.insert(temp);
		}
		stream.close();
	}
}

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
 
bool KMP(string pattern, string target) {
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
                return 1;
        }

        else
            k = f[k];
    }

    return 0;
}

bool is_concept_present(vector<string> v, string s) {
	bool flag = false; 
	vector<string>::iterator it1;
	for(it1 = v.begin(); it1 != v.end(); ++it1) {
		flag = KMP(*it1, s);
		if(flag) break;
	}

	return flag;
}

vector<string> concepts_that_are_present(string s) {
	map<string, vector<string> >::iterator it;
	vector<string> res;
	for(it = concepts.begin(); it != concepts.end(); ++it) {
		if(is_concept_present(it->second, s)) { 
			if(it->first == "{search_concept}") {
				res.push_back("{search}");
			}

			res.push_back(it->first);
		}
	}

	return res;
}

bool is_placeholder_present(vector<string> v, string s) {
	bool flag = false; 
	vector<string>::iterator it1;
	for(it1 = v.begin(); it1 != v.end(); ++it1) {
		flag = KMP(*it1, s);
		if(flag) break;
	}

	return flag;
}

vector<string> placeholders_that_are_present(string s) {
	map<string, vector<string> >::iterator it;
	vector<string> res;
	for(it = placeholder.begin(); it != placeholder.end(); ++it) {
		if(is_placeholder_present(it->second, s)) {
			res.push_back(it->first);
		}
	}

	return res;
}

void make_counters() {
	map<string, pair<set<string>, pair< set<string>, set<string> > > >::iterator it;
	for(it = grammar.begin(); it != grammar.end();  ++it) {
		counters.insert(make_pair(it->first, 0));
	}
}

void find_grammar_scores_concept(vector<string> v) {
	map<string, pair<set<string>, pair< set<string>, set<string> > > >::iterator it;
	for(size_t i = 0; i < v.size(); ++i) {
		for(it = grammar.begin(); it != grammar.end(); ++it) {
			set<string>::iterator it_s;
			it_s = it->second.first.find(v[i]);
			if(it_s != it->second.first.end()) {
				// cout << it->first << " " << v[i] << endl;
				counters[it->first] = counters[it->first] + 1;
			}	
		}
	}
}

void find_grammar_scores_placeholder(vector<string> v) {
	map<string, pair<set<string>, pair< set<string>, set<string> > > >::iterator it;
	for(size_t i = 0; i < v.size(); ++i) {
		for(it = grammar.begin(); it != grammar.end(); ++it) {
			set<string>::iterator it_s;
			it_s = it->second.second.first.find(v[i]);
			if(it_s != it->second.second.first.end())
				counters[it->first] = counters[it->first] + 1;
		}
	}
}

void find_grammar_scores_keywords(string input_str) {
	map<string, pair<set<string>, pair< set<string>, set<string> > > >::iterator it;
	for(it = grammar.begin(); it != grammar.end(); ++it) {
		set<string> keys = it->second.second.second;
		set<string>::iterator it_s;
		bool flag = false, flag1 = false;
		for(it_s = keys.begin(); it_s != keys.end(); ++it_s) {
			flag = KMP(*it_s, input_str);
			vector<string> :: iterator it_v;
			vector<string> v_g = grammar_words[it->first];
			for(it_v = v_g.begin(); it_v!=v_g.end(); it_v++) {
				flag1 = is_equal(*it_v, *it_s);
				if(flag1) break;
			}
			
			if(flag) {
				if(!flag1) 
					counters[it->first]++;
				else
					counters[it->first] += 2;  }
		}	
	}
}

pair<string, string> split(string str, string conjunction) {
	string s1, s2;
	transform(str.begin(), str.end(), str.begin(), ::tolower);
	transform(conjunction.begin(), conjunction.end(), conjunction.begin(), ::tolower);
	int index = str.find(conjunction);
	s1 = str.substr(0, index);
	s2 = str.substr(index + conjunction.length(), str.length());
	return make_pair(s1, s2);
}

string command(string input_str) {
	int max = -1;
	map<string, int>::iterator it;
	string s;
	for(it = counters.begin(); it != counters.end(); ++it) {
		if(max < it->second) {
			s = it->first;
			max = it->second;
		}
	}

	if((s == "DateTimeNow" || s == "DateTimeInPlace") && (counters["DateTimeNow"] >= counters["DateTimeInPlace"])) {
		s = "DateTimeNow";
		vector<string> :: iterator itr;
		bool flag = false;
		for(itr = placeholder["<places>"].begin(); itr != placeholder["<places>"].end(); ++itr) {
			flag = KMP(*itr, input_str);
			if(flag) {
				s = "DateTimeInPlace";
				break;
			}
		}
	}

	if((s == "CreateEventWithTime" || s == "CreateEventWithTimeAndPlace") && (counters["CreateEventWithTime"] >= counters["CreateEventWithTimeAndPlace"])) {
		s = "CreateEventWithTime";
		vector<string> :: iterator itr;
		bool flag = false;
		for(itr = placeholder["<places>"].begin(); itr != placeholder["<places>"].end(); ++itr) {
			flag = KMP(*itr, input_str);
			if(flag) {
				s = "CreateEventWithTimeAndPlace";
				break;
			}
		}
	}

	return s;
}

string give_command(string input_str) {
	counters.clear();
	make_counters();
	vector<string> v_c, v_p;
	v_c = concepts_that_are_present(input_str);
	v_p = placeholders_that_are_present(input_str);
	string date_time_modified = date_time_string(input_str);
	if(placeholder_values.find("<dateTime>") != placeholder_values.end()) v_p.push_back(string("<dateTime>"));

	find_grammar_scores_concept(v_c);
	find_grammar_scores_placeholder(v_p);
	find_grammar_scores_keywords(input_str);
	string ans = command(input_str);
	string input_str_modified = return_placeholders(ans, input_str, v_c);

	return ans;
}

int main() {
	make_conjunctions();
	make_concept();
	make_placeholder();
	make_custom_resource();
	make_grammar();
	make_grammar_words(); 
	make_grammar_lines();
	make_open_phrases();
	
	vector<string> questions;
	vector<string> answers;
	ifstream infile("Testing/input.txt");
	ofstream outfile("Testing/output1.txt", ios::out);
	string linebuffer;

	while (infile && getline(infile, linebuffer)) {
		if (linebuffer.length() == 0) continue;

		string temp5 = linebuffer.substr(0, 5);
		string temp6 = linebuffer.substr(0, 6);
		transform(temp5.begin(), temp5.end(), temp5.begin(), ::tolower);
		transform(temp6.begin(), temp6.end(), temp6.begin(), ::tolower);

		if(temp6 != "case #" && temp5 != "case#")
			questions.push_back(linebuffer);
	}

	questions.erase(questions.begin());
	int counter = 1;
	vector<string> :: iterator w;
	for(w = questions.begin(); w != questions.end(); ++w) {
		o_clock = false;
		placeholder_values.clear();
		string input_str = *w;
		input_str += string(" ");
		string answer = give_command(input_str);
		outfile << "Case#" << counter++ << ":" << "\n";
		outfile << answer << "\n";
		map<string, set<string> > :: iterator mo;
		for(mo = placeholder_values.begin(); mo != placeholder_values.end(); ++mo) {
			set<string> :: iterator ss;
			for(ss = (mo->second).begin(); ss != (mo->second).end(); ++ss) {
				outfile << mo->first << " : " << "\"";
				outfile << *ss << "\"\n";
			}

		}

	}
	
	return 0;
}