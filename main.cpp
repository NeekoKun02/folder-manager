#include <iostream>
#include <cstdlib>
#include <ctime>
#include <vector>
#include <string.h>

#define EMPTY 1
#define FOLDER 2
#define TEXT 3

/*
cd to change dir
dir to see the actual dir
mkdir to create a sub directory
rmdir to remove a directory
tree to show the tree structure
rename to give a new name to a dir
*/

using namespace std;

struct node{
    int type = 0;
    node *parallel = NULL;  // Used to iterate trough same-level folders
    node *previous = NULL;  // Used to get to the upper folder level
    node *content = NULL;   // Used to access the content of the folder: either files or subfolders
    string name = "~";
};

void errorManager(int type){
    cout<<endl<<"    Error [0d"<<type<<"]: ";

    if(type == 0)
        cout<<"Non-specified name for folder (type <-help mkdir> for more info)"<<"\n\n";
    else if(type == 1)
        cout<<"The specified directory could not be found"<<"\n\n";
    else if(type == 2)
        cout<<"The provided command could not be found"<<"\n\n";
    else if(type == 3)
        cout<<"The folder name must not be over 7 characters long"<<"\n\n";
    else if(type == 4)
        cout<<"Non-specified name for folder (-help cd for more info)"<<"\n\n";
    else if(type == 5)
        cout<<"The specified name is already in use for a folder in the same directory"<<"\n\n";    
    else if(type == 6)
        cout<<"The specified folder does not exist in the current directory (type <-help rename> for more info)"<<"\n\n";
    else if(type == 7)
        cout<<"Non-specified name for folder (type <-help rename> for more info)"<<"\n\n";
    else if(type == 8)
        cout<<"Rename function takes 2 arguments, 1 was given"<<"\n\n";
    else if(type == 9)
        cout<<"The specified folder does not exist in the current directory (type <-help rmdir> for more info)"<<"\n\n";
    else if(type == 10)
        cout<<"Rmdir function takes 1 arguments, 0 were given"<<"\n\n";
    else
        cout<<"The error is not listed"<<"\n\n";

    return;
}

node *append(node *&head, node *&toAppend){
    toAppend->parallel = head->parallel;
    head->parallel = toAppend;
    
    return head;
}

node *mkdir(node *&base, string name){
    if(name == ""){
        errorManager(0);

        return base;
    }if(name.length() > 7){
        errorManager(3);

        return base;
    }
    node *temp = base->content;

    while(temp->parallel != NULL){
        temp = temp->parallel;
        if(temp->name == name){
            errorManager(5);
            return base;
        }
    }

    node *newDir = new node;
    newDir->name = name;
    newDir->content = new node{1};
    newDir->previous = base;
    base->content = append(base->content, newDir);

    return base;
}

void rmdir(node *&position, string name){
    node *temp = position->content;
    node *removed = new node;

    while(temp->parallel != NULL){
        if(temp->parallel->name == name){
            removed = temp->parallel;
            temp->parallel = temp->parallel->parallel;
            delete removed;

            return;
        }
        temp = temp->parallel;
    }
    
    errorManager(9);

    return;
}

node *cd(node *&position, string name){
    node *temp = position->content;

    if(name == ".." && position->previous != NULL)
        return position->previous;

    while(temp->parallel != NULL){
        temp = temp->parallel;

        if(temp->name == name){
            return temp;
        }
    }

    errorManager(1);

    return position;
}

void dir(node *&position){
    node *temp = position->content;

    cout<<"    Name:"<<'\t'<<"Address:"<<'\t'<<"Type:"<<endl<<endl;

    while(temp->parallel != NULL){
        temp = temp->parallel;

        cout<<"    "<<temp->name<<'\t'<<temp<<'\t'<<temp->type<<endl;
    }
}

string getPath(node *&position, string space = "\\"){
    string path = ">";
    node *temp = position;

    path.insert(0, position->name);

    while(temp->previous != NULL){
        path.insert(0, space);
        temp = temp->previous;
        path.insert(0, temp->name);
    }

    return path;
}

void tree(node *&position, int level = 0){
    node *temp = position->content;

    while(temp->parallel != NULL){
        temp = temp->parallel;
        if(temp->parallel != NULL){
            for(int i=0; i<level; i++){
                if(temp->previous->parallel != NULL)
                    cout<<(char)179<<"   ";
                else
                    cout<<"    ";
            }

            cout<<(char)195<<(char)196<<(char)196<<(char)196<<temp->name<<'\n';

            tree(temp, level+1);
        }else{
            for(int i=0; i<level; i++){
                if(temp->previous->parallel != NULL)
                    cout<<(char)179<<"   ";
                else
                    cout<<"    ";
            }

            cout<<(char)192<<(char)196<<(char)196<<(char)196<<temp->name<<'\n';

            tree(temp, level+1);
        }
    }

    return;
}

vector<string> split(string input){
    vector<string> output;
    string arg = "";

    for(char& i : input){
        if(i != ' ')
            arg.push_back(i);
        else{
            output.push_back(arg);
            arg = "";
        }
    }

    output.push_back(arg);

    return output;
}

void rename(node *&position, string name, string newName){
    node *temp = position->content;

    while(temp->parallel != NULL){
        temp = temp->parallel;
        if(temp->name == name){
            temp->name = newName;
            return;
        }
    }

    errorManager(6);
    return;
}

int main(){
    node *root = new node;
    node *position = root;
    vector<string> argc;
    string input;
    bool running = true;

    system("CLS");

    root->type = FOLDER;
    root->content = new node{1};

    while(running){
        cout<<getPath(position);
        getline(cin, input);

        cout<<"    Recived ["<<input<<"]"<<endl<<endl;

        argc = split(input);

        if(argc[0] == "exit")
            break;
        else if(argc[0] == "mkdir"){
            if(argc.size() >= 2)
                position = mkdir(position, argc[1]);
            else
                errorManager(0);
        }else if(argc[0] == "dir")
            dir(position);
        else if(argc[0] == "cd"){
            if(argc.size() >= 2)
                position = cd(position, argc[1]);
            else
                errorManager(4);
        }else if(argc[0] == "rename"){
            if(argc.size() >= 3)
                rename(position, argc[1], argc[2]);
            else if(argc.size() == 2)
                errorManager(7);
            else
                errorManager(8);
        }else if(argc[0] == "rmdir"){
            if(argc.size() >= 2)
                rmdir(position, argc[1]);
            else
                errorManager(10);
        }else if(argc[0] == "tree")
            tree(position);
        else
            errorManager(2);
    }

    dir(position);

    return 0;
}