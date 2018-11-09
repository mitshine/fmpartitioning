#include <iostream>
#include <vector>
#include <chrono>  // for high_resolution_clock
#include <algorithm>
#include <string>
#include <map>
#include <list>
#include <fstream>
#include <sstream>

#define LOW_AREA (float)45
#define HIGH_AREA (float)55
#define AREA_FILE "nodes.area"
#define OUTPUT_FILE "TEAM20_OUTPUT_IBM.txt"

std::ofstream out (OUTPUT_FILE);

using namespace std;  

vector<string> split(const string &s, char delim) {
    stringstream ss(s);
    string item;
    vector<string> tokens;
    while (getline(ss, item, delim)) {
		item.erase( remove( item.begin(), item.end(), '\t' ), item.end() );
		tokens.push_back(item);
    }
    return tokens;
}

pair <std::map<std::string, std::string>, int> storenodes (string input, int counter, std::map<std::string, std::string> myMap)
{
  string mystr;
  vector<string> tokens;
  
  std::string s = input;
  std::string delimiter = " ";

  list<string>::iterator it;
  
  tokens = split(s, 0x20);
  myMap[tokens[0]] = tokens[1];

  std::pair <std::map<std::string, std::string>, int> foo;

  foo = std::make_pair (myMap,counter);
  
  return foo;
}

pair <std::map<std::string, std::string>, int> storenodesarea (vector<string> lines_tokenize1, std::map<std::string, std::string> orignodesarea, string input, int counter, std::map<std::string, std::string> myMap, std::map<std::string, std::string> myMaparea)
{
  string mystr;
  vector<string> tokens;
  
  std::string s = input;
  std::string delimiter = " ";

  list<string>::iterator it;
  int counternodes = 0;
  
  for(auto& s : lines_tokenize1) {
  tokens = split(s, 0x20);
  tokens[0].erase( remove( tokens[0].begin(), tokens[0].end(), ' ' ), tokens[0].end() );
  tokens[1].erase( remove( tokens[1].begin(), tokens[1].end(), ' ' ), tokens[1].end() );
  myMap[tokens[0]] = tokens[1];

  counternodes = counternodes + 1;
  }

  for (auto& z: myMap) {
    for (auto& x: myMaparea) {
	  std::string str0, str1, str2, str3;
	  str0 = z.first;
	  str1 = z.second;
	  str2 = x.first;
	  str3 = x.second;
	  if (str1.find(str2) != std::string::npos) {
		orignodesarea[str0] = str3;
	  }
    }
  }
  
  counter = counter + 1;

  std::pair <std::map<std::string, std::string>, int> foo;

  foo = std::make_pair (orignodesarea, counter);
  
  return foo;
}

std::tuple<std::map<std::string, std::list<string>>, string, int> storetuple (vector<string> lines_tokenize, std::map<std::string, std::list<string>> netNodesList, string pnetName, int &totalnodes, string input, int &counter, std::map<std::string, std::string> myMaparea, std::map<std::string, int> myMapnetarea, int area)
{
  string mystr;
  vector<string> tokens;
  string arraylist[6] = {"net ","net_","inst_","I","O","endnet"};
  
  std::string delimiter = " ";
  size_t pos = 0;

  list<string>::iterator it;
  list<string>::iterator it2;
  
	std::string s;
  for(int i=0;i<lines_tokenize.size();i++) {
	  s = lines_tokenize[i];
  if (s.find(arraylist[0]) != std::string::npos) {
	tokens = split(s, 0x20);
	totalnodes = totalnodes + stoi(tokens[2]);
	pnetName = tokens[1];
  }
  
  if (s.find(arraylist[2]) != std::string::npos) {
	tokens = split(s, 0x20);
	netNodesList[pnetName].push_back(tokens[0]);
    if (s.find(arraylist[3]) != std::string::npos) {
		//out << "Input: " << tokens[1] << endl;
	}
	else {
		//out << "Output: " << tokens[1] << endl;
	}
  }

  if (s.find(arraylist[5]) != std::string::npos) {
		myMapnetarea[pnetName] = area;
		counter = counter + 1;
		area = 0;
  }
  }
  
  std::tuple<std::map<std::string, std::list<string>>, string, int> foo;

  foo = std::make_tuple (netNodesList,pnetName,area);
  
  return foo;
}

