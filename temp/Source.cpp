#include <string>
#include <iostream>
#include <vector>
#include <thread>
#include <random>
#include <windows.h>
#include <chrono>
using namespace std::chrono;
using namespace std;

class BigInt {
	string digits;
public:
	BigInt();
	BigInt(int bit_len);
	BigInt(const char*);
	BigInt(string);
	BigInt(BigInt&);
	BigInt(const BigInt&);

	friend bool Null(const BigInt&);
	friend int Length(const BigInt&);
	int operator[](const int)const;

	BigInt& operator=(const BigInt&);

	BigInt& operator++();
	BigInt operator++(int temp);
	BigInt& operator--();
	BigInt operator--(int temp);

	friend BigInt binpow(BigInt base, BigInt e, BigInt mod);
	friend void divide_by_2(BigInt& a);

	friend BigInt& operator+=(BigInt&, const BigInt&);
	friend BigInt operator+(const BigInt&, const BigInt&);
	friend BigInt operator-(const BigInt&, const BigInt&);
	friend BigInt& operator-=(BigInt&, const BigInt&);

	friend bool operator==(const BigInt&, const BigInt&);
	friend bool operator!=(const BigInt&, const BigInt&);

	friend bool operator>(const BigInt&, const BigInt&);
	friend bool operator>=(const BigInt&, const BigInt&);
	friend bool operator<(const BigInt&, const BigInt&);
	friend bool operator<=(const BigInt&, const BigInt&);

	friend BigInt& operator*=(BigInt&, const BigInt&);
	friend BigInt operator*(const BigInt&, const BigInt&);
	friend BigInt& operator/=(BigInt&, const BigInt&);
	friend BigInt operator/(const BigInt&, const BigInt&);

	friend BigInt operator%(const BigInt&, const BigInt&);
	friend BigInt& operator%=(BigInt&, const BigInt&);

	friend ostream& operator<<(ostream&, const BigInt&);

	friend bool MillerRabin(BigInt n, int iter);

};

int intRand(int min, int max) {
	thread_local std::mt19937 generator(std::random_device{}());
	std::uniform_int_distribution<int> distribution(min, max);
	return distribution(generator);
}

BigInt::BigInt(const char* s) {
	digits = "";
	for (int i = strlen(s) - 1; i >= 0;i--)
	{
		if (!isdigit(s[i]))
			throw("ERROR");
		digits.push_back(s[i] - '0');
	}
}

BigInt::BigInt(string s) {
	digits = "";
	for (int i = s.length() - 1; i >= 0;i--)
	{
		if (!isdigit(s[i]))
			throw("ERROR");
		digits.push_back(s[i] - '0');
	}
}

BigInt::BigInt() {
	digits = "0";
}

BigInt::BigInt(int bit_len) {
	digits = "";
	int n = ceil(bit_len * 0.30103) - 1;
	for (int i = 0; i < n;i++) {
		digits += intRand(0, 9);
	}
	int arr[4] = { 1,3,7,9 };
	digits[0] = arr[intRand(0, 9) % 4];
	digits += 1 + intRand(0, 8);
}

BigInt::BigInt(BigInt& a) {
	digits = a.digits;
}

BigInt::BigInt(const BigInt& a) {
	digits = a.digits;
}

bool Null(const BigInt& a) {
	if (a.digits.size() == 1 && a.digits[0] == 0)
		return true;
	return false;
}
int Length(const BigInt& a) {
	return a.digits.size();
}
int BigInt::operator[](const int index)const {
	if (digits.size() <= index || index < 0)
		throw("ERROR");
	return digits[index];
}
bool operator==(const BigInt& a, const BigInt& b) {
	return a.digits == b.digits;
}
bool operator!=(const BigInt& a, const BigInt& b) {
	return !(a == b);
}
bool operator<(const BigInt& a, const BigInt& b) {
	int n = Length(a), m = Length(b);
	if (n != m)
		return n < m;
	while (n--)
		if (a.digits[n] != b.digits[n])
			return a.digits[n] < b.digits[n];
	return false;
}
bool operator>(const BigInt& a, const BigInt& b) {
	return b < a;
}
bool operator>=(const BigInt& a, const BigInt& b) {
	return !(a < b);
}
bool operator<=(const BigInt& a, const BigInt& b) {
	return !(a > b);
}

