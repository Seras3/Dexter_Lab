#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <vector>
#include <algorithm>
#include <queue>
#include <cstring>

using namespace std;

struct Lclosure{
    int nr;
    int arr[100];
    int len;
    void print();
    void copyarr(int v[100], int len);
}closures[100];

struct LetterIntList{
    void copyarr(int v[100], int len);
};

struct ArrayLen{
    int arr[100];
    int len;
    void copyarr(int vct[100], int n);
    ArrayLen();
    ArrayLen(const ArrayLen&);
};


int len;
int count_states;
int count_alfabet;
int count_final_states;
string alfabet[100];
int first_state;
int final_states[100];
int lambda_states[100];
map<int, map<int, vector <string>>> afnL;
map<string, map<string, vector <string>>> afd;
map<int, map<string, ArrayLen>> afnLT;

void Lclosure::print()
{
    cout<<nr<<" : ";
    for(int i = 0; i < len; i++)
        if(i < len-1)
            cout<<arr[i]<<", ";
        else
            cout<<arr[i];
    cout<<endl;
}

void Lclosure::copyarr(int v[100], int len)
{
    this->len = len;
    for(int i = 0; i < len; i++)
        arr[i] = v[i];
}


void ArrayLen::copyarr(int vct[100], int n)
{
    for(int i = 0; i < n; i++)
    {
        arr[i] = vct[i];
    }
    len = n;
}

ArrayLen::ArrayLen()
{
    len = 0;
    for(int i = 0; i < 100; i++)
        arr[i] = 0;
}

ArrayLen::ArrayLen(const ArrayLen& origin)
{
    len = origin.len;
    for(int i = 0; i < len; i++)
        arr[i] = origin.arr[i];
}



void readData()
{
    ifstream f ("date.in");
    int c1;     // cheie 1
    int c2;    // cheie 2
    string alf;     // litera
    int count_alf;  // nr de litere;
    f>>count_states;
    for(int i = 0; i < count_states; i++)
    {
        f>>c1;
        afnL.insert(make_pair(c1, map<int, vector <string>>()));
    }
    f>>count_alfabet;
    for(int i = 0; i < count_alfabet; i++)
    {
        f>>alf;
        alfabet[i] = alf;
    }
    f>>first_state;
    f>>count_final_states;
    for(int i = 0; i < count_final_states; i++)
    {
        f>>c1;
        final_states[i] = c1;
    }
    while(f>>c1>>c2)
    {
        f>>count_alf;
        vector <string> l1;
        for(int i = 0; i < count_alf; i++)
        {
            f>>alf;
            l1.push_back(alf);
        }
        afnL[c1].insert(make_pair(c2, l1));
    }
    f.close();
}

void afiAfnL()
{
    map<int, map<int, vector <string>>>::iterator itr;
    map<int, vector <string>>::iterator ptr;
    vector<string>::iterator its;
    for(itr = afnL.begin(); itr != afnL.end(); itr++)
    {
        for(ptr = itr->second.begin(); ptr != itr->second.end(); ptr++)
        {
            cout<<itr->first<<" ";
            cout<<ptr->first<<" ";
            cout<<"{ ";
            for(its = ptr->second.begin() ; its != ptr->second.end(); its++)
            {
                cout<<*its<<" ";
            }
            cout<<"}\n";
        }
        cout<<endl;
    }
}

string vToString(int v[100], int len)
{
    string s = "";
    sort(v, v+len);
    for(int i = 0; i < len; i++)
    {
        if( i < len-1 )
            s += to_string(v[i])+",";
        else
            s += to_string(v[i]);
    }
    return s;
}

void afiVect(int v[100], int len)
{
    for(int i = 0; i < len; i++)
        cout<<v[i]<<" ";
    cout<<endl;
}


bool findIn(vector<string> v, string x)
{
    vector<string>::iterator itr;
    for(itr = v.begin(); itr != v.end(); itr++)
        if((*itr).compare(x) == 0)
            return true;
    return false;
}

