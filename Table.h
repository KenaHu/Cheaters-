#ifndef TABLE_H
#define TABLE_H
#include <iostream>
#include <string>
#include <vector>
#include <stdlib.h>

using namespace std;

class Table {

private:

    struct Node {
	int file;
	Node *next;
    };

    Node *head;

public:

    Table(){
	head = NULL;
    }

    void addFile(int const &f){
	Node* temp = new Node;
	temp->file = f;
	temp->next = head;
	head = temp;
    }

    void removeFile(int const &f){
	if (head == NULL){
		cout << "File Not Found" << endl;
        	return;
    	}

	Node* p = head;
	Node* prev = NULL;

	while((p->next != NULL) && (p->file != f)){
        	prev = p;
        	p = p->next;
    	}
	if (prev == NULL){
        	head = p->next;
        	delete p;
        	return;
    	}
    	if (p->file == f){
        	prev->next = p->next;
        	delete p;
        	return;
    	}
    	cout << "File Not Found" << endl;
    	return;
    }
    
    void showFileList(){
	for(Node *p=head; p!=NULL; p=p->next){
		cout <<  p->file << " ";
	}
    }

    void getFileList(vector<int> &sim){
	for(Node *p=head; p!=NULL; p=p->next)
		sim.push_back(p->file);
    }
    ~Table(){
	while(head!=NULL)
		removeFile(head->file);
    }
};

#endif	