BigInt& BigInt::operator=(const BigInt& a) {
	digits = a.digits;
	return *this;
}

BigInt& BigInt::operator++() {
	int i, n = digits.size();
	for (i = 0; i < n && digits[i] == 9;i++)
		digits[i] = 0;
	if (i == n)
		digits.push_back(1);
	else
		digits[i]++;
	return *this;
}
BigInt BigInt::operator++(int temp) {
	BigInt aux = *this;
	++(*this);
	return aux;
}

BigInt& BigInt::operator--() {
	if (digits[0] == 0 && digits.size() == 1)
		throw("UNDERFLOW");
	int i, n = digits.size();
	for (i = 0; digits[i] == 0 && i < n;i++)
		digits[i] = 9;
	digits[i]--;
	if (n > 1 && digits[n - 1] == 0)
		digits.pop_back();
	return *this;
}
BigInt BigInt::operator--(int temp) {
	BigInt aux = *this;
	--(*this);
	return aux;
}

BigInt& operator+=(BigInt& a, const BigInt& b) {
	int t = 0, s, i;
	int n = Length(a), m = Length(b);
	if (m > n)
		a.digits.append(m - n, 0);
	n = Length(a);
	for (i = 0; i < n;i++) {
		if (i < m)
			s = (a.digits[i] + b.digits[i]) + t;
		else
			s = a.digits[i] + t;
		t = s / 10;
		a.digits[i] = (s % 10);
	}
	if (t)
		a.digits.push_back(t);
	return a;
}
BigInt operator+(const BigInt& a, const BigInt& b) {
	BigInt temp(a);
	temp += b;
	return temp;
}

BigInt& operator-=(BigInt& a, const BigInt& b) {
	if (a < b)
		throw("UNDERFLOW");
	int n = Length(a), m = Length(b);
	int i, t = 0, s;
	for (i = 0; i < n;i++) {
		if (i < m)
			s = a.digits[i] - b.digits[i] + t;
		else
			s = a.digits[i] + t;
		if (s < 0)
			s += 10,
			t = -1;
		else
			t = 0;
		a.digits[i] = s;
	}
	while (n > 1 && a.digits[n - 1] == 0)
		a.digits.pop_back(),
		n--;
	return a;
}
BigInt operator-(const BigInt& a, const BigInt& b) {
	BigInt temp(a);
	temp -= b;
	return temp;
}

BigInt& operator*=(BigInt& A, const BigInt& B)
{
	if (Null(A) || Null(B)) {
		A = BigInt("0");
		return A;
	}
	int n = A.digits.size(), m = B.digits.size();

	BigInt C("");
	for (int i = 0; i < n + m;i++) {
		C.digits += '0';
	}
	for (int b = 0; b < m; b++) {
		int flag = 0;
		int a;
		for (a = 0; a < n;a++) {
			int x = (B.digits[b]) * (A.digits[a]) + flag + (C.digits[a + b] - '0');
			C.digits[a + b] = x % 10 + '0';
			flag = x / 10;
		}
		if (flag > 0) {
			C.digits[a + b] = flag + '0';
		}
	}
	if (C.digits[n + m - 1] != '0') {
		C.digits[n + m] = '\0';
	}
	else {
		C.digits[n + m - 1] = '\0';
	}
	for (int i = 0; i < C.digits.size()-1 ;i++) {  //size -1
		C.digits[i] -= '0';
	}
	if (C.digits.back() == '\0') {
		C.digits.pop_back();
	}
	else {
		C.digits.back() -= '0';
	}
	A = C;
	return A;
}
BigInt operator*(const BigInt& a, const BigInt& b) {
	BigInt temp= a;
	temp *= b;
	return temp;
}

