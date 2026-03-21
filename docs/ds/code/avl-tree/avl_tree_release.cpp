#include <bits/stdc++.h>
using namespace std;

using i64 = long long;
using u64 = unsigned long long;
using u32 = unsigned;

using i128 = __int128;
using u128 = unsigned __int128;

mt19937_64 mrand((u64)random_device{}() << 32 ^ random_device{}() ^
	chrono::high_resolution_clock::now().time_since_epoch().count());
template<class T = i64,class T2>T rnd(T l,T2 r){
return uniform_int_distribution<T>(l,r)(mrand);}

struct node{
	int key = 0,sz = 0,s[2] = {},f = 0,d = 0;//为了把左旋和右旋写得对称一点，我们把l和r改成s[0]和s[1]
	bool operator < (const node &oth) const {return key < oth.key;}
}a[200010];

int mknew(int key){
	static int top = 0;
	a[++top] = {key,1,{},0,1};
	return top;
}

bool isr(int x){return x == a[a[x].f].s[1];}

int head;

void setf(int x,bool id,int f){
	a[f].s[id] = x;
	if (x) a[x].f = f;
}

void update(int x){
	if (!x) return ;
	a[x].sz = a[a[x].s[0]].sz + 1 + a[a[x].s[1]].sz;
	a[x].d = max(a[a[x].s[0]].d,a[a[x].s[1]].d) + 1;
}

void rot(int x){
/*
            g                g
           / \              / \
          /   \            /   \
         f        ->      x
        / \       <-     / \
       /   \            /   \
   	  x     b          l     f
     / \   / \        / \   / \
    l   r                  r   b
   / \ / \                / \ / \
*/
	int f = a[x].f,b = isr(x),c = isr(f);
	if (f == head) a[x].f = 0,head = x;
	else setf(x,c,a[f].f);
	setf(a[x].s[!b],b,f);
	setf(f,!b,x);
	update(f);
	update(x);
}

void upd(int x){//自底向上递归更新路径信息
	for (;x;x = a[x].f)
		update(x);
}

void balance(int x){//更新路径信息并平衡
	upd(x);

	for (;x;x = a[x].f){
		if (a[a[x].s[0]].d - a[a[x].s[1]].d > 1){
			if (a[a[a[x].s[0]].s[0]].d >= a[a[a[x].s[0]].s[1]].d)
				rot(x = a[x].s[0]);
			else
				rot(x = a[a[x].s[0]].s[1]),rot(x);
		}else if (a[a[x].s[1]].d - a[a[x].s[0]].d > 1){
			if (a[a[a[x].s[1]].s[0]].d >= a[a[a[x].s[1]].s[1]].d)
				rot(x = a[x].s[1]);
			else
				rot(x = a[a[x].s[1]].s[1]),rot(x);
		}
	}
}

void insert(int x){
	int nd = mknew(x);

	if (!head){//空树
		head = nd;
		return ;
	}

	x = head;
	bool to;

	for (;;){
		to = (a[nd] < a[x]?0:1);

		if (!a[x].s[to])
			break;

		x = a[x].s[to];
	}

	a[x].s[to] = nd;
	a[nd].f = x;
	x = nd;//插入到树中

	balance(x);
}

void erase(int x){
	int nd = head;

	for (;nd;){
		if (a[nd].key == x)
			break;

		if (a[nd].key < x)
			nd = a[nd].s[1];
		else
			nd = a[nd].s[0];
	}

	if (!a[nd].s[0])//被删除的是根节点且根节点没有右儿子
		if (nd == head)
			nd = head = a[nd].s[1],a[a[nd].s[1]].f = 0;
		else
			setf(a[nd].s[1],isr(nd),a[nd].f),nd = a[nd].f;
	else{//其他情况
		int x = a[nd].s[0];

		for (;a[x].s[1];x = a[x].s[1]);

		swap(a[x].key,a[nd].key);

		nd = a[x].f;//从这里开始维护平衡
		setf(a[x].s[0],isr(x),a[x].f);
	}

	balance(nd);
}
//平衡树常识啦！
int Rank(int x){
	int ans = 1,nd = head;

	for (;nd;)
		if (a[nd].key < x)
			ans += a[a[nd].s[0]].sz + 1,nd = a[nd].s[1];
		else
			nd = a[nd].s[0];

	return ans;
}

int kth(int x){
	int nd = head;

	for (;nd;)
		if (a[a[nd].s[0]].sz < x){
			x -= a[a[nd].s[0]].sz + 1;

			if (!x)
				return a[nd].key;

			nd = a[nd].s[1];
		}else
			nd = a[nd].s[0];

	abort();
}

int Prev(int x){
	return kth(Rank(x) - 1);
}

int Next(int x){
	int nd = head,ans = 0;

	for (;nd;)
		if (a[nd].key <= x)
			nd = a[nd].s[1];
		else
			ans = a[nd].key,nd = a[nd].s[0];

	return ans;
}

int main (){
	ios::sync_with_stdio(0);
	cin.tie(0),cout.tie(0);

	int n;
	cin >> n;

	for (;n--;){
		int op,x;
		cin >> op >> x;

		switch (op){
		case 1:
			insert(x);
			break;
		case 2:
			erase(x);
			break;
		case 3:
			cout << Rank(x) << '\n';
			break;
		case 4:
			cout << kth(x) << '\n';
			break;
		case 5:
			cout << Prev(x) << '\n';
			break;
		case 6:
			cout << Next(x) << '\n';
			break;
		}
	}
}