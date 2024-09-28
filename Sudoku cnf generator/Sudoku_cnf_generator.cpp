#define _CRT_SECURE_NO_DEPRECATE

#include <bits/stdc++.h>
using namespace std;

int n; // sudoku size
vector <vector <int> > clause; // clauses of the cnf formula
bool exist[1005][1005], row[1005][1005], col[1005][1005]; // check if the cell (i, j) is filled

struct Node{
    int i, j, k;
    Node(int i, int j, int k) : i(i), j(j), k(k) {}
};

int encode(Node cell)
{
    return (cell.i - 1) * n * n + (cell.j - 1) * n + cell.k;
}

Node decode(int val)
{
    int k = val % n; val = (val-k) / n;
    if(k==0) k=4;
    int j = val % n + 1; val = (val-j+1) / n;
    int i = val + 1;
    return Node(i, j, k);
}

void update_cell(int i, int j, int k)
{
    exist[i][j] = true; // update cell (i, j)
    row[i][k] = true; // update row i
    col[j][k] = true; // update column j

    vector <int> single_clause;
    for(int v = 1; v <= n; v++)
    {
        if(v==k) continue;
        single_clause.push_back(-encode(Node(i, j, v)));
        clause.push_back(single_clause);

        single_clause.clear();
    } // update the clauses related to cell (i, j) with value other than k


    for(int v = 1; v <= n; v++)
    {
        if(v==j) continue;
        single_clause.push_back(-encode(Node(i, v, k)));
    }
    clause.push_back(single_clause); // update the clauses related to row i
    single_clause.clear();

    for(int v = 1; v <= n; v++)
    {
        if(v==i) continue;
        single_clause.push_back(-encode(Node(v, j, k)));
    }
    clause.push_back(single_clause); // update the clauses related to column j
    single_clause.clear();

    int sqrt_n = sqrt(n);
    int x = (i - 1) / sqrt_n, y = (j - 1) / sqrt_n;
    int xtop = x * sqrt_n + 1, xbot = x * sqrt_n + sqrt_n, ytop = y * sqrt_n + 1, ybot = y * sqrt_n + sqrt_n;
    for(int u = xtop; u <= xbot; u++)
    {
        for(int v = ytop; v <= ybot; v++)
        {
            if(u==i && v==j) continue;
            single_clause.push_back(-encode(Node(u, v, k)));
        }
    }
    clause.push_back(single_clause); // update the clauses related to block
} // update the logic value of node (i, j, k) and the clauses related to row, column and block. 

void input()
{
    cin >> n;
    
    int sqrt_n = sqrt(n);
    if(sqrt_n*sqrt_n!=n)
    {
        cout << "Invalid size, n has to be a square number" << endl;
        return;
    } // check if n is a square number

    for(int i = 1; i <= n; i++)
    {
        for(int j = 1; j <= n; j++)
        {
            int x; cin >> x;
            if(x) 
            {
                vector <int> single_clause; single_clause.push_back(encode(Node(i, j, x)));
                clause.push_back(single_clause);
                update_cell(i, j, x);
            }
        }
    } // initialize the logic value of the board
}

void update_logic_cell()
{
    for(int i = 1; i <= n; i++)
    {
        for(int j = 1; j <= n; j++)
        {
            if(exist[i][j]) continue;
            vector <int> single_clause;
            for(int k = 1; k <= n; k++)
            {
                for(int _k = 1; _k < k; _k++)
                {
                    if(k==_k) continue;
                    single_clause.push_back(-encode(Node(i, j, k)));
                    single_clause.push_back(-encode(Node(i, j, _k)));
                    clause.push_back(single_clause);
                    single_clause.clear();
                }
            }
        }
    }
} // update the logic value of the cell (i, j), total clause = n ^ 3 * (n - 1) / 2

void update_logic_row()
{
    for(int i = 1; i <= n; i++)
    {
        for(int k = 1; k <= n; k++)
        {
            if(row[i][k]) continue;
            vector <int> single_clause;
            for(int j = 1; j <= n; j++)
            {
                for(int _j = 1; _j <= n; _j++)
                {
                    if(j==_j) continue;
                    single_clause.push_back(-encode(Node(i, j, k)));
                    single_clause.push_back(-encode(Node(i, _j, k)));
                    clause.push_back(single_clause);
                    single_clause.clear();
                }
            }
        }
    }
} // update the logic value of the row i, total clause = n ^ 3 * (n - 1) / 2

void update_logic_col()
{
    for(int j = 1; j <= n; j++)
    {
        for(int k = 1; k <= n; k++)
        {
            if(col[j][k]) continue;
            vector <int> single_clause;
            for(int i = 1; i <= n; i++)
            {
                for(int _i = 1; _i <= n; _i++)
                {
                    if(i==_i) continue;
                    single_clause.push_back(-encode(Node(i, j, k)));
                    single_clause.push_back(-encode(Node(_i, j, k)));
                    clause.push_back(single_clause);
                    single_clause.clear();
                }
            }
        }
    }
} // update the logic value of the column j, total clause = n ^ 3 * (n - 1) / 2

void update_logic_block()
{
    int sqrt_n = sqrt(n);
    for(int x = 0; x < sqrt_n; x++)
    {
        for(int y = 0; y < sqrt_n; y++)
        {
            for(int k = 1; k <= n; k++)
            {
                vector <int> single_clause;
                int xtop = x * sqrt_n + 1, xbot = x * sqrt_n + sqrt_n, ytop = y * sqrt_n + 1, ybot = y * sqrt_n + sqrt_n;
                for(int u = xtop; u <= xbot; u++)
                {
                    for(int v = ytop; v <= ybot; v++)
                    {
                        for(int _u = xtop; _u <= xbot; _u++)
                        {
                            for(int _v = ytop; _v <= ybot; _v++)
                            {
                                if(u==_u && v==_v) continue;
                                single_clause.push_back(-encode(Node(u, v, k)));
                                single_clause.push_back(-encode(Node(_u, _v, k)));
                                clause.push_back(single_clause);
                                single_clause.clear();
                            }
                        }
                    }
                }
            }
        }
    }
} // update the logic value of the block, total clause = n ^ 3 * (n - 1) / 2

void solve()
{
    update_logic_cell();
    update_logic_row();
    update_logic_col();
    update_logic_block();
}

void print_cnf()
{
    cout << "p cnf " << n*n*n << " " << clause.size() << "\n";
    for(auto single_clause: clause)
    {
        for(auto i: single_clause) cout << i << " ";
        cout << 0 << "\n";
    }
}

signed main()
{
    ios::sync_with_stdio(0); cin.tie(0); cout.tie(0);
    freopen("sudoku.cnf", "w", stdout);
    int t = 1;
    while(t--)
    {
        input();
        solve();
        print_cnf();
        cout << endl;
    }
}