BigInt& operator/=(BigInt& A, const BigInt& B) {
	if (Null(B))
		throw("Arithmetic Error: Division By 0");
	if (A < B) {
		A = BigInt("0");
		return A;
	}
	if (A == B) {
		A = BigInt("1");
		return A;
	}
	int n = A.digits.size(), m = B.digits.size();
	int size = n - m;
	string l = "1";
	string r = "99";
	for (int i = 1;i < size;i++) {
		l += "0";
		r += "9";
	}
	BigInt left(l);
	BigInt right(r);

	BigInt kq = left * B;
	BigInt mid("0");
	BigInt ss(A);
	while (left <= right) {
		mid = right - left;
		divide_by_2(mid);
		mid = mid + left;
		kq = mid * B;
		if (kq < A) {
			left = mid;
			ss = A - kq;
		}
		else if (kq == A) {
			return mid;
		}
		else
			right = mid;
		if (ss <= B) {
			A = mid;
			return A;
		}
	}
	//BigInt kq("0");
	//BigInt rt(A);
	//BigInt temp("");
	//BigInt base("10");
	//BigInt time("1");
	//BigInt t("1");
	//int n = A.digits.size(), m = B.digits.size();
	//do {
	//	temp = B;
	//	while (rt.digits.size() != temp.digits.size())
	//	{
	//		temp *= base;
	//		time *= base;
	//	}
	//	if (rt <= temp) {
	//		temp.digits.erase(0,1);
	//		time.digits.erase(0,1);
	//	}

	//	kq += time;
	//	rt -= temp;
	//	time = t;
	//}
	//while (rt > B);
	//if (rt == B) kq += t;
	//A = kq;
	//return A;
}
BigInt operator/(const BigInt& a, const BigInt& b) {
	BigInt temp = a;
	temp /= b;
	return temp;
}

void divide_by_2(BigInt& a) {
	int add = 0;
	for (int i = a.digits.size() - 1; i >= 0;i--) {
		int digit = (a.digits[i] >> 1) + add;
		add = ((a.digits[i] & 1) * 5);
		a.digits[i] = digit;
	}
	while (a.digits.size() > 1 && !a.digits.back())
		a.digits.pop_back();
}

BigInt& operator%=(BigInt& A, const BigInt& B) {
	if (Null(B))
		throw("Arithmetic Error: Division By 0");
	if (A < B) {
		return A;
	}
	if (A == B) {
		A = BigInt("0");
		return A;
	}
	int n = A.digits.size(), m = B.digits.size();
	int size = n - m;
	string l = "1";
	string r = "99";
	for (int i = 1;i < size;i++) {
		l += "0";
		r += "9";
	}
	BigInt left(l);
	BigInt right(r);

	BigInt kq = left * B;
	BigInt mid("0");
	BigInt ss(A);
	while (left <= right) {
		mid = right - left;
		divide_by_2(mid);
		mid = mid + left;
		kq = mid * B;
		if (kq < A) {
			left = mid;
			ss = A - kq;
		}
		else
			right = mid;
		if (ss <= B) {
			A = ss;
			if (ss == B) {
				A = BigInt("0");
			}
			return A;
		}
	}
	//BigInt kq("0");
	//BigInt rt(A);
	//BigInt temp("");
	//BigInt base("10");
	//BigInt time("1");
	//BigInt t("1");
	//int n = A.digits.size(), m = B.digits.size();

	//do {
	//	temp = B;
	//	while (rt.digits.size() != temp.digits.size())
	//	{
	//		temp *= base;
	//		time *= base;
	//	}
	//	if (rt <= temp) {
	//		temp.digits.erase(0, 1);
	//		time.digits.erase(0, 1);
	//	}
	//	kq += time;
	//	rt -= temp;

	//	time = t;
	//	//cout << rt;
	//	//cout << "ket qua:" << kq;
	//} while (rt > B);
	//if (rt == B) rt = BigInt("0");

	//A = rt;
	//return A;
}
BigInt operator%(const BigInt& a, BigInt& b) {
	BigInt temp = a;
	temp %= b;
	return temp;
}

BigInt binpow(BigInt base, BigInt e, BigInt mod) {
	BigInt num0("0");
	BigInt result("1");
	base %= mod;
	while (e > num0) {
		int k = e.digits[0];
		if (k % 2 == 1) {
			result *= base;
			result %= mod;
		}
		base *= base;
		base %= mod;
		divide_by_2(e);
	}
	return result;
}

