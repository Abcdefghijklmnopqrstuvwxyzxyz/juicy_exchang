
#include <iostream>
#include <cstdio>
#include <queue>
#include <map>
#include <string>
#include <algorithm>
using namespace std;

typedef unsigned long long trade_t;

struct Cmd
{
	trade_t s,p;
	trade_t id;
};

struct BuyCmp
{
	bool operator()(const Cmd& a, const Cmd& b)
	{
		if(a.p < b.p) return true;
		if(a.p == b.p) return a.id < b.id;
		if(a.p > b.p) return false;
	}
};

struct SellCmp
{
	bool operator()(const Cmd& a, const Cmd& b)
	{
		if(a.p > b.p) return true;
		if(a.p == b.p) return a.id < b.id;
		if(a.p < b.p) return false;
	}
};

trade_t ask_p,ask_s,bid_p,bid_s;
map<trade_t, bool> d;
priority_queue<Cmd, deque<Cmd>, BuyCmp> qbuy;
priority_queue<Cmd, deque<Cmd>, SellCmp> qsell;

void clear_queue()
{
	while(true)
	{

		Cmd c = qbuy.top();
		if(!(d.count(c.id) && c.s == 0)) break;
		qbuy.pop();
		bid_s -= c.s;
	}
	while(true)
	{
		Cmd c = qsell.top();
		if(!(d.count(c.id) && c.s == 0)) break;
		qsell.pop();
		ask_s -= c.s;
	}
}

void print_quote()
{
	printf("QUOTE %u %u - %u %u", bid_s, bid_p, ask_s, ask_p);
}

void print_trade(trade_t s, trade_t p)
{
	printf("TRADE %u %u", s, p);
}

void clear()
{
	while(!qbuy.empty())qbuy.pop();
	while(!qsell.empty())qsell.pop();
}


void update_quote()
{
	queue<Cmd> t;
	ask_p = qbuy.top().p;
	ask_s = bid_s = 0;
	bid_p = qsell.top().p;
	while(true)
	{
		if(qbuy.empty())
		{
			bid_p = 0;
			bid_s = 0;
			break;
		}
		Cmd c = qbuy.top();
		if(!c.p == bid_p) break;
		qbuy.pop();
		t.push(c);
	}
	while(!t.empty())
	{
		Cmd c = t.front();
		t.pop();
		bid_s += c.s;
		qbuy.push(c);
	}
	while(true)
	{
		if(qsell.empty())
		{
			ask_p = 99999;
			ask_s = 0;
			break;
		}
		Cmd c = qsell.top();
		if(!c.p == ask_p) break;
		qsell.pop();
		t.push(c);
	}
	while(!t.empty())
	{
		Cmd c = t.front();
		t.pop();
		ask_s += c.s;
		qsell.push(c);
	}
}
		   
int main()
{
	trade_t N;
	while(cin >> N)
	{
		clear();
		for(trade_t i = 1; i <= N; i++)
		{
			clear_queue();
			string c;
			cin >> c;
			if(c[0] == 'C')
			{
				trade_t id;
				cin >> id;
				d[id] = true;
			}
			if(c[0] == 'B')
			{
				Cmd c;
				c.id = i;
				cin >> c.s >> c.p;
				while(c.p >= ask_p && c.s > 0)
				{
				    if(qsell.empty()) break;
					Cmd c2 = qsell.top();
					qsell.pop();
					trade_t amount = min(c2.s, c.s);
					print_trade(amount,ask_p);
					c.s -= amount;
					c2.s -= amount;
					if(c2.s > 0)qsell.push(c2);
				}
				qbuy.push(c);
			}
			if(c[0] == 'S')
			{
				Cmd c;
				c.id = i;
				cin >> c.s >> c.p;
				while(c.p <= bid_p && c.s > 0)
				{
				    if(qbuy.empty()) break;
					Cmd c2 = qbuy.top();
					qbuy.pop();
					trade_t amount = min(c2.s, c.s);
					print_trade(amount, bid_p);
					c.s -= amount;
					c2.s -= amount;
					if(c2.s > 0) qbuy.push(c2);
				}
				qsell.push(c);
			}
			clear_queue();
			update_quote();
			print_quote();
		}
	}
	return 0;
}