int secondLargest(int a[], int size) {
  int currentMax = a[0];
  int secondMax=0;
  for( int i = 0; i< size; i++) {
    if (a[i] >secondMax)
      secondMax = a[i]; 

    if (a[i]>currentMax){
      secondMax=currentMax;
      currentMax=a[i];
    }
  }
return secondMax;
}

std::tuple<std::map<std::string, int>, std::map<std::string, int>, std::string> shift_and_find_cutset(std::map<std::string, int> maxgainaftershiftnode, string shiftednode, int &counter, std::map<std::string, std::list<string>> netNodesList, int &maxcomputegains, float &percentpartition1, float &percentpartition2, int &cutset, int &addswappedarea, std::map<std::string, int> mypartition1, std::map<std::string, int> mypartition2) {
	
	    string swapnode1;
		string swapnode2;
		std::map<int, std::string> mapswapnode1;
		std::map<int, std::string> mapswapnode2;
		list<string> mappartition1;
		list<string> mappartition2;
		std::map<std::string, int> mapareanode1all;
		std::map<std::string, int> mapareanode2all;
		std::map<std::string, int> mapareanode1;
		std::map<std::string, int> mapareanode2;
		
		float partitionarea1 = 0;
		float partitionarea2 = 0;
		percentpartition1 = 0;
		percentpartition2 = 0;
		
		std::list<std::string> norepeat;
		std::list<std::string>::iterator pos2;
		
		  string netName;
		  
		  int count = 0;
		  int row_cutset_if_swapped = 0;
		  int current_row_cutset = 0;
		  int totalcutsetpernode = 0;
		  int current_iteration_cutset = 0;
		  std::map<std::string, int> computegain1;
		  std::map<std::string, int> computegain2;
		  
		  std::list<string>::iterator pos;
		  std::list<string>::iterator pos1;
		  
		  auto second_hurdle = std::chrono::high_resolution_clock::now();
		  
			  for(auto& pair : netNodesList) {
				netName = pair.first;
				int countpartition1 = 0;
				int countpartition2 = 0;
				int current_cutset = 0;
				int row_cutset_if_swapped = 0;
				int row_cutset_if_swapped1 = 0;
				int row_cutset_if_swapped2 = 0;
				//out << "NETNAME UNDER CONSIDERATION: " << netName << endl;
				for (std::list<string>::iterator it=(pair.second).begin(); it != (pair.second).end(); ++it) {
					//std::out << ' ' << *it << endl;
					if(mypartition1.count(*it) != 0) { countpartition1 = countpartition1 + 1; }
					if(mypartition2.count(*it) != 0) { countpartition2 = countpartition2 + 1; }
				}
				for (std::list<string>::iterator it=(pair.second).begin(); it != (pair.second).end(); ++it) {
					//std::out << ' ' << *it << endl;
					if(countpartition1 > 0 && countpartition2 == 0) { current_cutset = 0; if(computegain1.count(*it) != 0) {
						if(maxgainaftershiftnode.count(*it) > 0) {
							computegain1.insert({*it, -100});
						}
						else {
							computegain1[*it] = computegain1[*it] - 1;
						}
						}
						else {computegain1.insert({*it, -1}); }
					}
					if(countpartition1 == 0 && countpartition2 > 0) { current_cutset = 0; if(computegain2.count(*it) != 0) { 
						if(maxgainaftershiftnode.count(*it) > 0) {
							computegain2.insert({*it, -100});
						}
						else {
							computegain2[*it] = computegain2[*it] - 1;
						}
						}
						else {computegain2.insert({*it, -1}); }
					}
					if(countpartition1 == 1 && countpartition2 == 1) { current_cutset = 1; 
						if(mypartition1.count(*it) != 0) { if(computegain1.count(*it) != 0) { 
							if(maxgainaftershiftnode.count(*it) > 0) {
								computegain1.insert({*it, -100});
							}
							else { 
								computegain1[*it] = computegain1[*it] + 1;
							}
							}
							else {computegain1.insert({*it, 1}); }
						}
						if(mypartition2.count(*it) != 0) { if(computegain2.count(*it) != 0) { 
							if(maxgainaftershiftnode.count(*it) > 0) {
								computegain2.insert({*it, -100});
							}
							else {
								computegain2[*it] = computegain2[*it] + 1;
							}
							}
							else {computegain2.insert({*it, 1}); }
						}
					}
					if(countpartition1 == 1 && countpartition2 > 1) { current_cutset = 1; 
						if(mypartition1.count(*it) != 0) { if(computegain1.count(*it) != 0) { 
							if(maxgainaftershiftnode.count(*it) > 0) {
								computegain1.insert({*it, -100});
							}
							else {
								computegain1[*it] = computegain1[*it] + 1;
							}
							}
							else {computegain1.insert({*it, 1}); }
						}
						if(mypartition2.count(*it) != 0) { if(computegain2.count(*it) != 0) { 
							if(maxgainaftershiftnode.count(*it) > 0) {
								computegain2.insert({*it, -100});
							}
							else {
								computegain2[*it] = computegain2[*it] + 0;
							}
							}
							else {computegain2.insert({*it, 0}); }
						}
					}
					if(countpartition1 > 1 && countpartition2 == 1) { current_cutset = 1; 
						if(mypartition1.count(*it) != 0) { if(computegain1.count(*it) != 0) { 
							if(maxgainaftershiftnode.count(*it) > 0) {
								computegain1.insert({*it, -100});
							}
							else {
								computegain1[*it] = computegain1[*it] + 0;
							}
							}
							else {computegain1.insert({*it, 0}); }
						}
						if(mypartition2.count(*it) != 0) { if(computegain2.count(*it) != 0) { 
							if(maxgainaftershiftnode.count(*it) > 0) {
								computegain2.insert({*it, -100});
							}
							else {
								computegain2[*it] = computegain2[*it] + 1;
							}
							}
							else {computegain2.insert({*it, 1}); }
						}
					}
					if(countpartition1 > 1 && countpartition2 > 1) { current_cutset = 1; 
						if(mypartition1.count(*it) != 0) { if(computegain1.count(*it) != 0) { 
							if(maxgainaftershiftnode.count(*it) > 0) {
								computegain1.insert({*it, -100});
							}
							else {
								computegain1[*it] = computegain1[*it] + 0;
							}
							}
							else {computegain1.insert({*it, 0}); }
						}
						if(mypartition2.count(*it) != 0) { if(computegain2.count(*it) != 0) { 
							if(maxgainaftershiftnode.count(*it) > 0) {
								computegain2.insert({*it, -100});
							}
							else {
								computegain2[*it] = computegain2[*it] + 0;
							}
							}
							else {computegain2.insert({*it, 0}); }
						}
					}
				}
				current_iteration_cutset = current_iteration_cutset + current_cutset;
			  }
						  
				auto third_hurdle = std::chrono::high_resolution_clock::now();

				// Effective simulation time
				std::chrono::duration<double> elapsed_third_hurdle = third_hurdle - second_hurdle;
		
				out << "=================" << endl;
				out << "Simulation time @ " << "loop end: " << elapsed_third_hurdle.count() << " seconds\n";
				out << "=================" << endl;
		  
		  out << "Cutset before shifting: " << current_iteration_cutset << endl;
		  
		  int totalarea = 0;
		  partitionarea1 = 0;
		  partitionarea2 = 0;
		  
		  auto max1 = std::max_element(computegain1.begin(), computegain1.end(), [](const pair<string, int>& p1, const pair<string, int>& p2) {return p1.second < p2.second; });
		  auto max2 = std::max_element(computegain2.begin(), computegain2.end(), [](const pair<string, int>& p1, const pair<string, int>& p2) {return p1.second < p2.second; });
		  
		  out << max1->first << "<=>" << max1->second << endl;
		  out << max2->first << "<=>" << max2->second << endl;
		  //cin.get();
		  
		  string node1 = max1->first;
		  string node2 = max2->first;
		  int value1 = max1->second;
		  int value2 = max2->second;
		  if(value1 > value2) {
			  mypartition2.insert({ node1, mypartition1.at(node1) });
			  shiftednode = node1;
			  maxcomputegains = computegain1[node1];
			  addswappedarea = addswappedarea + mypartition1[node1];
			  cutset = current_iteration_cutset - maxcomputegains;
			  mypartition1.erase(node1);
			  out << "The node " << node1 << " in Partition 1 will be shifted to Partition 2." << endl;
		  }
		  else {
			  mypartition1.insert({ node2, mypartition2.at(node2) });
			  shiftednode = node2;
			  maxcomputegains = computegain2[node2];
			  addswappedarea = addswappedarea + mypartition2[node2];
			  cutset = current_iteration_cutset - maxcomputegains;
			  mypartition2.erase(node2);
			  out << "The node " << node2 << " in Partition 2 will be shifted to Partition 1." << endl;
		  }
		  
		out << "Total Addswappedarea: " << addswappedarea << endl;
		
		for (const auto &p : mypartition1) {
          out << "mpartition1[" << p.first << "] = " << p.second << '\n';
		  partitionarea1 = partitionarea1 + p.second;
		}
		
		for (const auto &p : mypartition2) {
          out << "mpartition2[" << p.first << "] = " << p.second << '\n';
		  partitionarea2 = partitionarea2 + p.second;
		}
		
		partitionarea1 = partitionarea1;
		partitionarea2 = partitionarea2;
		totalarea = partitionarea1 + partitionarea2;
		
		out << "Total design area: " << totalarea << endl;
		out << "Total number of nets in the given design: " << counter << endl;
		out << "Total area in partition 1: " << partitionarea1 << endl;
		out << "Total area in partition 2: " << partitionarea2 << endl;
		
		percentpartition1 = (partitionarea1/totalarea)*100;
		percentpartition2 = (partitionarea2/totalarea)*100;
			
		if (percentpartition1 > LOW_AREA & percentpartition1 < HIGH_AREA) {
			out << "Partition 1 is between " << LOW_AREA << "%-" << HIGH_AREA << "%: " << percentpartition1 << endl;
			out << "Partition 2 is between " << LOW_AREA << "%-" << HIGH_AREA << "%: " << percentpartition2 << endl;
		}
		else {
			out << "\033[1;31mALERT: Partition 1 & Partition 2 are not in between " << LOW_AREA << "%-" << HIGH_AREA << "%\033[0m" << endl;
			out << "Partition 1 is between " << LOW_AREA << "%-" << HIGH_AREA << "%: " << percentpartition1 << endl;
			out << "Partition 2 is between " << LOW_AREA << "%-" << HIGH_AREA << "%: " << percentpartition2 << endl;
		}
		
		out << "\033[0;36mNew Cutset: " << cutset << "\033[0m" << endl;
	
	    std::tuple<std::map<std::string, int>, std::map<std::string, int>, std::string> foo;

        foo = std::make_tuple (mypartition1,mypartition2, shiftednode);
  
  return foo;
}

