#include <bits/stdc++.h>
#include <dirent.h>

using namespace std;

unordered_set<string> months;
unordered_set<string> days;
unordered_set<string> number_clock;

void make_months() {
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
				if (stream) months.insert(temp);
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
				if (stream) days.insert(temp);
			}
			stream.close();
		}
	}
}

int main() {
	make_months();
	unordered_set<string>::iterator it;
	for (it = months.begin(); it != months.end(); ++it) {
		cout << *it << endl;
	}
	for (it = days.begin(); it != days.end(); ++it) {
		cout << *it << endl;
	}
}