ostream& operator<<(ostream& out, const BigInt& a) {
	for (int i = a.digits.size() - 1; i >= 0;i--)
		cout << (short)a.digits[i];
	cout << endl;
	return cout;
}

BigInt modInverse(BigInt A, BigInt M) {
	BigInt n0("0");
	BigInt n1("1");
	BigInt m0 = M;
	BigInt y = n0, x = n1;
	BigInt temp("1");
	bool check_x = false;
	bool check_y = false;
	bool check_t = false;
	if (M == n1)
		return 0;
	int i = 0;
	while (A > n1) {
		BigInt q = A / M;
		BigInt t = M;
		M = A % M, A = t;
		t = y;
		if (check_y) {
			check_t = true;
		}
		else check_t = false;
		temp = q * y;
		if (!check_x && check_y) {
			y = x + temp;
			check_y = false;
		}
		else if (check_x && !check_y) {
			y = x + temp;
			check_y = true;
		}
		else {
			if (temp <= x) {
				y = x - temp;
			}
			else {
				check_y = true;
				y = temp - x;
			}
		}
		x = t;
		if (check_t) {
			check_x = true;
		}
		else {
			check_x = false;
		}
	}
	if (check_x)
		x = m0 - x;
	return x;
}


bool check_composite(BigInt n, BigInt a, BigInt d, int s) {
	BigInt x = binpow(a, d, n);
	BigInt n1("1");
	BigInt t = n - n1;
	if (x == n1 || x == t)
		return false;
	for (int r = 1; r < s; r++) {
		x = x * x % n;
		if (x == t)
			return false;
	}
	return true;
};

bool MillerRabin(BigInt n, int iter) {
	BigInt n1("1");

	int s = 0;
	BigInt d = n - n1;
	int k = d.digits[0];
	while ((k % 2) == 0) {
		divide_by_2(d);
		s++;
		k = d.digits[0];
	}

	for (int i = 0; i < iter; i++) {
		BigInt a(166);
		//cout << "miller lan thu: " << i << endl;
		if (check_composite(n, a, d, s))
			return false;
		cout << "xong miller lan thu: " << i << endl;
	}
	return true;
}



