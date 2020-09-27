#include <iostream>
#include <vector>
#include <fstream>
#include <algorithm>

using namespace std;

int N,K;
vector< pair<float,float> > v;
vector< pair<float, int> > distances;

void read_input(char *filename)
{
	ifstream fin(filename);
	fin>>N>>K;
	int i;
	float x,y;
	for(i=0;i<N;i++)
	{
		fin>>x>>y;
		v.push_back(make_pair(x,y));
	}
	fin.close();
}

void write_k_distances(char *filename)
{
	ofstream fout(filename);
	
	int i,j;
	for(i=0;i<N;i++)
	{
		/*new distances*/
		distances.clear();
		for(j=0;j<N;j++)
		{
			if(i!=j)
			{
				/*distance from i to j */
				float xdiff = v.at(j).first - v.at(i).first;
				float ydiff = v.at(j).second - v.at(i).second;
				
				float dist = xdiff*xdiff + ydiff*ydiff;
				
				distances.push_back(make_pair(dist,j+1));
			}						
		}
		sort(distances.begin(), distances.end());
		fout<<(int)(i+1)<<" ";
		for(j=0;j<K-1;j++)
		{
			fout<<distances.at(j).second<<" ";
		}
		fout<<distances.at(K-1).second<<endl;
	}
	fout.close();
}

int main(int argc, char **argv)
{
	if(argc!=3)
	{
		cout<<" Usage: main.exe input_file_name output_file_name"<<endl;
		return 0;
	}
	read_input(argv[1]);
	write_k_distances(argv[2]);	
	return 0;
}