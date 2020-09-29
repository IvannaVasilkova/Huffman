
//
//  main.cpp
//  Huffman
//
//  Created by Ivanna Vasilkova on 20.09.2020.
//  Copyright © 2020 Ivanna Vasilkova. All rights reserved.
//
#include <iostream>
#include <vector>
#include <map>
#include <list>
#include <fstream>
#include <iostream>
#include <fstream>
#include <sstream> // для std::stringstream
#include <cstdlib> // для exit()

using namespace std;

vector<bool> code;
map<string,vector<bool> > table;
class Node
{
public:
    int a;
    string c;
    Node *left, *right;
    
    Node() {left=right=NULL;}
    
    Node(Node *L, Node *R)
    {  left =  L;
        right = R;
        a = L->a + R->a;  }
};


struct MyCompare
{
    bool operator()(const Node* l, const Node* r) const { return l->a < r->a; }
};



void BuildTable(Node *root)
{
    if (root->left!=NULL)
    { code.push_back(0);
        BuildTable(root->left);
    }
    
    if (root->right!=NULL)
    { code.push_back(1);
        BuildTable(root->right);}
    
    if (root->left==NULL && root->right==NULL) table[root->c]=code;
    
    code.pop_back();
}

    
void makeTree(Node*& cur, string::iterator key, string value) {
   if (cur == NULL)
      cur = new Node();
   if (*key == '0')
       makeTree(cur->left, ++key, value);
   else if (*key == '1')
       makeTree(cur->right, ++key, value);
   else {
       cur->c = value;
   }
    
}

void Encode(string pathInitial, int countSymbols, string pathFinite){

    //_______________________READ FROM FILE______________________________________________
    
    ifstream f(pathInitial, ios::out | ios::binary);
    
    
    map<string,int> m;
    while (!f.eof())
    {
        char str[countSymbols+1];
        f.get(str, countSymbols+1);
        string c = string(str);
        m[c]++;
        
    }
    
    //__________________________ADD POINTERS_________________________________________________
    list<Node*> t;
    for( map<string,int>::iterator itr=m.begin(); itr!=m.end(); ++itr)
    {
        Node *p = new Node;
        p->c = itr->first;
        p->a = itr->second;
        t.push_back(p);      }
    // t.pop_front();
    
    //____________________CREATE A TREE_______________________________________________________
    while (t.size()!=1)
    {
        t.sort(MyCompare());
        
        Node *SonL = t.front();
        t.pop_front();
        Node *SonR = t.front();
        t.pop_front();
        
        Node *parent = new Node(SonL,SonR);
        t.push_back(parent);
        
    }
    
    Node *root = t.front();
    
    //_______________________CREATE A TABLE_________________________________________________
    BuildTable(root);
    
    
    //________________________OUTPUT TABLE TO FILE___________________________________________
    std::ofstream N;          // поток для записи
    N.open(pathFinite, ios::out | ios::binary);
    
    
    map <string,vector<bool> >::iterator n;
    for (n=table.begin(); n!=table.end(); ++n)
    {
        vector<bool>& vec = n->second;
        for (vector<bool>::iterator it = vec.begin(); it != vec.end(); ++it)
        {
        //cout << *it << "";
            N << *it;
            
        }
        
        //cout << ":";
        N << "#";
        N <<  n->first;
        
        // cout << n->first;
        //cout<<endl;
        N << "\n";
      
    }
        N<<"#";
      
    
    //____________________________WRITE BYTES TO FILE________________________________________
    
    f.clear(); f.seekg(0);
    //ВЫВОД В БИНАРНЫЙ ФАЙЛ
    //ofstream g(pathFinite, ios::app | ios::binary);
    
    int count=0; char buf=0;
    while (!f.eof())
    {
        char str[countSymbols+1];
        f.get(str, countSymbols+1);
        string c = string(str);
        
        //  char c = f.get();
        vector<bool> x = table[c];
        for(int n=0; n<x.size(); n++)
        {buf = buf | x[n]<<(7-count);
           // cout <<x[n];
            count++;
            if (count==8) { count=0;   N<<buf; buf=0; }
        }
    }
  //   cout<<"count"<<count<<endl;
    if (count) {
        N<<buf;
    }
  
    f.close();
    N.close();
    
    
}

void Decode(string pathFinite, string pathDecode){
    std::ifstream input(pathFinite);
    std::string key, value;
    Node *root = new Node();
    std::getline(input, key, '#');
    while (key != ""){
        std::getline(input, value);
     //    std::cout << "key --  " << key << " value --  " << value << '\n';
        
        std::string::iterator it;
        it = key.begin();
        while (it!= key.end()){
            //   cout<<*it;
            ++it;
            
        }
   //    std::cout << '\n';
        
        makeTree(root, key.begin(),value);
        std::getline(input, key, '#');
    }
   // input.close();
    ofstream M;  M.open(pathDecode);
    //ifstream F(pathFinite, ios::in | ios::binary);

    
    //___________________DECODE HUFFMAN________________________________________________________
      setlocale(LC_ALL,"Russian");

      
      Node *p = root;
      int count=0;
      char byte;
      byte = input.get();
      while(!input.eof())
      {   bool b = byte & (1 << (7-count) ) ;
          if (b) p=p->right; else p=p->left;
          if (p->left==NULL && p->right==NULL) { M<<p->c; p=root;}
          count++;
          if (count==8) {count=0; byte = input.get(); }
      }
      
      input.close();

    
} //


int main(int argc, const char * argv[])

{
    
//
//   string pathInitial = "text.txt";
//   int countSymbols = 1;
//   string pathFinite ="output.bin";
//   string pathDecode = "final.txt";
//   Encode(pathInitial,countSymbols, pathFinite);
//   Decode(pathFinite, pathDecode);
   
    string pathInitial(argv[1]);
       cout <<"pathInitial     "<<pathInitial<<endl;

       string DecodeOrCode(argv[2]);
    cout <<"DecodeOrCode    " <<DecodeOrCode<<endl;

       std::stringstream convert(argv[3]);
       int countSymbols;
       if (!(convert >> countSymbols)) // выполняем конвертацию
           countSymbols = 1;
      cout << "countSymbols     " << countSymbols;


       string pathFinite(argv[4]);
       cout<<"pathFinite    "<<pathFinite<<endl;



       if((std::string(argv[2]) == "-e") || (std::string(argv[2]) == "-E")){
           cout <<"e or d"<< std::string(argv[2])<<endl;
           Encode(argv[1], countSymbols,argv[4]);
           return 0;
       }
       if((std::string(argv[2]) == "-d") || (std::string(argv[2]) == "-D")){
          cout <<"e or d"<< std::string(argv[2])<<endl;
           Decode(argv[1],argv[4]);
           return 0;
       }


    return 0;
}
