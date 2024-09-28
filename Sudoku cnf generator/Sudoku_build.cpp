#include <bits/stdc++.h>
using namespace std;

int n, sdk[1005][1005]; // sudoku size and the sudoku board

struct Node{
    int i, j, k;
    Node(int i, int j, int k) : i(i), j(j), k(k) {}
};

Node decode(int val)
{
    int k = val % n; if(k==0) k=4; val = (val-k) / n;
    int j = val % n + 1; val = (val-j+1) / n;
    int i = val + 1;
    return Node(i, j, k);
}

void input()
{
    cin >> n;
    
    int sqrt_n = sqrt(n);
    if(sqrt_n*sqrt_n!=n)
    {
        cout << "Invalid size, n has to be a square number" << endl;
        return;
    } // check if n is a square number
}

void solve()
{
    for(int i = 1; i <= n*n*n; i++)
    {
        int x; cin >> x;
        if(x < 0) continue;
        Node cur = decode(x);
        sdk[cur.i][cur.j] = cur.k;
    }
}

void print()
{
    for(int i = 1; i <= n; i++)
    {
        for(int j = 1; j <= n; j++)
        {
            cout << sdk[i][j] << " ";
        }
        cout << endl;
    }
}

signed main()
{
    ios::sync_with_stdio(0); cin.tie(0); cout.tie(0);
    int t = 1;
    while(t--)
    {
        input();
        solve();
        print();
    }
    return 0;
}