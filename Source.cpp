#include <iostream>
#include <vector>
#include <queue>
#include <fstream>
#include <string>
using namespace std;

struct fVal {
	ifstream rdr;
	int val;
};

class comparator {
public:
	bool operator()(int& a, int& b)
	{
		return a > b;
	}
	bool operator()(fVal* a, fVal* b)
	{
		return a->val > b->val;
	}
};

#pragma region Utility Functions
void createfile(int size_b,string filename)
{
	ofstream wrt(filename);
	for (int i = 0; i < size_b; i++)
	{
		wrt << i + (rand() % (100+i));
		if (i % 25 == 0 && i != 0)
			wrt << endl;
		else
			wrt << " ";
	}
	wrt.close();
	cout << "File Created\n";
}
void loadfile(vector<int>& V, string filename)
{
	ifstream rdr(filename);
	if (!rdr.is_open())
		return;

	while (rdr)
	{
		int val;
		rdr >> val;
		if (rdr)
			V.push_back(val);
	}
	cout << "File Loaded\n";
}
void storefile(vector<int>& V, string filename)
{
	ofstream wrt(filename);
	for (int i = 0; i < V.size(); i++)
	{
		wrt << V[i];
		if (i % 25 == 0 && i!=0)
			wrt << endl;
		else
			wrt << " ";
	}
	cout << "File Stored\n";
	wrt.close();
}

void randomized_init(vector<int>& V,int size)
{
	int val;
	for (int i = 0; i < size; i++)
		val = rand() % 100, V.push_back(val);

}
ostream& operator<<(ostream& cout, vector<int> V)
{
	for (int x : V)
		cout << x << " ";
	return cout;
}
bool bubbleSortSwap(vector<int>& V)
{
	bool changeHappened = false;
	for (int i = 0; i < V.size() -1; i++)
	{
		if (V[i] > V[i + 1])
			swap(V[i], V[i + 1]), changeHappened = true;
	}
	return changeHappened;
}
int findMinInRange(vector<int>V, int si, int ei)
{
	int mi = si;
	for (int i = si+1; i <= ei; i++)
	{
		if (V[i] < V[mi])
			mi = i;
	}
	return mi;
}

vector<int> merge(vector<int>& A, vector<int>& B)
{
	vector<int> ret;
	int ai, bi;
	ai = bi = 0;

	while (ai < A.size() && bi < B.size())
	{
		if (A[ai] > B[bi])
			ret.push_back(B[bi]), bi++;
		else
			ret.push_back(A[ai]), ai++;

	}
	while (ai < A.size())
	{
		ret.push_back(A[ai]);
		ai++;
	}
	while (bi < B.size())
	{
		ret.push_back(B[bi]);
		bi++;
	}

	return ret;
}

int createSubFiles(string filename, int filesize_b, int ram_b)
{
	ifstream rdr(filename);
	vector<int> loadvector;
	int i = 1;
	string subfilename;
	while (!rdr.eof())
	{
		int val;
		rdr >> val;
		loadvector.push_back(val);
		if (loadvector.size() == ram_b)
		{
			subfilename = filename + "_sub_" + to_string(i);
			sort(loadvector.begin(), loadvector.end());
			storefile(loadvector, subfilename);
			loadvector.clear();
			i++;
		}
	}
	rdr.close();
	return filesize_b / ram_b;


}
priority_queue<fVal*, vector<fVal*>,comparator> openSubFiles(string filename, int nof)
{
	priority_queue<fVal*, vector<fVal*>, comparator> pque;
	fVal* ptr;
	for (int i = 1; i <= nof; i++)
	{
		string subfilename = filename + "_sub_" + to_string(i);
		ptr = new fVal;
		ptr->rdr.open(subfilename);
		ptr->rdr >> ptr->val;
		pque.push(ptr);
	}

	return pque;
}

#pragma endregion

#pragma region Main Sorting Functions
template <class T>
void HeapSort(vector<T>& V)
{
	priority_queue<int, vector<int>, comparator> pque;

	for (int& x : V)
		pque.push(x);

	V.clear();

	while (!pque.empty())
	{
		V.push_back(pque.top());
		pque.pop();
	}

}
template <class T>
void BubbleSort(vector<T>& V)
{
	while (bubbleSortSwap(V)) {}
}
template <class T>
void SelectionSort(vector<T>& V)
{
	for (int i = 0; i < V.size(); i++)
	{
		int mi = findMinInRange(V, i, V.size() - 1);
		swap(V[i], V[mi]);
	}
}

