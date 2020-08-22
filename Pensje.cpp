#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>

using namespace std;
struct pracownik {
	pracownik(int id, int pen, int wielkosc_drzewa = 0) :id_pracownika(id), pensja(pen), wielosc_drzewa(0) {};
	int id_pracownika;
	int pensja;
	int wielosc_drzewa;
	bool operator<(pracownik  const &_drugi) const {
		return pensja < _drugi.pensja;
	}

};

int wielkoscPoddrzewa(int pracownikId, vector<vector<int>> &pracownicy_bez_pensji)
{
	int ilosc_potomkow = 0;
	for (auto i : pracownicy_bez_pensji[pracownikId])
	{
		ilosc_potomkow += wielkoscPoddrzewa(i, pracownicy_bez_pensji);
	}
	return 1 + ilosc_potomkow;
}


void preOrder(int v, vector<int> &M, vector<int> &pensje, vector<vector<int>> &pracownicy_b_pensji)
{

	if (pracownicy_b_pensji[v].size() == 1 && M.size() != 0)
	{
		pensje[pracownicy_b_pensji[v].front()] = M.back();
		M.pop_back();
		preOrder(pracownicy_b_pensji[v].front(), M, pensje, pracownicy_b_pensji);
	}
}


int main() {
	std::ios_base::sync_with_stdio(false);
	int ilosc_pracownikow = 0;
	int id_przelozonego;
	int pensja;
	vector<int> a;

	queue<int> K;
	queue<int> P;

	cin >> ilosc_pracownikow;

	vector<pracownik> pracownicy_z_pensja;//wezly z wynagrodzeniem
	vector<vector<int>> pracownicy_bez_pensji(ilosc_pracownikow + 1); //lista sasiedztwa zaczynaj¹æ od wez³ów bez podanego wynagrodzenia
	vector<int> pensje(ilosc_pracownikow + 1, 0);//pensje pracownikow indeks = id pracownika
	vector<bool> uzyte_pensje(ilosc_pracownikow + 1, false); // pensje publicznie znane
	vector<bool> niejednoznacze(ilosc_pracownikow + 1, false);

	for (int i = 0; i < ilosc_pracownikow; i++)
	{
		cin >> id_przelozonego >> pensja;

		if (id_przelozonego == i + 1)
		{
			pensja = ilosc_pracownikow;
		}
		if (pensja != 0)
		{

			pracownicy_z_pensja.push_back(pracownik(i + 1, pensja));
			pensje[i + 1] = pensja;
			uzyte_pensje[pensja] = true;


		}
		else {
			pracownicy_bez_pensji[id_przelozonego].push_back(i + 1);
		}
	}

	for (int i = 1; i < ilosc_pracownikow + 1; i++)
	{
		if (!uzyte_pensje[i])
		{
			K.push(i);
			P.push(i);
		}
	}
	sort(pracownicy_z_pensja.begin(), pracownicy_z_pensja.end());
	for (auto it = pracownicy_z_pensja.begin(); it != pracownicy_z_pensja.end(); it++)
	{
		it->wielosc_drzewa = wielkoscPoddrzewa(it->id_pracownika, pracownicy_bez_pensji);
	}


	for (auto it = pracownicy_z_pensja.begin(); it != pracownicy_z_pensja.end(); it++)
	{
		
		vector<int> M;
		if (it->wielosc_drzewa > 1)
		{
			for (int i = 0; i < it->wielosc_drzewa - 1; i++)
			{
				if (K.empty() || K.front() > it->pensja)
				{
					break;
				}
				M.push_back(K.front());
				K.pop();
			}
			

			if (K.empty() || K.front() > it->pensja)
			{
				
				preOrder(it->id_pracownika, M, pensje, pracownicy_bez_pensji);
				while (!P.empty() && P.front() < it->pensja)
				{
					P.pop();
				}

			}
			else {
				while (!P.empty() && P.front() < it->pensja)
				{
					niejednoznacze[P.front()] = true;
					P.pop();
				}
			}
		}
	}


	for (int i = 1; i < ilosc_pracownikow + 1; i++)
	{
		if (niejednoznacze[pensje[i]])
		{
			cout << "0" << endl;;
		}
		else {
			cout << pensje[i] << endl;
		}
	}

	return 0;
}