void primeGenerator(int size, vector<BigInt>& pl) {
	vector <BigInt> primes;
	BigInt n0("0");
	vector<string> primelist = { "3", "5", "7", "11", "13", "17", "19", "23", "29", "31", "37", "41", "43", "47", "53", "59", "61", "67", "71", "73", "79", "83", "89", "97", "101", "103", "107", "109", "113", "127", "131", "137", "139", "149", "151", "157", "163", "167", "173", "179", "181", "191", "193", "197", "199", "211", "223", "227", "229", "233", "239", "241", "251", "257", "263", "269", "271", "277", "281", "283", "293", "307", "311", "313", "317", "331", "337", "347", "349", "353", "359", "367", "373", "379", "383", "389", "397", "401", "409", "419", "421", "431", "433", "439", "443", "449", "457", "461", "463", "467", "479", "487", "491", "499", "503", "509", "521", "523", "541", "547", "557", "563", "569", "571", "577", "587", "593", "599", "601", "607", "613", "617", "619", "631", "641", "643", "647", "653", "659", "661", "673", "677", "683", "691", "701", "709", "719", "727", "733", "739", "743", "751", "757", "761", "769", "773", "787", "797", "809", "811", "821", "823", "827", "829", "839", "853", "857", "859", "863", "877", "881", "883", "887", "907", "911", "919", "929", "937", "941", "947", "953", "967", "971", "977", "983", "991", "997", "1009", "1013", "1019", "1021", "1031", "1033", "1039", "1049", "1051", "1061", "1063", "1069", "1087", "1091", "1093", "1097", "1103", "1109", "1117", "1123", "1129", "1151", "1153", "1163", "1171", "1181", "1187", "1193", "1201", "1213", "1217", "1223", "1229", "1231", "1237", "1249", "1259", "1277", "1279", "1283", "1289", "1291", "1297", "1301", "1303", "1307", "1319", "1321", "1327", "1361", "1367", "1373", "1381", "1399", "1409", "1423", "1427", "1429", "1433", "1439", "1447", "1451", "1453", "1459", "1471", "1481", "1483", "1487", "1489", "1493", "1499", "1511", "1523", "1531", "1543", "1549", "1553", "1559", "1567", "1571", "1579", "1583", "1597", "1601", "1607", "1609", "1613", "1619", "1621", "1627", "1637", "1657", "1663", "1667", "1669", "1693", "1697", "1699", "1709", "1721", "1723", "1733", "1741", "1747", "1753", "1759", "1777", "1783", "1787", "1789", "1801", "1811", "1823", "1831", "1847", "1861", "1867", "1871", "1873", "1877", "1879", "1889", "1901", "1907", "1913", "1931", "1933", "1949", "1951", "1973", "1979", "1987", "1993", "1997", "1999", "2003", "2011", "2017", "2027", "2029", "2039", "2053", "2063", "2069", "2081", "2083", "2087", "2089", "2099", "2111", "2113", "2129", "2131", "2137", "2141", "2143", "2153", "2161", "2179", "2203", "2207", "2213", "2221", "2237", "2239", "2243", "2251", "2267", "2269", "2273", "2281", "2287", "2293", "2297", "2309", "2311", "2333", "2339", "2341", "2347", "2351", "2357", "2371", "2377", "2381", "2383", "2389", "2393", "2399", "2411", "2417", "2423", "2437", "2441", "2447", "2459", "2467", "2473", "2477", "2503", "2521", "2531", "2539", "2543", "2549", "2551", "2557", "2579", "2591", "2593", "2609", "2617", "2621", "2633", "2647", "2657", "2659", "2663", "2671", "2677", "2683", "2687", "2689", "2693", "2699", "2707", "2711", "2713", "2719", "2729", "2731", "2741", "2749", "2753", "2767", "2777", "2789", "2791", "2797", "2801", "2803", "2819", "2833", "2837", "2843", "2851", "2857", "2861", "2879", "2887", "2897", "2903", "2909", "2917", "2927", "2939", "2953", "2957", "2963", "2969", "2971", "2999", "3001", "3011", "3019", "3023", "3037", "3041", "3049", "3061", "3067", "3079", "3083", "3089", "3109", "3119", "3121", "3137", "3163", "3167", "3169", "3181", "3187", "3191", "3203", "3209", "3217", "3221", "3229", "3251", "3253", "3257", "3259", "3271", "3299", "3301", "3307", "3313", "3319", "3323", "3329", "3331", "3343", "3347", "3359", "3361", "3371", "3373", "3389", "3391", "3407", "3413", "3433", "3449", "3457", "3461", "3463", "3467", "3469", "3491", "3499", "3511", "3517", "3527", "3529", "3533", "3539", "3541", "3547", "3557", "3559", "3571", "3581"};

	for (int i=0;i < 500;i++) {
		primes.push_back(BigInt(primelist[i]));
	}
	while (true) {
		BigInt p(size);
		bool check = false;
		for (int i=0;i < 500;i++) {
			if (p % primes[i] == n0) {
				check = true;
				break;
			}
		}
		if (check == false) {
			if (MillerRabin(p, 3)) {
				pl.push_back(p);
			}
		}
	}
}

void RSA_key_gen(vector <BigInt> primes) {
	BigInt n1("1");
	BigInt N = primes[0] * primes[1];
	BigInt phi = (primes[0] - n1) * (primes[1] - n1);
	BigInt e("65537");
	BigInt d = modInverse(e, phi);
	cout << "N :" << N;
	//cout << "phi :" << phi;
	cout << "e :" << e;
	cout << "d :" << d;

}

void bestcase_utility(vector <BigInt> &primes, BigInt p) {
	if (MillerRabin(p, 3)) {
		primes.push_back(p);
	}
}

