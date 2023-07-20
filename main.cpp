#include<bits/stdc++.h>
using namespace std;
 
#define endl "\n"
#define ll long long
const int N = 1e5 + 7;
 
struct Node{
    bool accept;
    map<char, vector<Node *>> neighbours;
  
    Node(){
    	accept = false;
    }
};
 
struct nfa_list{
    Node * start_node;
    Node * final_node;
    
    //constructing nfa with a given character
    nfa_list(char ch){
    	Node* start = new Node();
    	Node* end = new Node();
    	start->neighbours[ch].push_back(end);
    	start_node = start;
    	final_node = end;
    	final_node->accept = true;
    }
};

stack<nfa_list*>nfas;
string reg,mat;
stack<char> st; 
 
void pluss(){
    auto nfa1 = nfas.top();
    nfas.pop();
 
    nfa1->final_node->neighbours['e'].push_back(nfa1->start_node);
    nfas.push(nfa1);
}
 
void mult(){
    auto nfa1 = nfas.top();
    nfas.pop();
 
    Node* dummy = new Node();
    dummy->neighbours['e'].push_back(nfa1->final_node);
    dummy->neighbours['e'].push_back(nfa1->start_node);
    nfa1->final_node->neighbours['e'].push_back(nfa1->start_node);
    nfa1->start_node = dummy;
    nfas.push(nfa1);   
}
 
void unionn(){
    //poping out the two lists
    auto nfa2 = nfas.top();
    nfas.pop();
    auto nfa1 = nfas.top();
    nfas.pop();
 
    //inserting dummy start node
    Node* new_start = new Node();
    new_start->neighbours['e'].push_back(nfa1->start_node);
    new_start->neighbours['e'].push_back(nfa2->start_node);
 
    //inserting dummy final node
    Node* new_final = new Node();
    new_final->accept = true;
    nfa1->final_node->neighbours['e'].push_back(new_final);
    nfa2->final_node->neighbours['e'].push_back(new_final);
    nfa1->final_node->accept = false;
    nfa2->final_node->accept = false;
 
    //pushing back the nfalist in the stack after modifying nfa1
    nfa1->start_node = new_start;
    nfa1->final_node = new_final;
    nfas.push(nfa1);
}
 
void concatt(){
    //poping out the two lists
    auto nfa2 = nfas.top();
    nfas.pop();
    auto nfa1 = nfas.top();
    nfas.pop();
 
    nfa1->final_node->accept = false;
    nfa1->final_node->neighbours['e'].push_back(nfa2->start_node);
 
    //pushing back the nfalist in the stack after modifying nfa1
    nfa1->final_node = nfa2->final_node;
    nfas.push(nfa1);
}

bool match(int ind, int n, string s, Node *curr){
	if(curr->accept == true && ind == n){
		return true;
	}

	bool check = false;
	for(auto c: curr->neighbours){
		char ch = c.first;
		vector<Node *> v = c.second;
		if(ch == 'e'){
			for(Node* nod: v){
				check = check || match(ind,n,s,nod);
			}
		}
		else
		if(ch == s[ind]){
			for(Node* nod: v){
				check = check || match(ind+1,n,s,nod);
			}
		}
	}
	return check;
}
 
int main()
{
	#ifndef ONLINE_JUDGE
    //input from input.txt
    freopen("input.txt", "r", stdin);
    //output on output.txt
    freopen("output.txt", "w", stdout);
    #endif
    
    string exp;
    string s;
    cin>>exp>>s;
 
    if(exp.size() == 0 && s.size() == 0){
        return 0;
    }
 
    int len = exp.length();
 
    // for modifying the regex
    reg.push_back(exp[0]);
    for(int i = 1; i < len; i++){
        if(exp[i] == '(' && exp[i-1] == ')'){
            reg.push_back('.');
            reg.push_back('(');
        }
        else
        {
            reg.push_back(exp[i]);
        }
    }           
    
    len = reg.length();
    for(int i = 0; i < len; i++){
        if(reg[i] == '(' || reg[i] == '|' || reg[i] == '.'){
            st.push(reg[i]);
        }
        else
        if(reg[i] == '*'){
            mult();
        }
        else
        if(reg[i] == '+'){
            pluss();
        }
        else
        if(reg[i] == 'a'){
        	nfa_list* a_nfa = new nfa_list(reg[i]);
        	nfas.push(a_nfa);
        }
        else
        if(reg[i] == 'b'){
        	nfa_list* b_nfa = new nfa_list(reg[i]);
        	nfas.push(b_nfa);
        }
        else
        if(reg[i] == ')'){
            char ch = st.top();
            st.pop();

            if(ch == '.'){
            	concatt();
            	st.pop();
            }
            else
            if(ch == '|'){
            	unionn();
            	st.pop();
            }
        }
    }
 	
 	// bugging the final nfa list
 	nfa_list* list = nfas.top();
 	Node* curr = list->start_node;
 	
 	int len2 = s.length();
 	Node* curr2 = curr;
 	string ans;

	int i = 0, j = len2;
	while(i < len2){
		while(i < j){
			bool mat = match(i,j,s,curr);
			if(mat == true){
				ans.push_back('$');
				ans += s.substr(i,(j-i));
				i = j;
				j = len2;
			}
			else
			{
				j--;
			}
		} 
		if(i == len2){
			break;
		}
		ans.push_back('@');
		ans.push_back(s[i]);
		i++;
		j = len2;	
 	}
 	ans.push_back('#');
 	cout<<ans<<endl;
}