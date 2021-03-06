#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <algorithm>
#define deb(x) cout << #x << " : " << x << '\n'
#define logn(x) cout << x << '\n'
#define logs(x) cout << x << ' '
#define log(x) cout << x
#define MOD 1000000007
#define uInt unsigned long long int
#define Int long long int

using namespace std;

typedef pair<Int, Int> HashPair;

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

class HashString
{
  public:
    static const Int mod1 = 1e9 + 7;
    static const Int mod2 = 1e9 + 123;
    static const Int x = 31;
    static vector<Int> pow1, pow2;
    static Int mxPow;
    string str;
    Int len;
    vector<Int> hash1, hash2;
    HashString(const string &s)
    {
        str = s;
        len = s.length();
        hash1.resize(len + 1);
        hash2.resize(len + 2);
        calcHashes();
    }
    void calcHashes()
    {
        hash1[0] = 0;
        hash2[0] = 0;
        for (Int i = 1; i <= len; i++)
        {
            hash1[i] = ((hash1[i - 1] * x) % mod1 + int(str[i - 1] - 'a' + 1)) % mod1;
            if (hash1[i] < 0)
                hash1[i] += mod1;
            hash2[i] = ((hash2[i - 1] * x) % mod2 + int(str[i - 1] - 'a' + 1)) % mod2;
            if (hash2[i] < 0)
                hash2[i] += mod2;
        }
    }
    HashPair getHashPair(Int i = -1, Int j = -1)
    {
        if (i == -1)
            i = 1;
        if (j == -1)
            j = len;
        HashPair h;
        h.first = (hash1[j] - (hash1[i - 1] * pow1[j - i + 1]) % mod1) % mod1;
        h.second = (hash2[j] - (hash2[i - 1] * pow2[j - i + 1]) % mod2) % mod2;
        if (h.first < 0)
            h.first += mod1;
        if (h.second < 0)
            h.second += mod2;
        return h;
    }
    Int getHash1(Int i = -1, Int j = -1)
    {
        if (i == -1)
            i = 1;
        if (j == -1)
            j = len;
        Int retHash = (hash1[j] - (hash1[i - 1] * pow1[j - i + 1]) % mod1) % mod1;
        if (retHash < 0)
            retHash += mod1;
    }
    Int getHash2(Int i = -1, Int j = -1)
    {
        if (i == -1)
            i = 1;
        if (j == -1)
            j = len;
        Int retHash = (hash2[j] - (hash2[i - 1] * pow2[j - i + 1]) % mod2) % mod2;
        if (retHash < 0)
            retHash += mod2;
        return retHash;
    }
    static void calcPows(const Int &n)
    {
        mxPow = n;
        pow1.resize(n + 1);
        pow2.resize(n + 1);
        pow1[0] = pow2[0] = 1;
        for (Int i = 1; i <= n; i++)
        {
            pow1[i] = (pow1[i - 1] * x) % mod1;
            if (pow1[i] < 0)
                pow1[i] += mod1;
            pow2[i] = (pow2[i - 1] * x) % mod2;
            if (pow2[i] < 0)
                pow2[i] += mod2;
        }
    }
    static bool hashPairEqual(const HashPair &a, const HashPair &b)
    {
        return (a.first == b.first) and (a.second == b.second);
    }
    static bool compareHashPair(const HashPair &h1, const HashPair &h2)
    {
        return h1.first < h2.first;
    }
};
Int HashString::mxPow = 0;
vector<Int> HashString::pow1;
vector<Int> HashString::pow2;

void process(const string &s1, const string &s2)
{
    HashString h1(s1), h2(s2);
    Int n = s1.length(), m = s2.length();
    Int low = 0, high = min(n, m) - 1;
    Int pos = -1;
    while (low <= high)
    {
        Int mLen = low + (high - low) / 2;
        vector<HashPair> hashes;
        for (Int i = 0; i + mLen <= n - 1; i++)
            hashes.push_back(h1.getHashPair(i + 1, i + mLen + 1));
        sort(hashes.begin(), hashes.end(), HashString::compareHashPair);
        Int p = -1;
        for (Int i = 0; i + mLen <= m - 1; i++)
        {
            if (binary_search(hashes.begin(), hashes.end(), h2.getHashPair(i + 1, i + mLen + 1), HashString::compareHashPair))
            {
                p = i;
                break;
            }
        }
        if (p >= 0)
        {
            pos = p;
            low = mLen + 1;
        }
        else
        {
            high = mLen - 1;
        }
    }
    Int substrLen = low;
    Int pos2 = pos, pos1 = -1;
    HashPair subHash = h2.getHashPair(pos2 + 1, pos2 + substrLen);
    //deb(subHash.first);
    //deb(subHash.second);
    for (Int i = 0; i + substrLen - 1 <= n - 1; i++)
    {
        HashPair tHash = h1.getHashPair(i + 1, i + substrLen);
        //deb(tHash.first);
        //deb(tHash.second);
        if (HashString::hashPairEqual(subHash, tHash))
        {
            pos1 = i;
            break;
        }
    }
    if (pos1 == -1 and pos2 == -1)
        pos1 = pos2 = 0;
    logs(pos1);
    logs(pos2);
    logn(substrLen);
}

int main()
{
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    HashString::calcPows(10e5);
    string s;
    while (getline(cin, s))
    {
        if (s == "")
            break;
        string s1, s2;
        stringstream ss(s);
        ss >> s1;
        ss >> s2;

        process(s1, s2);
    }
    return 0;
}

/*

LINK :
Status: In Progress

*/