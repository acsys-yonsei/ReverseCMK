#include <iostream>
#include <fstream>
#include <sstream>
#include <climits>
#include <queue>
#include <vector>
#include <utility>
#include <algorithm>
#include <string>

using namespace std;

/* text format
*  n
*  first row
*  second row
*  ....
*  final row
*/

int* degree;
bool* visit;
int* r;
int n;
int m;
vector<vector<int>> edge;
vector<int> node;
queue<int> q;

void ReverseCMK();

int main(int argc, char** argv) {
	ifstream openFile(argv[1]);
	if (openFile.is_open()) {
		string line;
		getline(openFile, line);
		n = stoi(line);
		getline(openFile, line);
		m = stoi(line);

		//dynamic allocation
		degree = new int[n];
		visit = new bool[n];
		for (int i = 0; i < n; i++) {
			degree[i] = 0;
			visit[i] = false;
		}
		r = new int[n];
		int idx = 0;
		int vertex = -1;
		vector<int> *list;
		bool isExist = false;
		for (int i = 0; i < m; i++) {
			getline(openFile, line);
			stringstream ss(line);
			string tmp;
			bool first = true;
			while (getline(ss, tmp, ' ')) {
				if (first) {
					if (vertex != stoi(tmp)) {
						if (vertex != -1)
							edge.push_back(*list);
						vertex = stoi(tmp);
						int idx = 0;
						for (int j = 0; j <node.size(); j++) {
							if (node[j] == vertex) {
								isExist = true;
								idx = j;
								break;
							}
						}
						if (isExist)
							list = &edge[idx];
						else {
							node.push_back(vertex);
							list = new vector<int>();
						}
					}
					first = false;
				}
				else {
					list->push_back(stoi(tmp));
					degree[vertex]++;
				}
			}
		}
		if (!isExist)
			edge.push_back(*list);
		//run Reverse Cuthill-Mckee
		
		
		ReverseCMK();
		vector<vector<int>> cmk_edge;
		vector<pair<int, int>> cmk_node;
		int index_1 = INT_MAX, index_2;
		for (int i = 0; i < edge.size(); i++) {
			vector<int> tmp;
			for (int j = n-1; j >= 0; j--) {
				if (node[i] == r[j]) {
					cmk_node.push_back(make_pair(n-1 - j, i));
					break;
				}
			}
			for (int j = 0; j < edge[i].size(); j++) {
				for (int k = n-1; k >= 0; k--) {
					if (edge[i][j] == r[k]) {
						tmp.push_back(n-1 - k); 
						break;
					}
				}
			}
			sort(tmp.begin(), tmp.end());
			cmk_edge.push_back(tmp);
		}
		edge.clear();
		sort(cmk_node.begin(), cmk_node.end());
		for (int i = 0; i < cmk_node.size(); i++) {
			edge.push_back(cmk_edge[cmk_node[i].second]);
		}
		cmk_edge.clear();
		//make output file
		ofstream output("output.txt");
		output<<n<<endl;
		output<<m<<endl;
		for (int i = 0; i < edge.size(); i++) {
			for (int j = 0; j < edge[i].size(); j++) {
				output<<cmk_node[i].first<<" "<<edge[i][j]<<endl;
			}
		}
		output.close();
		
	}
	else {
		throw invalid_argument("Cannot open datafile.");
	}
}

void ReverseCMK() {
	
	int idx = 0;

	//repeat until all of node is in r
	while (idx < n) {
		//find minimun_degree
		int min_deg = INT_MAX;
		int min_row = 0;
		for (int i = 0; i < n; i++) {
			if (visit[i])
				continue;
			if (degree[i] < min_deg) {
				min_row = i;
				min_deg = degree[i];
			}
		}

		//mark the visit flag and add to r
		visit[min_row] = true;
		r[idx] = min_row;
		idx++;
		vector<pair<int, int>> v;
		int check;
		bool notzero = false;

		//add the adjacency node in ascending order of degree
		for (int i = 0; i < node.size(); i++) {
			if (node[i] == min_row) {
				check = i;
				notzero = true;
				break;
			}
		}
		if (notzero)
		{
			for (int i = 0; i < edge[check].size(); i++) {
				v.push_back(make_pair(degree[edge[check][i]],edge[check][i]));
			}
			sort(v.begin(), v.end());
			while (!v.empty()) {
				q.push(v.front().second);
				v.erase(v.begin());
			}
			//repeat until q is empty
			while (!q.empty()) 
			{
				notzero = false;
				int c = q.front();
				q.pop();
				bool in_r = false;
				for (int i = 0; i < idx; i++) {
					if (r[i] == c) {
						in_r = true;
						break;
					}
				}
				if (!in_r) {
					visit[c] = true;
					r[idx] = c;
					idx++;
					for (int i = 0; i < node.size(); i++) {
						if (node[i] == c) {
							check = i;
							notzero = true;
							break;
						}
					}
					if (notzero) {
						for (int i = 0; i < edge[check].size(); i++) {
							v.push_back(make_pair(degree[edge[check][i]],edge[check][i]));
						}
						sort(v.begin(), v.end());
						while (!v.empty()) {
							q.push(v.front().second);
							v.erase(v.begin());
						}
					}
				}
			}
		}
	}
}


