#include <iostream>
#include <cstdio>
#include <queue>
#include <map>
#include <algorithm>
using namespace std;

typedef unsigned long long trade_t;

struct Command
{
    trade_t id, p, s;
    bool type; //True for BUY, false for SELL.
    bool operator<(const Command &b) const
    {
        if(type)
        {
            //BUY
            if(p == b.p) return id < b.id;
            else return p < b.p;
        }
        else
        {
            //SELL
            if(p == b.p) return id < b.id;
            else return p > b.p;
        }
    }
};

priority_queue<Command> qbuy, qsell;
map<trade_t, bool> cancelled;
map<trade_t, trade_t> buys, sells;

trade_t bid_p, bid_s, ask_p, ask_s;


void clearQueue()
{
    while(true)
    {
    	if(qbuy.empty()) break;
        trade_t id = qbuy.top().id;
        if(qbuy.empty() || ((cancelled.count(id) == 0) && (qbuy.top().s > 0))) break;
        buys[qbuy.top().p] -= qbuy.top().s;
        qbuy.pop();
    }
    while(true)
    {
    	if(qsell.empty()) break;
        trade_t id = qsell.top().id;
        if(qsell.empty() || (cancelled.count(id) == 0) && (qsell.top().s > 0)) break;
        sells[qsell.top().p] -= qsell.top().s;
        qsell.pop();
    }
}

void update_quote()
{
	if (qbuy.empty())
		bid_p = 0;
	else
		bid_p = qbuy.top().p;
	if (qsell.empty())
		ask_p = 99999;
	else
		ask_p = qsell.top().p;
}

void print_quote()
{
	trade_t bid_s = buys[bid_p], ask_s = sells[ask_p];
	printf("QUOTE %u %u - %u %u\n", bid_s, bid_p, ask_s, ask_p);
}

void trade(trade_t s, trade_t p, trade_t id)
{
	printf("TRADE %u %u\n", s, p);
}

int _main(unsigned N)
{
    for(unsigned i = 1 ; i <= N ; i++)
    {
    	update_quote();
        string cmd;
        cin >> cmd;
        if(cmd[0] == 'C')
        {
            trade_t id;
            cin >> id;
            cancelled[id] = true;
        }
        if(cmd[0] == 'B')
        {
            Command c;
            c.type = true;
            c.id = i;
            cin >> c.s >> c.p;
            buys[c.p] += c.s;
            while(c.p >= ask_p && c.s > 0)
            {
                Command c2 = qsell.top();
                qsell.pop();
                trade_t amount = min(c2.s, c.s);
                trade(amount, ask_p, c.id);
                c2.s -= amount;
                c.s -= amount;
                sells[ask_p] -= amount;
                buys[c.p] -= amount;
                if(c2.s > 0) qsell.push(c2);
                update_quote();
            }
            qbuy.push(c);
        }
        if(cmd[0] == 'S')
        {
            Command c;
            c.type = false;
            c.id = i;
            cin >> c.s >> c.p;
            sells[c.p] += c.s;
            while(c.p <= bid_p && c.s > 0)
            {
                Command c2 = qbuy.top();
                qbuy.pop();
                trade_t amount = min(c2.s, c.s);
                trade(amount, bid_p, c.id);
                c2.s -= amount;
                c.s -= amount;
                buys[bid_p] -= amount;
                sells[c.p] -= amount;
                if(c2.s > 0) qbuy.push(c2);
                update_quote(); 
            }
            qsell.push(c);
        }
        clearQueue();
        update_quote();
        print_quote();
    }
}

int main()
{
    unsigned N;
    while(cin>>N)
    {
    	while(!qbuy.empty()) qbuy.pop();
    	while(!qsell.empty()) qsell.pop();
        _main(N);
    }
}
