#include <bits/stdc++.h>
using namespace std;

const int INF = 1000000000;

int N,
    M;
vector<pair<int, int>>
    points;

/**
 * 2点間の消費エネルギーを計算する関数
 */
int calc_energy(int i, int j)
{
    int dx, dy;
    int energy;
    auto [x1, y1] = points[i];
    auto [x2, y2] = points[j];
    dx = x1 - x2;
    dy = y1 - y2;
    energy = dx * dx + dy * dy;

    // 惑星かどうかは i < Nで判定可能
    if (i < N)
    {
        energy *= 5;
    }
    if (j < N)
    {
        energy *= 5;
    }
    return energy;
}

int main()
{
    // 入力読み込み
    cin >> N >> M;
    for (int i = 0; i < N; i++)
    {
        int a, b;
        cin >> a >> b;
        points.emplace_back(a, b);
    }

    // 宇宙ステーションの座標は適当に決め打ち
    //
    // x x x
    // x   x
    // x x x
    //
    // みたいにする

    points.emplace_back(300, 300);
    points.emplace_back(300, 500);
    points.emplace_back(300, 700);
    points.emplace_back(500, 300);
    points.emplace_back(500, 700);
    points.emplace_back(700, 300);
    points.emplace_back(700, 500);
    points.emplace_back(700, 700);

    // 経路の作成(貪欲法)

    // 惑星1から出発し、一番近い惑星を貪欲に選び続ける(Nearest Neighbour法)
    int v = 0;
    vector<bool> visited(N, false);
    visited[0] = true;
    vector<int> route{0};

    // 惑星1以外のN-1個の惑星を訪問していく
    for (int i = 0; i < N - 1; i++)
    {
        int nearest_dist = INF;
        int nearest_v = -1;

        // 一番近い惑星を探す
        for (int next = 0; next < N; next++)
        {
            if (visited[next])
            {
                continue;
            }
            int d = calc_energy(v, next);
            if (d < nearest_dist)
            {
                nearest_dist = d;
                nearest_v = next;
            }

            // 次の頂点に移動
            v = nearest_v;
            visited[v] = true;
            route.push_back(nearest_v);
        }
    }
    // 最後に惑星1に戻る必要がある
    route.push_back(0);

    // 解の出力
    // 宇宙ステーションの座標を出力
    for (int i = N; i < N + M; i++)
    {
        cout << points[i].first << " " << points[i].second << endl;
    }
    // 経路の長さを出力
    cout << route.size() << endl;

    // 経路を出力
    for (auto v : route)
    {
        if (v < N)
        {
            cout << "1 " << v + 1 << endl;
        }
        else
        {
            cout << "2 " << v - N + 1 << endl;
        }
    }
}