int Partition(vector<int>& A, int si, int ei)
{
	int pivot = ei;
	int retpi = si - 1;
	for (int i = si; i < ei; i++)
	{
		if (A[i] <= A[pivot]) {
			retpi++;
			swap(A[retpi], A[i]);
		}
	}
	swap(A[retpi + 1], A[pivot]);
	return retpi + 1;
}
template <class T>
void QuickSortRec(vector<T>& V, int si, int ei)
{
	if (si >= ei)
		return;

	int pi = Partition(V, si, ei);
	QuickSortRec(V, si, pi - 1);
	QuickSortRec(V, pi + 1, ei);
}
template <class T>
void QuickSort(vector<T>& V)
{
	QuickSortRec(V, 0, V.size() - 1);
}


template <class T>
void MergeSort(vector<T>& V)
{
	if (V.size() < 2)
		return;

	vector<T> L(V.begin(), V.begin() + V.size() / 2);
	vector<T> R(V.begin() + (V.size() / 2), V.end());
	MergeSort(L);
	MergeSort(R);
	V = merge(R, L);
}

void DiskSort(priority_queue<fVal*, vector<fVal*>, comparator>& pq, string out_filename)
{
	ofstream wrt(out_filename);
	fVal* tptr;
	int i = 0;
	while (!pq.empty())
	{
		tptr = pq.top();
		pq.pop();
		wrt << tptr->val;
		i++;
		if (i % 25 == 0 && i != 0)
			wrt << endl;
		else
			wrt << " ";
		tptr->rdr >> tptr->val;
		if (!tptr->rdr.eof())
			pq.push(tptr);
	}
}

#pragma endregion



void task_1()
{
	priority_queue<int> pque;
	pque.push(20);
	pque.push(30);
	pque.push(40);
	pque.push(10);
	pque.push(5);

	while (!pque.empty())
	{
		cout << pque.top() << " ";
		pque.pop();
	}
}
void task_2()
{
	priority_queue<int,vector<int>,comparator> pque;
	pque.push(20);
	pque.push(30);
	pque.push(40);
	pque.push(10);
	pque.push(5);

	while (!pque.empty())
	{
		cout << pque.top() << " ";
		pque.pop();
	}
}

void SelectFile(string& filename, int& filesize)
{
	int opt;

	do
	{
		system("cls");
		cout << "Select One: \n"
			<< "1.\tKiloByte File\n"
			<< "2.\tMegaByte File\n"
			<< "3.\tGigaByte File\n"
			<< ">";
		cin >> opt;
	} while (opt > 3 && opt < 1);

	switch (opt)
	{
	case 1:
		filename = "kbfile";
		filesize = 1000;
		break;
	case 2:
		filename = "mbfile";
		filesize = 1000 * 1000;
		break;
	case 3:
		filename = "gbfile";
		filesize = 1000 * 1000 * 1000;
		break;
	}
}



int main()
{


	//createfile(1000*1000, "mbfile");
	vector<int> vec;
	priority_queue<fVal*, vector<fVal*>, comparator> pq;
	int nof, filesize;
	string filename;
	SelectFile(filename, filesize);
	loadfile(vec, filename);

	int opt;

	do
	{
		system("cls");
		cout << "Select One: \n"
			<< "1.\tHeapSort\n"
			<< "2.\tQuickSort\n"
			<< "3.\tMergeSort\n"
			<< "4.\tBubbleSort\n"
			<< "5.\tSelectionSort\n"
			<< "6.\tDiskSort\n"
			<< ">";
		cin >> opt;
	} while (opt > 6 && opt < 1);

	int timestart = time(0);
	switch (opt)
	{
	case 1:
		HeapSort(vec);
		break;
	case 2:
		QuickSort(vec);
		break;
	case 3:
		MergeSort(vec);
		break;
	case 4:
		BubbleSort(vec);
		break;
	case 5:
		SelectionSort(vec);
		break;
	case 6:
		nof = createSubFiles(filename, filesize, filesize / 10);
		pq = openSubFiles(filename, nof);
		DiskSort(pq, filename + "_ans");
		break;
	default:
		return 0;
	}
	cout << "Time Taken: " << time(0) - timestart << " sec(s)" << endl;

	if(opt != 6)
		storefile(vec, filename+"_ans");
}