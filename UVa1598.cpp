#include <iostream>
#include <queue>
#include <map>
using namespace std;

typedef unsigned long long trade_t

struct Cmd
{
	trade_t s,p;
	trade_t id;
};

trade_t ask_p,ask_s,bid_p,bid_s;
map<trade_t, bool> d;
proirity_queue<Cmd> qbuy, qsell;

void clear_queue()
{
	while(true)
	{
		Cmd c = qbuy.top();
		if(!(d.count(c.id) && c.s == 0) break;
		qbuy.pop();
	}
}
