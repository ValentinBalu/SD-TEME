#include <iostream>
#include <list>
#include <fstream>

using namespace std;

ifstream in("date.txt");
ofstream out("rezultat.txt");

struct Nod
{
	int data, rang;
	Nod* copil, * frate, * parinte;
};

class BHeap
{
	list<Nod*> heap;
public:
	Nod* nodNou(int k)
	{
		Nod* x = new Nod;
		x->data = k;
		x->copil = x->frate = x->parinte = NULL;
		return x;
	}
	Nod* mergeArbori(Nod* , Nod* );
	list<Nod*> mergeHeapuri(list<Nod*>& , list<Nod*>& );
	void rearanjareHeap();
	void insertArbore(Nod* );
	void insert(int);
	list<Nod*> deleteMinimArbore(Nod*);
	Nod* getMin();
	void delMin();
	Nod* gasireNod(Nod*, int );
	void scadereValoare(int, int);
	void delValoare(int);
	void printArbore(Nod*);
	void printHeap();
};
//Reuniune dintre doi arbori binomiali
Nod* BHeap::mergeArbori(Nod* b1, Nod* b2)
{
	if (b1->data > b2->data)
		swap(b1, b2);

	b2->parinte = b1;
	b2->frate = b1->copil;
	b1->copil = b2;
	b1->rang++;

	return b1;
}
//Reuniune dintre doua heapuri
list<Nod*> BHeap::mergeHeapuri(list<Nod*>& h1, list<Nod*>& h2)
{
	list<Nod*> hnou;
	list<Nod*>::iterator i = h1.begin();
	list<Nod*>::iterator j = h2.begin();
	while (i != h1.end() && j != h2.end())
	{
		if ((*i)->rang <= (*j)->rang)
		{
			hnou.push_back(*i);
			i++;
		}
		else
		{
			hnou.push_back(*j);
			j++;
		}
	}
	//daca mai ramane ceva in h1
	while (i != h1.end())
	{
		hnou.push_back(*i);
		i++;
	}
	//daca mai ramane ceva in h2
	while (j != h2.end())
	{
		hnou.push_back(*j);
		j++;
	}
	return hnou;
}
//Rearanjeaza heapul astfel incat sa se respecte regulile
void BHeap::rearanjareHeap()
{
	if (heap.size() <= 1)
		return;
	list<Nod*>hnou;
	list<Nod*>::iterator i, j, k;
	i = j = k = heap.begin();

	if (heap.size() == 2)
	{
		j = i;
		j++;
		k = heap.end();
	}
	else
	{
		j++;
		k = j;
		k++;
	}
	while (i != heap.end())
	{
		if (j == heap.end())
			i++;
		//daca totul e in ordine continuam
		else if ((*i)->rang < (*j)->rang)
		{
			i++;
			j++;
			if (k != heap.end())
				k++;
		}

		else if (k != heap.end() && (*i)->rang == (*j)->rang && (*i)->rang == (*k)->rang)
		{
			i++;
			j++;
			k++;
		}
		else if ((*i)->rang == (*j)->rang)
		{
			*i = mergeArbori(*i, *j);
			j = heap.erase(j);
			if (k != heap.end())
				k++;
		}
	}

}
//insert arbore in heap
void BHeap::insertArbore(Nod* a)
{
	list<Nod*>hnou;
	hnou.push_back(a);
	heap = mergeHeapuri(heap, hnou);
	rearanjareHeap();
}
//insert valoare in heap
void BHeap::insert(int k)
{
	Nod* nnou = nodNou(k);
	insertArbore(nnou);
}
//stergere minim din heap
//stergere minim  element din arbore
list<Nod*> BHeap::deleteMinimArbore(Nod* A)
{
	list<Nod*> hnou;
	Nod* nnou = A->copil;
	Nod* x;
	while (nnou)
	{
		x = nnou;
		nnou = nnou->frate;
		x->frate = NULL;
		hnou.push_front(x);
	}
	return hnou;
}
//returneaza pointerul Nodului minim din heap
Nod* BHeap::getMin()
{
	list<Nod*>::iterator i = heap.begin();
	Nod* nnou = *i;
	while (i != heap.end())
	{
		if ((*i)->data < nnou->data)
			nnou = *i;
		i++;
	}
	return nnou;
}
//stergere min din heap
void BHeap::delMin()
{
	list<Nod*> hnou, x;
	Nod* nmin;
	nmin = getMin();
	list<Nod*>::iterator i;
	i = heap.begin();
	while (i != heap.end())
	{
		if (*i != nmin)
		{
			hnou.push_back(*i);
		}
		i++;
	}
	x = deleteMinimArbore(nmin);
	heap = mergeHeapuri(hnou, x);
	rearanjareHeap();
}
//functie pentru gasire element(folosita pentru stergere element)
Nod* BHeap::gasireNod(Nod* n, int val)
{
	if (n == NULL)
		return NULL;
	if (n->data == val)
		return n;
	Nod* rez = gasireNod(n->copil, val);
	if (rez != NULL)
		return rez;
	return gasireNod(n->frate, val);
}
void BHeap::scadereValoare(int oval, int nval)
{
	list<Nod*>::iterator i = heap.begin();
	Nod* n = NULL;
	while (i != heap.end() && n == NULL)
	{
		n = gasireNod(*i, oval);
		i++;
	}
	if (n == NULL)
		return;
	n->data = nval;
	Nod* parinte = n->parinte;
	while (parinte != NULL && n->data < parinte->data)
	{
		swap(n->data, parinte->data);
		n = parinte;
		parinte = parinte->parinte;
	}
}
//stergere valoare
void BHeap::delValoare(int val)
{
	scadereValoare(val, -1);

	delMin();
}

int main()
{
	BHeap H;
	int n;
	in >> n;
	while (n != 0)
	{
		int i;
		in >> i;
		if (i == 1)
		{
			int v;
			in >> v;
			H.insert(v);
			out << "s-a inserat valoarea " << v<<"\n";
		}
		if (i == 2)
		{
			int v;
			in >> v;
			H.delValoare(v);
			out << "s-a sters valoarea " << v<<"\n";
		}
		if (i == 3)
		{
			Nod* min;
			min = H.getMin();
			out << "minimul din heap este: " << min->data << "\n";
		}
		if (i == 4)
		{
			H.delMin();
			out << "s-a sters minimul din heap\n";
		}
		n--;
	}

	return 0;
}


