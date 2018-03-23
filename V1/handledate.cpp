#include <bits/stdc++.h>
#include <dirent.h>

using namespace std;

vector<string> months;
vector<string> days;
vector<string> number_clock;

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
		if (filenames[i] == "number.txt") {
			char * file = new char[("DateTimeFormats/" + filenames[i]).length() + 1];
			std::strcpy(file, ("DateTimeFormats/" + filenames[i]).c_str());
			stream.open(file);
			char temp[250];
			while(stream) {
				stream.getline(temp, 250);
				if (stream) number_clock.push_back(temp);
			}
			stream.close();
		}
	}
}

bool is_day_present(string s) {
	vector<string>::iterator it;
	for (it = days.begin(); it != days.end(); ++it) {
		int flag = KMP(*it, s);
		if (flag != -1) cout << "Mil gaya: " << *it << " idhar mila" << flag << endl;
	}
}

// void integrate(string input) {

// }

int main() {
	make_day_date_time_vectors();
	string s = "aaj ko english me today bolte hain";
	is_day_present(s);
	// vector<string>::iterator it;
	// for (it = months.begin(); it != months.end(); ++it) {
	// 	cout << *it << endl;
	// }
	// for (it = days.begin(); it != days.end(); ++it) {
	// 	cout << *it << endl;
	// }
	// for (it = number_clock.begin(); it != number_clock.end(); ++it) {
	// 	cout << *it << endl;
	// }
}