void bestcase(int size) {
	auto start = high_resolution_clock::now();
	vector <BigInt> primes;
	if (size == 1024) {
		BigInt p("9322836429283396474116709145643257806750829644456661129223428781442327908782953783782282705658610614048774135661203760326643552131164246697327273370388787");
		BigInt q("7453972708564828132544149815299784762887116385822283954081780260394355331304971420169419276666650885189959137424401409470495176518030685390738019668009493");
		thread thr1(bestcase_utility, std::ref(primes), p);
		thr1.detach();
		thread thr2(bestcase_utility, std::ref(primes), q);
		thr2.detach();
	}
	else if (size == 2048) {
		BigInt p("161396784824625307284081165802712967193572609991659410950893526437022322378991980127116012329764407008121289304213530542147571656739882165921157793446461371090165547425284887859201472501392001093996644375922046587446709102600393778204050269339888456454384855643954258368926744539001446732034500026582229181381");
		BigInt q("101004648173579736151747040762319872573125109099555333106351231777898455291218937201983497826919040561666212151337563473405473240443578280258305339594003974407392563992636051997186233973242924511323329722807209143875063563995318327838359020530515153347266378239546693733287506187557234319877368340514905061689");
		thread thr1(bestcase_utility, std::ref(primes), p);
		thr1.detach();
		thread thr2(bestcase_utility, std::ref(primes), q);
		thr2.detach();
	}
	while (true) {
		if (primes.size() == 2) break;
	}
	RSA_key_gen(primes);
	auto stop = high_resolution_clock::now();
	auto duration = duration_cast<seconds>(stop - start);
	cout << "executed in " << duration.count() << " seconds" << endl;
}

void run(int size) {
	auto start = high_resolution_clock::now();
	vector <BigInt> primes;
	for (int i = 0;i < 8;i++) {
		thread thr(primeGenerator, size, std::ref(primes));
		thr.detach();
		Sleep(10);
	}
	while (true) {
		if (primes.size() == 2) {
			break;
		}
	}
	RSA_key_gen(primes);
	auto stop = high_resolution_clock::now();
	auto duration = duration_cast<seconds>(stop - start);
	cout << "executed in " << duration.count() << " seconds" << endl;
}

int main()
{
	//BigInt first("6655656777888865489713218764321656878978956488");
	//cout << "The number of digits"
	//	<< " in first big integer = "
	//	<< Length(first) << '\n';
	//BigInt third("5");
	//cout << first % third;

	//auto start = high_resolution_clock::now();
	//BigInt first("63114332134597298167586122324167251171554288490477");
	//cout << "The number of digits"
	//	<< " in first big integer = "
	//	<< Length(first) << '\n';

	//BigInt third("2394982229084464369186143536490431154951105459603198585284857286757763383909");
	//BigInt s("306557725322811439255826372670775187833741498829209418916461732704993713140353");
	//cout << binpow(first, third, s);
	//auto stop = high_resolution_clock::now();

	//srand(time(0));
	//auto duration = duration_cast<microseconds>(stop - start);
	//cout << duration.count() << endl;

	//auto start = high_resolution_clock::now();
	//vector <BigInt> primes;
	//for (int i = 0;i < 8;i++) {
	//	thread thr(primeGenerator, 512, std::ref(primes));
	//	thr.detach();
	//	Sleep(10);
	//}
	//while (true) {
	//	if (primes.size() == 2) {
	//		break;
	//	}
	//}
	//RSA_key_gen(primes);

	//auto stop = high_resolution_clock::now();

	//auto duration = duration_cast<seconds>(stop - start);
	//cout << "executed in " << duration.count() << " seconds" << endl;


	int choice;
	int size = 0;
	cout << "==============RSA KEY GENERATOR=============\n";
	cout << "\t\t1. 512 bit \n";
	cout << "\t\t2. 1024 bit \n";
	cout << "\t\t3. 2048 bit \n";
	cout << "\n";
	cout << "Pretended that prime is generated at first generation\n";
	cout << "=============BEST CASE GENERATOR============\n";
	cout << "\t\t4. 1024 bit \n";
	cout << "\t\t5. 2048 bit \n";

	cout << "choice? ";
	cin >> choice;
	if (choice == 1) size = 256;
	else if (choice == 2) size = 512;
	else if (choice == 3) size = 1024;
	else if (choice == 4) {
		cout << "generating " << 1024 << " bits key\n";
		bestcase(1024);
	}
	else if (choice == 5) {
		cout << "generating " << 2048 << " bits key\n";
		bestcase(2048);
	}
		
	if (size > 0) {
		cout << "generating " << size*2 << " bits key\n";
		run(size);
		size = 0;
		cout << "--------------------done--------------------\n";
	}
	return 0;
}
