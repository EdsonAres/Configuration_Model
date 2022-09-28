#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <iostream>
#include<fstream>
#include <algorithm>// função find
#include <vector>
#include <cstdlib>
#include <iomanip>
#include<iterator>
#include <string>
#include <list>
#include <stack> // pilha para usar na DFS
#include <queue> // fila para usar na BFS
float ran1(long *idum);
using namespace std;
#define nv 13//numero de nodes da rede
long int iseed= -1024;
vector<bool> visited(nv, false);
vector<int> lista_nt;
void Carrega_Conectividade();
void Constroi_Simples(int nt);
void Constroi_Multigrafo(int nt);




class Grafo
{
	int V; // número de vértices
	list<int> *adj; // ponteiro para um array contendo as listas de adjacências

public:
	Grafo(int V); // construtor
	void adicionarAresta(int v1, int v2); // adiciona uma aresta no grafo
	bool existeVizinho(int v1, int v2); // verifica se v2 é vizinho de v1
    void apagaListas();//zera todas as listas de adjacencias
    bool bfs(int v, int sourced); // verifica se existe um caminho entre v e sourced


};
Grafo::Grafo(int V)
{
	this->V = V; // atribui o número de vértices
	adj = new list<int>[V]; // cria as listas
}
float displayAdjList(list<int> adj_list[]) //exibe as listas
{
   for(int i = 0; i<nv; i++) {
      cout << i << "--->";
      list<int> :: iterator it;
      for(it = adj_list[i].begin(); it != adj_list[i].end(); ++it) {
         cout << *it << " ";
      }
      cout << endl;
   } 
}
void Grafo::adicionarAresta(int v1, int v2)
{
	// adiciona vértice v2 à lista de vértices adjacentes de v1
	adj[v1].push_back(v2);
    if(v1!=v2){adj[v2].push_back(v1);}
    displayAdjList(adj);
}
void Grafo::apagaListas(){
    for(int i = 0; i<nv; i++) {
        adj[i].clear();
    } 
}
bool Grafo::existeVizinho(int v1, int v2)
{
    if(adj[v1].size()<(adj[v2].size())){
	    if(find(adj[v1].begin(), adj[v1].end(), v2) != adj[v1].end()){
		    return true;}
    }
    else{
	    if(find(adj[v2].begin(), adj[v2].end(), v1) != adj[v2].end()){
		    return true;
        }
    }
	return false;
}
bool Grafo::bfs(int v, int sourced)
{
	queue<int> fila;
	bool visitados[V]; // vetor de visitados

	for(int i = 0; i < V; i++)
		visitados[i] = false;

	//cout << "Visitando vertice " << v << " ...\n";
	visitados[v] = true; // marca como visitado

	while(true)
	{
		list<int>::iterator it;
		for(it = adj[v].begin(); it != adj[v].end(); it++)
		{
			if(!visitados[*it])
			{
				//cout << "Visitando vertice " << *it << " ...\n";
				visitados[*it] = true; // marca como visitado
				if (*it==sourced)
				{
					return(true);
				}
				
				fila.push(*it); // insere na fila
			}
		}

		// verifica se a fila NÃO está vazia
		if(!fila.empty())
		{
			v = fila.front(); // obtém o primeiro elemento
			fila.pop(); // remove da fila
		}
		else
			return(false);
	}
}



int main()
{  	//variaveis
	
	Carrega_Conectividade();
	int nt=lista_nt.size();//tamanho da lista
	//a variavel clasgraf serve apenas para dizer se será produzido um grafo simples ou multigrafo
	int clasgraf=2; //escolha 1 para grafo simples e 2 para multigrafo
	if (clasgraf==1){Constroi_Simples(nt);}
    else{Constroi_Multigrafo(nt);}
    return 0;
}





void Carrega_Conectividade(){
	//criando/carregando a lista Nt onde o node i de grau ki entra ki vezes
    ifstream arquivo ("test_data.txt");
    int a;
    while (!arquivo.eof())
    {   
        arquivo >> a;
        lista_nt.push_back(a);
    }
	arquivo.close();
    lista_nt.pop_back();
}
void Constroi_Simples(int nt){
	Grafo grafo(nv);
	int k1, k2,s1,s2;
	int tentativa=0;
		while (nt>0)
		{ 	
			k1=nt*ran1(&iseed);
			k2=nt*ran1(&iseed);
			s1=lista_nt[k1];
			s2=lista_nt[k2];
			if(nt==2){k1=0;k2=1;}
			while (s1==s2)
			{
				k1=nt*ran1(&iseed);
				k2=nt*ran1(&iseed);
				s1=lista_nt[k1];
				s2=lista_nt[k2];
			}
			tentativa++;
			if (grafo.existeVizinho(s1,s2)==false){//garante q não são vizinho
						grafo.adicionarAresta(s1,s2);
						lista_nt.erase(lista_nt.begin()+k1);
						if(k2>k1){lista_nt.erase(lista_nt.begin()+k2-1);}//-1 pq a linha acima diminui em 1 o tamnaho da lista
						else {lista_nt.erase(lista_nt.begin()+k2);}//se k2 esta atrás k1
            			nt=nt-2;
						tentativa=0;
			}
			if(tentativa>1000){
				tentativa=0;
				grafo.apagaListas();
				lista_nt.clear();
				Carrega_Conectividade();
				nt=lista_nt.size();
			}
		}
}
void Constroi_Multigrafo(int nt){
	Grafo grafo(nv);
	int k1, k2,s1,s2;
	while (nt>0){ 
		k1=nt*ran1(&iseed);
		k2=nt*ran1(&iseed);
		if(nt==2){k1=0;k2=1;}
		while (k1==k2)
			{
				k1=nt*ran1(&iseed);
				k2=nt*ran1(&iseed);
			}
		s1=lista_nt[k1];
		s2=lista_nt[k2];
		grafo.adicionarAresta(s1,s2);
		lista_nt.erase(lista_nt.begin()+k1);
		if(k2>k1){lista_nt.erase(lista_nt.begin()+k2-1);}//-1 pq a linha acima diminui em 1 o tamnaho da lista
		else {lista_nt.erase(lista_nt.begin()+k2);}//se k2 esta atrás k1
		nt=nt-2;
	}
}


















//---------------------------------------------------------------------------------------------------------------------
#define IA 16807
#define IM 2147483647
#define AM (1.0/IM)
#define IQ 127773
#define IR 2836
#define NTAB 32
#define NDIV (1+(IM-1)/NTAB)
#define EPS 1.2e-7
#define RNMX (1.0-EPS)

float ran1(long *idum)
{
	int j;
	long k;
	static long iy=0;
	static long iv[NTAB];
	float temp;

	if (*idum <= 0 || !iy) {
		if (-(*idum) < 1) *idum=1;
		else *idum = -(*idum);
		for (j=NTAB+7;j>=0;j--) {
			k=(*idum)/IQ;
			*idum=IA*(*idum-k*IQ)-IR*k;
			if (*idum < 0) *idum += IM;
			if (j < NTAB) iv[j] = *idum;
		}
		iy=iv[0];
	}
	k=(*idum)/IQ;
	*idum=IA*(*idum-k*IQ)-IR*k;
	if (*idum < 0) *idum += IM;
	j=iy/NDIV;
	iy=iv[j];
	iv[j] = *idum;
	if ((temp=AM*iy) > RNMX) return RNMX;
	else return temp;
}
#undef IA
#undef IM
#undef AM
#undef IQ
#undef IR
#undef NTAB
#undef NDIV
#undef EPS
#undef RNMX

//---------------------------------------------------------------------------------------------------------------