int main(int argc, char** argv)
{
	auto start = std::chrono::high_resolution_clock::now();
	
	for (int i = 1; i < argc; ++i) 
      cout << "ARG: " << argv[i] << "\n";
	
    string line;
	string linemap;
	string linemaparea;
	vector<string> tokens;
	vector<string> tokensdesignfiles;
	
	#define AUX_FILE argv[1]
	ifstream auxfile (AUX_FILE);
	string designline;
	int count = 0;
	string nodesin;
	string netsin;
	
	if (!auxfile) {
		cout << "Unable to open file";
		exit(1);
    }
	
	if (auxfile.is_open())
    {
		while ( getline (auxfile,designline) )
		{
			count = count + 1;
			if(count == 2) {
				tokensdesignfiles = split(designline, 0x20);
				nodesin = tokensdesignfiles[2];
				netsin = tokensdesignfiles[3];
			}
		}
		auxfile.close();
    }
    else cout << "Unable to open file";
	
	#define NODES_FILE nodesin.c_str()
	#define NETS_FILE netsin.c_str()
    ifstream netsfile (NETS_FILE);
	ifstream nodesfile (NODES_FILE);
	ifstream nodesarea (AREA_FILE);
	ofstream out (OUTPUT_FILE);

	int counter = 0;
	list<string>::iterator it2;
	list<string>::iterator it3;
	std::map<std::string, int> myMapnetarea;
	std::map<std::string, list<string>> myMappartition1;
	std::map<std::string, list<string>> myMappartition2;
	int totaldesignarea = 0;
	
	std::pair <std::map<std::string, std::string>,int> foo;
	int area = 0;
	int totalnodes = 0;
	string pnetName;
	
	int countermap = 0;
	
	std::pair <std::map<std::string, std::string>, int> keyvalmap;
	std::map<std::string, int> myMapnodearea;
	std::map<std::string, int> mypartition1;
	std::map<std::string, int> mypartition2;
	std::map<std::string, int> mypartition11;
	std::map<std::string, int> mypartition22;
	std::map<std::string, int> mypartition111;
	std::map<std::string, int> mypartition222;
	std::map<std::string, std::string> orignodesarea;
	string instanceName;
	float totalarea = 0;
	int v1 = 0;
	float partitionarea1 = 0;
	float partitionarea2 = 0;
	float percentpartition1 = 0;
	float percentpartition2 = 0;
	
	std::map<std::string, std::string> myMap;
	std::map<std::string, std::string> myMaparea;
	std::map<std::string, std::list<string>> netNodesList;
	
	int countermaparea = 0;
	
	std::pair <std::map<std::string, std::string>, int> keyvalmaparea;

	if (!nodesarea) {
		out << "Unable to open file";
		exit(1);
    }
	
	if (nodesarea.is_open())
    {
		while ( getline (nodesarea,linemaparea) )
		{
			keyvalmaparea = storenodes(linemaparea, countermaparea, myMaparea);
			myMaparea = keyvalmaparea.first;
			countermaparea = keyvalmaparea.second;
			linemaparea.clear();
		}
		
		out << "AREA OF MODULES: " << endl;
		for (const auto &p : myMaparea) {
          out << "marea[" << p.first << "] = " << p.second << '\n'; // marea[DSP48E] = 429
		}
		
		nodesarea.close();
    }
    else out << "Unable to open file";
	
	auto first_hurdle = std::chrono::high_resolution_clock::now();
	std::chrono::duration<double> elapsed_first_hurdle = first_hurdle - start;
	out << "=================" << endl;
	out << "Simulation time for parsing file 'nodes.area': " << elapsed_first_hurdle.count() << " seconds\n";
	out << "=================" << endl;
	
	////out << "Press any key to continue . . .";
	////cin.get();
	
	first_hurdle = std::chrono::high_resolution_clock::now();
	
	vector<string> lines_tokenize1;
		
		FILE *f1 = fopen(NODES_FILE, "rb");
	fseek(f1, 0, SEEK_END);
	long fsize1 = ftell(f1);
	fseek(f1, 0, SEEK_SET);

	char *string1 = (char *)malloc(fsize1 + 1);
	fread(string1, fsize1, 1, f1);
	fclose(f1);

	string1[fsize1] = 0;
	
	out << "Size of string: " << fsize1 << endl;

    lines_tokenize1 = split((string)string1, 0x0A);
	
	keyvalmap = storenodesarea(lines_tokenize1, orignodesarea, linemap, countermap, myMap, myMaparea);
			orignodesarea = keyvalmap.first;
			countermap = keyvalmap.second;
		
	  out << "NODES TO AREA: " << endl;
		for (const auto &p : orignodesarea) {
          out << "mnodearea[" << p.first << "] = " << p.second << '\n'; // mnodearea[inst_2142] = RAMB36E2
		  int v1 = rand() % 2;
		  if (v1 == 0)
			  mypartition1[p.first] = stoi(p.second);
		  else
			  mypartition2[p.first] = stoi(p.second);
		}
		
		mypartition11 = mypartition1;
		mypartition22 = mypartition2;
		
		for (const auto &p : mypartition1) {
          out << "mpartition1[" << p.first << "] = " << p.second << '\n';
		  partitionarea1 = partitionarea1 + p.second;
		}
		
		for (const auto &p : mypartition2) {
          out << "mpartition2[" << p.first << "] = " << p.second << '\n';
		  partitionarea2 = partitionarea2 + p.second;
		}
		
		totalarea = partitionarea1 + partitionarea2;
		
		out << "Total area in partition 1: " << partitionarea1 << endl;
		out << "Total area in partition 2: " << partitionarea2 << endl;
		
		percentpartition1 = (partitionarea1/totalarea)*100;
		percentpartition2 = (partitionarea2/totalarea)*100;
			
		if (percentpartition1 > LOW_AREA & percentpartition1 < HIGH_AREA) {
			out << "Partition 1 is between " << LOW_AREA << "%-" << HIGH_AREA << "%: " << percentpartition1 << endl;
			out << "Partition 2 is between " << LOW_AREA << "%-" << HIGH_AREA << "%: " << percentpartition2 << endl;
		}
		else {
			out << "\033[1;31mALERT: Partition 1 & Partition 2 are not in between " << LOW_AREA << "%-" << HIGH_AREA << "%\033[0m" << endl;
			out << "Partition 1 is between " << LOW_AREA << "%-" << HIGH_AREA << "%: " << percentpartition1 << endl;
			out << "Partition 2 is between " << LOW_AREA << "%-" << HIGH_AREA << "%: " << percentpartition2 << endl;
		}
	
	auto second_hurdle = std::chrono::high_resolution_clock::now();
	std::chrono::duration<double> elapsed_second_hurdle = second_hurdle - first_hurdle;
	out << "=================" << endl;
	out << "Simulation time for parsing file 'design.nodes': " << elapsed_second_hurdle.count() << " seconds\n";
	out << "=================" << endl;
	
	////out << "Press any key to continue . . .";
	////cin.get();
	
	second_hurdle = std::chrono::high_resolution_clock::now();
	
	vector<string> lines_tokenize;
	
	FILE *f = fopen(NETS_FILE, "rb");
	fseek(f, 0, SEEK_END);
	long fsize = ftell(f);
	fseek(f, 0, SEEK_SET);

	char *string2 = (char *)malloc(fsize + 1);
	fread(string2, fsize, 1, f);
	fclose(f);

	string2[fsize] = 0;
	
	out << "Size of string: " << fsize << endl;

    lines_tokenize = split(string2, 0x0A);
	
	  auto output = storetuple(lines_tokenize, netNodesList, pnetName, totalnodes, line, counter, myMaparea, myMapnetarea, area);
	  netNodesList = std::get<0>(output);
	  pnetName = std::get<1>(output);
	  area = std::get<2>(output);
		
	  for(auto& pair : netNodesList) {
		out << pair.first << ":";
		for(auto& i : pair.second) {
			out << "->" << i;
		}
		out << endl;
	  }
	  
	  	auto third_hurdle = std::chrono::high_resolution_clock::now();
	std::chrono::duration<double> elapsed_third_hurdle = third_hurdle - second_hurdle;
	out << "=================" << endl;
	out << "Simulation time for parsing file 'design.nets': " << elapsed_third_hurdle.count() << " seconds\n";
	out << "=================" << endl;
	
	////out << "Press any key to continue . . .";
	////cin.get();
	  
		int addswappedarea = 0;
		int cutset = 1;
		int h = 0;
		int newnodes = 0;
		std::vector<int> maxgains;
		std::vector<std::string> shiftednodesvector;
		std::map<std::string, int> maxgainaftershiftnode;
		int maxcomputegains = 0;
		std::string shiftednode = "DUMMY";
		
		out << "Total number of nodes present in the design: " << totalnodes << endl;
		
		int numOfShifts = 1;
		int numOfPasses = 1;
		int pass = 0;
		while (pass < numOfPasses) {
			h = 0;
			//out << "===STARTED " << pass << " PASS===" << endl;
			//cin.get();
			maxgainaftershiftnode.clear();
			shiftednodesvector.clear();
		while(h < numOfShifts) { // Include for specific number of shifts.
		//while(cutset > 0) { // Include for all shifts.
		  newnodes = totalnodes-h;
		  if (newnodes >= 0) {
			  if (percentpartition1 > LOW_AREA & percentpartition1 < HIGH_AREA) {
				// Record 1st hurdle time
				auto third_hurdle = std::chrono::high_resolution_clock::now();

				// Effective simulation time
				std::chrono::duration<double> elapsed_third_hurdle = third_hurdle - second_hurdle;
		
				out << "=================" << endl;
				out << "Simulation time @ " << h << " loop start: " << elapsed_third_hurdle.count() << " seconds\n";
				out << "=================" << endl;
		
				out << "=====" << h << ". Computing new cutset=====" << endl;
				maxcomputegains = 0;
				auto output = shift_and_find_cutset(maxgainaftershiftnode, shiftednode, counter, netNodesList, maxcomputegains, percentpartition1, percentpartition2, cutset, addswappedarea, mypartition1, mypartition2);
				mypartition1 = std::get<0>(output);
				mypartition2 = std::get<1>(output);
				shiftednode = std::get<2>(output);
				out << "MAXCOMPUTEGAIN: " << maxcomputegains << endl;
				out << "SHIFTEDNODE: " << shiftednode << endl;
				shiftednodesvector.push_back(shiftednode);
				maxgainaftershiftnode[shiftednode] = maxcomputegains;
				out << "Shifted " << h << " nodes with area (Partition 1 to Partition 2 OR Partition 2 to Partition 1): " << addswappedarea << endl;
				out << "Total number of nodes left in the design: " << newnodes << endl;
				out << "=====End of net cutset computation=====" << endl;
		  
				// Record 1st hurdle time
				auto fourth_hurdle = std::chrono::high_resolution_clock::now();

				// Effective simulation time
				std::chrono::duration<double> elapsed_fourth_hurdle = fourth_hurdle - second_hurdle;
		
				out << "=================" << endl;
				out << "Simulation time @ " << h << " loop end: " << elapsed_fourth_hurdle.count() << " seconds\n";
				out << "=================" << endl;
			  }
			  else {
				out << "\033[1;31mALERT: Partition 1 & Partition 2 are not in between " << LOW_AREA << "%-" << HIGH_AREA << "%\033[0m" << endl;
				out << "\033[1;31mALERT: TERMINATING THE PROCESS...\033[0m" << endl;
				break;
			  }
		  }
		  else {
			out << "\033[1;31mALERT: No nodes left to swap!\033[0m" << endl;
			////out << "Press any key to continue . . .";
			////cin.get();
			break;
		}
		h = h + 1;
		}
  
    string maximumgainnode;
	
    if(!(maxgainaftershiftnode.empty())) {
      auto maximumgain = std::max_element(maxgainaftershiftnode.begin(), maxgainaftershiftnode.end(), [](const pair<string, int>& p1, const pair<string, int>& p2) {return p1.second < p2.second; });
      out << maximumgain->first << "=>" << maximumgain->second << endl;
	  maximumgainnode = maximumgain->first;
	}
	
	mypartition1.clear();
	mypartition2.clear();
	
	mypartition111 = mypartition11;
	mypartition222 = mypartition22;
	std::vector<string> looped;
	
	int i = 0;
	int j = 0;
	std::string x;
	out << "The following nodes are shifted till we get the good cutset: " << endl;
	
	for (auto it = shiftednodesvector.begin(); it != shiftednodesvector.end(); ++it) {
        //out << ' ' << *it;
		out << "MAXGAIN[" << *it << "] = " << maxgainaftershiftnode[*it] << endl;
		i = i + 1;
		x = *it;
		if (mypartition111.count(x) > 0) {
			mypartition222.insert({x, mypartition111.find(x)->second});
			mypartition111.erase(x);
		}
		else if (mypartition222.count(x) > 0){
			mypartition111.insert({x, mypartition222.find(x)->second});
			mypartition222.erase(x);
		}
		if(maximumgainnode.compare(x) == 0) {
			j = i;
			break;
		}
	}
	
	out << "It took " << j << " number of iterations to reach the good cutset." << endl;
	
	partitionarea1 = 0;
	partitionarea2 = 0;
	
	for (const auto &p : mypartition111) {
        out << "mpartition1[" << p.first << "] = " << p.second << '\n';
		partitionarea1 = partitionarea1 + p.second;
	}
		
	for (const auto &p : mypartition222) {
        out << "mpartition2[" << p.first << "] = " << p.second << '\n';
		partitionarea2 = partitionarea2 + p.second;
	}
	
		mypartition1.clear();
		mypartition2.clear();
		mypartition1 = mypartition111;
		mypartition2 = mypartition222;
		
		partitionarea1 = partitionarea1; // + *(addswappedarea + 0);
		partitionarea2 = partitionarea2; // + *(addswappedarea + 1);
		totalarea = partitionarea1 + partitionarea2;
		
		out << "Total design area: " << totalarea << endl;
		out << "Total number of nets in the given design: " << counter << endl;
		out << "Total area in partition 1: " << partitionarea1 << endl;
		out << "Total area in partition 2: " << partitionarea2 << endl;
		
		percentpartition1 = (partitionarea1/totalarea)*100;
		percentpartition2 = (partitionarea2/totalarea)*100;
			
		if (percentpartition1 > LOW_AREA & percentpartition1 < HIGH_AREA) {
			out << "Partition 1 is between " << LOW_AREA << "%-" << HIGH_AREA << "%: " << percentpartition1 << endl;
			out << "Partition 2 is between " << LOW_AREA << "%-" << HIGH_AREA << "%: " << percentpartition2 << endl;
		}
		else {
			out << "\033[1;31mALERT: Partition 1 & Partition 2 are not in between " << LOW_AREA << "%-" << HIGH_AREA << "%\033[0m" << endl;
			out << "Partition 1 is between " << LOW_AREA << "%-" << HIGH_AREA << "%: " << percentpartition1 << endl;
			out << "Partition 2 is between " << LOW_AREA << "%-" << HIGH_AREA << "%: " << percentpartition2 << endl;
		}
		pass = pass + 1;
	}
	
	// Record end time
	auto finish = std::chrono::high_resolution_clock::now();

	// Effective simulation time
	std::chrono::duration<double> elapsed = finish - start;
	
	out << "=================" << endl;
	out << "\033[1;32mSimulation time: " << elapsed.count() << " seconds\033[0m\n"; // design - 2 - Simulation time: 749.597 seconds, design1 - 1 - Simulation time: 3.25841 seconds, design2 - FPGA3 - 
	out << "=================" << endl;
	
	//out << "Press any key to continue . . .";
	//cin.get();
	
	return 0;
	
}