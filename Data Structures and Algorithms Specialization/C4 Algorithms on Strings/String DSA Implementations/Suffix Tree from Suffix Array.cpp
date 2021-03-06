#include <iostream>
#include <string>
#include <vector>
#include <cstring>
#include <algorithm>
#include <unordered_map>
#include <queue>
#define deb(x) cout << #x << " : " << x << '\n'
#define logn(x) cout << x << '\n'
#define logs(x) cout << x << ' '
#define log(x) cout << x
#define MOD 1000000007
#define uInt unsigned long long int
#define Int long long int

using namespace std;

typedef vector<Int> ListInt;

template <typename T, typename U>
void debArr(T arr[], U n)
{
	for (U i = 0; i <= n - 1; i++)
	{
		logs(arr[i]);
	}
	log(endl);
}

template <typename T>
void debVect(vector<T> arr)
{
	for (auto i : arr)
	{
		logs(i);
	}
	log(endl);
}

class SuffixTree
{
  public:
	struct Node
	{
		Int start, end;
		Node *parent;
		vector<Node *> childs;
		Int charsTill;
		Node(Node *p = nullptr, Int s = -1, Int e = -1, Int c = 0)
		{
			parent = p;
			start = s;
			end = e;
			charsTill = c;
			childs.resize(SuffixTree::charSetSize, nullptr);
		}
	};
	Node *root;
	string text;
	string bwt_last;
	string bwt_first;
	ListInt suffArray;
	ListInt LCPA;
	ListInt firstToLast, lastToFirst;
	ListInt firstOccurance;
	vector<ListInt> rankByRow;
	Int len;
	static const char delim = '$';
	static const char base = 'a';
	static const Int charSetSize = 27;
	SuffixTree(const string &s)
	{
		text = s + delim;
		len = text.length();
		suffArray = buildSuffArray();
		LCPA = getLCPA();
		root = buildSuffTree();
		bwt_first = codeBWTfirst();
		bwt_last = codeBWTlast();
		firstOccurance = buildFO();
		firstToLast = buildFTL();
		lastToFirst = buildLTF();
		rankByRow = buildRBR();
	}
	static Int index(const char &c)
	{
		if (c == delim)
			return 0;
		else
			return c - base + 1;
	}
	static bool strSort(const char &c1, const char &c2)
	{
		return index(c1) < index(c2);
	}
	Node *buildSuffTree()
	{
		root = new Node();
		Int lcp = 0;
		Node *curr = root;
		for (Int i = 0; i < len; i++)
		{
			Int suffix = suffArray[i];
			while (curr->charsTill > lcp)
			{
				curr = curr->parent;
			}
			Int offset = lcp - curr->charsTill;
			if (offset == 0)
			{
				curr = createNewLeaf(curr, suffix);
			}
			else
			{
				Int eStart = suffArray[i - 1] + curr->charsTill;
				Node *midNode = breakEdge(curr, eStart, offset);
				curr = createNewLeaf(midNode, suffix);
			}
			if (i < len - 1)
				lcp = LCPA[i];
		}
		return root;
	}
	Node *breakEdge(Node *par, const Int &start, const Int &offset)
	{
		Int startChar = index(text[start]);
		Int midChar = index(text[start + offset]);
		Node *mid = new Node(par);
		mid->start = start;
		mid->end = start + offset - 1;
		mid->charsTill = par->charsTill + offset;
		mid->childs[midChar] = par->childs[startChar];
		par->childs[startChar]->parent = mid;
		par->childs[startChar]->start += offset;
		par->childs[startChar] = mid;
		return mid;
	}
	Node *createNewLeaf(Node *par, const Int &suffix)
	{
		Node *t = new Node(par);
		t->start = suffix + par->charsTill;
		t->end = len - 1;
		t->charsTill = len - suffix;
		Int indx = index(text[t->start]);
		par->childs[indx] = t;
		return par;
	}
	ListInt buildSuffArray()
	{
		// Time Complexity O(nLOGn) using Couting Sort
		ListInt order = sortChars();
		ListInt eqvClass = initEqvClass(order);
		Int L = 1;
		while (L < len)
		{
			order = sortDoubled(order, eqvClass, L);
			eqvClass = updateEqvClass(order, eqvClass, L);
			L *= 2;
		}
		return order;
	}
	ListInt sortChars()
	{
		ListInt order(len, 0), count(charSetSize, 0);
		for (Int i = 0; i < len; i++)
			count[index(text[i])]++;
		for (Int i = 1; i < charSetSize; i++)
			count[i] += count[i - 1];
		for (Int i = len - 1; i >= 0; i--)
		{
			Int indx = index(text[i]);
			count[indx]--;
			order[count[indx]] = i;
		}
		return order;
	}
	ListInt initEqvClass(const ListInt &order)
	{
		ListInt eqvClass(len, 0);
		eqvClass[order[0]] = 0;
		for (Int i = 1; i < len; i++)
		{
			Int curr = order[i];
			Int prev = order[i - 1];
			if (text[curr] == text[prev])
				eqvClass[curr] = eqvClass[prev];
			else
				eqvClass[curr] = eqvClass[prev] + 1;
		}
		return eqvClass;
	}
	ListInt sortDoubled(const ListInt &order, const ListInt &eClass, const Int &L)
	{
		ListInt newOrder(len, 0), count(len, 0);
		for (Int i = 0; i < len; i++)
			count[eClass[i]]++;
		for (Int i = 1; i < len; i++)
			count[i] += count[i - 1];
		for (Int i = len - 1; i >= 0; i--)
		{
			Int start = (order[i] - L + len) % len;
			Int cl = eClass[start];
			count[cl]--;
			newOrder[count[cl]] = start;
		}
		return newOrder;
	}
	ListInt updateEqvClass(const ListInt &newOrder, const ListInt &eClass, const Int &L)
	{
		ListInt updClass(len, 0);
		updClass[newOrder[0]] = 0;
		for (Int i = 1; i < len; i++)
		{
			Int curr = newOrder[i], prev = newOrder[i - 1];
			Int midCurr = (curr + L) % len, midPrev = (prev + L) % len;
			if (eClass[curr] == eClass[prev] and eClass[midCurr] == eClass[midPrev])
				updClass[curr] = updClass[prev];
			else
				updClass[curr] = updClass[prev] + 1;
		}
		return updClass;
	}
	ListInt getLCPA()
	{
		// Kasai Algorithm
		// Time Complexity O(n)
		ListInt lcp(len - 1, 0);
		ListInt invSA(len, 0);
		// Invert Suffix Array
		for (Int i = 0; i < len; i++)
			invSA[suffArray[i]] = i;
		Int k = 0;
		for (Int i = 0; i < len; i++)
		{
			if (invSA[i] == len - 1)
			{
				k = 0;
				continue;
			}
			Int j = suffArray[invSA[i] + 1];
			while ((i + k < len and j + k < len) and (text[i + k] == text[j + k]))
				k++;
			lcp[invSA[i]] = k;
			k = k > 0 ? --k : 0;
		}
		return lcp;
	}
	vector<ListInt> buildRBR()
	{
		vector<ListInt> rbr;
		rbr.resize(len);
		rbr[0] = ListInt(charSetSize, 0);
		Int ind = index(bwt_last[0]);
		rbr[0][ind]++;
		for (Int i = 1; i < len; i++)
		{
			rbr[i] = rbr[i - 1];
			ind = index(bwt_last[i]);
			rbr[i][ind]++;
		}
		return rbr;
	}
	ListInt buildFTL()
	{
		ListInt ftl(len, -1);
		unordered_map<char, queue<Int>> m;
		for (Int i = 0; i < len; i++)
		{
			m[bwt_last[i]].push(i);
		}
		for (Int i = 0; i < len; i++)
		{
			queue<Int> &q = m[bwt_first[i]];
			ftl[i] = q.front();
			q.pop();
		}
		return ftl;
	}
	ListInt buildLTF()
	{
		ListInt ltf(len, -1);
		unordered_map<char, queue<Int>> m;
		for (Int i = 0; i < len; i++)
		{
			m[bwt_first[i]].push(i);
		}
		for (Int i = 0; i < len; i++)
		{
			queue<Int> &q = m[bwt_last[i]];
			ltf[i] = q.front();
			q.pop();
		}
		return ltf;
	}
	ListInt buildFO()
	{
		// calculate First Occurance in BWT 1st Column
		ListInt fo(charSetSize, -1);
		for (Int i = 0; i < len; i++)
		{
			Int ind = index(bwt_first[i]);
			if (fo[ind] == -1)
				fo[ind] = i;
		}
		return fo;
	}
	// to get Burrows–Wheeler Transform of String
	string codeBWTlast()
	{
		char code[len + 1];
		code[len] = '\0';

		// for bwt_last
		for (Int i = 0; i < len; i++)
		{
			Int ind = suffArray[i] - 1;
			code[i] = text[(ind + len) % len];
		}
		return string(code);
	}
	string codeBWTfirst()
	{
		char code[len + 1];
		code[len] = '\0';

		// for bwt_first
		for (Int i = 0; i < len; i++)
		{
			Int ind = suffArray[i];
			code[i] = text[ind];
		}
		return string(code);
	}
	// to decode Burrows–Wheeler Transform of string
	static string decodeBWT(const string &L)
	{
		Int n = L.length();
		string F = L;
		sort(F.begin(), F.end(), SuffixTree::strSort);
		unordered_map<char, queue<Int>> m;
		for (Int i = 0; i < n; i++)
		{
			m[L[i]].push(i);
		}
		Int ftl[n];
		for (Int i = 0; i < n; i++)
		{
			queue<Int> &q = m[F[i]];
			ftl[i] = q.front();
			q.pop();
		}
		char decode[n + 1];
		decode[n] = '\0';
		Int x = 0;
		for (Int i = 0; i < n; i++)
		{
			x = ftl[x];
			decode[i] = F[x];
		}
		return string(decode);
	}
	ListInt getOccurances(const string &p)
	{
		Int i = p.length() - 1;
		Int top = 0, bottom = len - 1;
		ListInt occur;
		while (top <= bottom and (top >= 0 and bottom >= 0))
		{
			if (i < 0)
			{
				occur.reserve(bottom - top + 1);
				for (Int i = top; i <= bottom; i++)
					occur.push_back(suffArray[i]);
				break;
			}
			else
			{
				Int ind = index(p[i--]);
				if (top - 1 >= 0)
					top = firstOccurance[ind] + rankByRow[top - 1][ind];
				else
					top = firstOccurance[ind];
				bottom = firstOccurance[ind] + rankByRow[bottom][ind] - 1;
			}
		}
		return occur;
	}
	void showContents(Node *p)
	{
		if (p)
		{
			if (p->start >= 0 and p->end >= 0)
				logn(text.substr(p->start, p->end - p->start + 1));
			for (auto i : p->childs)
			{
				if (i != nullptr)
					showContents(i);
			}
		}
	}
	void showContents()
	{
		showContents(root);
	}
};

int main()
{
	ios_base::sync_with_stdio(false);
	cin.tie(NULL);

	string s = "ababaa";
	SuffixTree st(s);
	debVect(st.LCPA);
	st.showContents();

	return 0;
}
