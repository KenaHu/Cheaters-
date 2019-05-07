#include <sys/types.h>
#include <dirent.h>
#include <errno.h>
#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <stdlib.h>
#include "Table.h"


using namespace std;


int getdir (string dir, vector<string> &files);
unsigned int TableSize = 50007;
unsigned int hashFunction (string key, unsigned int TableSize);

int main(int argc, char* argv[]){
    string dir = string(argv[1]);
    vector<string> files = vector<string>();
    getdir(dir,files);
    
    int numFiles = files.size();
    int chunkSize = atoi(argv[2]);
    int cheaters = atoi (argv[3]);
    int Array[numFiles][numFiles];
    Table Hash[TableSize];

    for(int i=2; i<numFiles; i++){
	for(int j=i+1; j<numFiles; j++){
		Array[i][j]=0;
	}
    }

    for (int i=2; i < numFiles; i++){   //for all text files
	vector<string> chunks = vector<string>();
	string data;
	chunks.clear(); 
	string f = files[0] + "/" + argv[1] + "/" + files[i];   //get path
	ifstream myfile (f.c_str());
	if(myfile.is_open()){
		while (myfile >> data){
			string key = "";
        		if (chunks.size() < chunkSize)   //get chunks
                		chunks.push_back(data);
			else{
				chunks.erase(chunks.begin());
				chunks.push_back(data);
			}
			for (vector<string>::iterator j = chunks.begin(); j != chunks.end(); j++)	{   
				key += *j;
		 	}
			for (int it = 0; it < key.length();it++){   //scrub chunks
				if((key[it] >= 97) && (key[it] <= 122)){}
				else if ((key[it] >= 48) && (key[it] <= 57)){}
				else if ((key[it]>=65) && (key[it]<=90)){
					key[it] = key[it] + 32;
				}
				else
					key[it] = 0;
			} 
			unsigned int hashValue = hashFunction(key, TableSize);
			Hash[hashValue].addFile(i);
		}
		myfile.close();
	}
	else
	cout << "Unable to open file " << files[i] << endl;
    }

    for(unsigned int index = 0;index < TableSize; index++){
	vector<int> Docs = vector<int>();
	Docs.clear();
	Hash[index].getFileList(Docs);
	if(Docs.size() > 1){   //Compute collisions
		for(int i=0; i<Docs.size()-1;i++){
			for(int j=i+1; j<Docs.size();j++){
				Array[Docs[j]][Docs[i]]+=1;
			}
		}
	}
    }
    
    vector<int> Collisions = vector<int>();
    vector<int> x = vector<int>();
    vector<int> y = vector<int>();
    for (int i=2; i<numFiles-1;i++){ //MAKE THIS OUTPUT IN ORDER!!!
	for (int j=i+1;j<numFiles;j++){
		if(Array[i][j] > cheaters){
			Collisions.push_back(Array[i][j]);
			x.push_back(i);
			y.push_back(j);
		}
	}
    }
    while(Collisions.size() > 0){
	int max = 0;		
	for(int i = 0; i < Collisions.size();i++){
		if (Collisions[i] > Collisions[max])
			max = i;
	}
	cout << Collisions[max] << ": " << files[x[max]] << ", " << files[y[max]] << endl;
	Collisions.erase(Collisions.begin()+max);
	x.erase(x.begin()+max);
	y.erase(y.begin()+max);	
    }
    return 0;
}

/*function... might want it in some class?*/
int getdir (string dir, vector<string> &files){
    DIR *dp;
    struct dirent *dirp;
    if((dp  = opendir(dir.c_str())) == NULL) {
        cout << "Error(" << errno << ") opening " << dir << endl;
        return errno;
    }

    while ((dirp = readdir(dp)) != NULL) {
        files.push_back(string(dirp->d_name));
    }
    closedir(dp);
    return 0;
}

unsigned int hashFunction(string key, unsigned int TableSize){
    unsigned long result = 0;
    for (int i=0;i<key.length();i++){
    	unsigned long power = 1;
	for (int a=0; a<i; a++){
		power *= 3;
		power = power % TableSize;
	}
	result += (int) key[i] * power;
	result = result % TableSize;	
    }	
    return result;
}