bool findIn(int v[100], int len, int x)
{
    for(int i = 0; i < len; i++)
        if(v[i] == x)
            return true;
    return false;
}

void union2v(int a[100], int& lenA, int b[100], int lenB)
{
    int i = 0;
    int j = 0;
    int k = 0;
    int v[100];
    sort(a, a+lenA);
    sort(b, b+lenB);
    while( i < lenA && j < lenB )
    {
        if( a[i] < b[j] )
        {
            v[k] = a[i];
            k++;
            i++;
        }
        else if( a[i] == b[j] )
        {
            v[k] = a[i];
            k++;
            i++;
            j++;
        }
        else
        {
            v[k] = b[j];
            k++;
            j++;

        }
    }
    while( i < lenA )
    {
        v[k] = a[i];
        k++;
        i++;
    }
    while( j < lenB)
    {
        v[k] = b[j];
        k++;
        j++;
    }

    lenA = k;
    for(int i = 0; i < k; i++)
        a[i] = v[i];
}


void getUnion(int v[100], int len, int unionV[100], int& lenV)
{
    for(int i = 0 ; i < 100; i++)
        unionV[i] = 0;
    lenV = 0;
    for(int i = 0; i < len; i++)
    {
        union2v(unionV, lenV, closures[v[i]].arr, closures[v[i]].len);
    }

}


void eatLetter(int origin, int v[100], int len, string let)
{
    map<int, vector<string>>::iterator itr;
    int statesLet[100];
    int lenLet = 0;
    int unionV[100];
    int lenV = 0;
    int nod;
    for(int i = 0; i < len; i++)
    {
        nod = v[i];
        for(itr = afnL[nod].begin(); itr != afnL[nod].end(); itr++)
        {
            if(findIn(itr->second, let) && !findIn(statesLet, lenLet, itr->first))
            {
                statesLet[lenLet] = itr->first;
                lenLet++;
            }
        }
    }

    afiVect(statesLet, lenLet);   /// afiseaza L -> let
    //getUnion(statesLet, lenLet, unionV, lenV); /// -> L ( reuniune de closure )

    ArrayLen aux;
    aux.copyarr(unionV, lenV);
    afnLT[origin].insert(pair<string, ArrayLen>(let, aux));

}

void bfs(int nod)
{
    len = 0;
    map<int, vector<string>>::iterator itr;
    vector<string>::iterator its;
    lambda_states[len] = nod;
    len++;
    int p = 0;
    while( p < len )
    {
        nod = lambda_states[p];
        for(itr = afnL[nod].begin(); itr != afnL[nod].end(); itr++)
        {
            if( findIn(afnL[nod][itr->first],"lambda") && !findIn(lambda_states, len, itr->first) )
            {
                lambda_states[len] = itr->first;
                len++;
            }
        }
        p++;
    }
    sort(lambda_states, lambda_states + len);

}

void doClosures()
{
    for(int i = 0; i < count_states; i++)
    {
        bfs(i);
        closures[i].nr = i;
        closures[i].copyarr(lambda_states, len);
        closures[i].print();
    }

}

void doAFNlambdaTable()
{
    string let;
    for(int i = 0; i < count_states; i++)
    {
        afnLT.insert(make_pair(i, map<string, ArrayLen>()));
        for(int k = 0; k < count_alfabet; k++)
        {
            let = alfabet[k];
            cout<<endl<< i << " " <<let<<endl;
            eatLetter(i, closures[i].arr, closures[i].len, let);
        }
    }
}


int main()
{
    readData();
    afiAfnL();
    doClosures();
    doAFNlambdaTable();

    /*
    LetterIntList l1;
    l1.ch = "a";
    l1.len = 2;
    l1.arr[0] = 2;
    l1.arr[1] = 5;
    afnLT.insert(pair<int, LetterIntList>(3, l1));
    */

    return 0;
}
