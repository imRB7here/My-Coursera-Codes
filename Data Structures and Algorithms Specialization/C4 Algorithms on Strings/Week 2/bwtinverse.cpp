#include <iostream>
#include <string>
#include <vector>
#include <cstring>
#include <algorithm>
#include <unordered_map>
#include <queue>
#include <cassert>
#include <exception>
#include <stdexcept>
#define deb(x) cout << #x << " : " << x << '\n'
#define logn(x) cout << x << '\n'
#define logs(x) cout << x << ' '
#define log(x) cout << x
#define MOD 1000000007
#define uInt unsigned long long int
#define Int int

using namespace std;

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

class SuffixArray
{
  public:
	struct Suffix
	{
		Int indx, rank1, rank2;
		Suffix()
		{
			indx = rank1 = rank2 = -1;
		}
		static bool compare(const Suffix &s1, const Suffix &s2)
		{
			if (s1.rank1 == s2.rank1)
				return s1.rank2 < s2.rank2;
			else
				return s1.rank1 < s2.rank1;
		}
	};
	string text;
	vector<Int> suffArray;
	Int len;
	static const char delim = '$';
	static const char base = 'A';
	SuffixArray(const string &s)
	{
		text = s;
		len = text.length();
		suffArray.resize(len, -1);
		buildSuffArray();
	}
	static Int index(const char &c)
	{
		switch (c)
		{
		case delim:
			return 0;
		case 'A':
			return 1;
		case 'C':
			return 2;
		case 'G':
			return 3;
		case 'T':
			return 4;
		}
		return -1;
	}
	void buildSuffArray()
	{
		vector<Suffix> v(len);
		for (Int i = 0; i < len; i++)
		{
			v[i].indx = i;
			v[i].rank1 = index(text[i]);
			if (i + 1 < len)
				v[i].rank2 = index(text[i + 1]);
			else
				v[i].rank2 = -1;
		}
		sort(v.begin(), v.end(), Suffix::compare);
		Int corrIndx[len];
		for (Int k = 4; k < 2 * (len); k *= 2)
		{
			Int rank = 0;
			Int prevR1 = v[0].rank1;
			v[0].rank1 = rank;
			corrIndx[v[0].indx] = 0;
			for (Int i = 1; i < len; i++)
			{
				if (v[i].rank1 == prevR1 and v[i].rank2 == v[i - 1].rank2)
				{
					v[i].rank1 = rank;
				}
				else
				{
					prevR1 = v[i].rank1;
					v[i].rank1 = ++rank;
				}
				corrIndx[v[i].indx] = i;
			}
			for (Int i = 0; i < len; i++)
			{
				Int nextIndx = v[i].indx + k / 2;
				if (nextIndx < len)
					v[i].rank2 = v[corrIndx[nextIndx]].rank1;
				else
					v[i].rank2 = -1;
			}
			sort(v.begin(), v.end(), Suffix::compare);
		}
		for (Int i = 0; i < len; i++)
		{
			suffArray[i] = v[i].indx;
		}
	}
	string getBWT()
	{
		char bwt[len + 1];
		for (Int i = 0; i < len; i++)
		{
			Int ind = suffArray[i] - 1;
			if (ind >= 0)
				bwt[i] = text[ind];
			else
				bwt[i] = text[len - 1];
		}
		bwt[len] = '\0';
		return string(bwt);
	}
	static bool strSort(const char &c1, const char &c2)
	{
		return index(c1) < index(c2);
	}
	static string decodeBWT(const string &L)
	{
		Int n = L.length();
		string F = L;
		sort(F.begin(), F.end(), SuffixArray::strSort);
		unordered_map<char, queue<Int>> m;
		for (Int i = 0; i < n; i++)
		{
			m[L[i]].push(i);
		}
		Int shifts[n];
		for (Int i = 0; i < n; i++)
		{
			queue<Int> &q = m[F[i]];
			assert(!q.empty());
			shifts[i] = q.front();
			q.pop();
		}
		char decode[n + 1];
		decode[n] = '\0';
		Int x = 0;
		for (Int i = 0; i < n; i++)
		{
			x = shifts[x];
			decode[i] = F[x];
		}
		return string(decode);
	}
};

int main()
{
	ios_base::sync_with_stdio(false);
	cin.tie(NULL);

	string s;
	cin >> s;
	logn(SuffixArray::decodeBWT(s));
	return 0;
}

/*
LINK : 
*/