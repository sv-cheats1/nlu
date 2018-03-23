#include <bits/stdc++.h>
#include <dirent.h>

using namespace std;

map<string, vector<string> > concepts;
map<string, vector<string> > placeholder;
map<string, pair<set<string>, pair< set<string>, set<string> > > > grammar;
unordered_set<string> reject_words;
unordered_set<string> conjunctions;
map<string, int> counters;

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

		stream.close();
		// if (filenames[i] == "contact_name.txt") {
		if(1) {
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

	vector<string> v_s = {"a.m.", " am ", "p.m.", " pm "};
	placeholder.insert(make_pair("<dateTime>", v_s));
	v_s.clear();

	v_s = {"monday", "tuesday", "wednesday", "thursday", "friday", "saturday", "sunday", "today", "tomorrow"};
	placeholder.insert(make_pair("<day>", v_s));
	v_s.clear();

	v_s = {"january", "february", "march", "april", "may", "june", "july", "august", "september", "october", "november", "december"};
	placeholder.insert(make_pair("<date>", v_s));
	v_s.clear();
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
			if(it_s != it->second.second.first.end()) {
				counters[it->first] = counters[it->first] + 1;
			}	
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
			flag1 = KMP(*it_s, it->first);
			if(flag) {
				if(!flag1) counters[it->first]++;
				else counters[it->first] += 2;
			}
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

	if(s == "DateTimeNow") {
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
	return s;
}

string give_command(string input_str) {
	counters.clear();
	make_counters();
	vector<string> v_c, v_p;
	v_c = concepts_that_are_present(input_str);
	v_p = placeholders_that_are_present(input_str);

	find_grammar_scores_concept(v_c);
	find_grammar_scores_placeholder(v_p);
	find_grammar_scores_keywords(input_str);

	return command(input_str);
}

int main() {
	make_conjunctions();
	make_concept();
	make_placeholder();
	make_custom_resource();
	make_grammar();
	
	vector<string> questions;
	vector<string> answers;

	ifstream infile("Testing/input.txt");
	ofstream outfile("Testing/output.txt", ios::out);
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

	int counter = 1;
	vector<string> :: iterator myitr;
	for(myitr = questions.begin(); myitr != questions.end(); myitr++) {
		answers.clear();
		string input_str = *myitr;
		pair<string, string> new_input;
		string conjunction;
		bool whattosend = false;
		unordered_set<string> :: iterator itre;
		for(itre = conjunctions.begin(); itre != conjunctions.end(); ++itre) {
			if(KMP(*itre, input_str)) {
				whattosend = true;
				conjunction = *itre;
				new_input = split(input_str, conjunction);
			}
		}
		
		if(!whattosend) 
			answers.push_back(give_command(input_str));

		else {
			vector<string> v_c;
			v_c = concepts_that_are_present(new_input.first);
			if(v_c.size())
				answers.push_back(give_command(new_input.first));

			v_c.clear();
			v_c = concepts_that_are_present(new_input.second);
			if(v_c.size())
				answers.push_back(give_command(new_input.second));
		}
		
		vector<string> :: iterator final;
		outfile << "Case#" << counter++ << ":\n";
		for(final = answers.begin(); final != answers.end(); final++) 
			outfile << *final << "\n";
	}

	return 0;
}