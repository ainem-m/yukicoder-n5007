#include <bits/stdc++.h>
using namespace std;

template <typename T>
bool chmin(T &a, const T &b)
{
    if (a > b)
    {
        a = b;
        return true;
    }
    return false;
}

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

/**
 *  ダイクストラを行い、経由点iから経由点jへの最短経路を復元する関数
 */
vector<int> dijkstra(int i, int j)
{
    vector<int> dijkstra_dist(points.size(), INF);

    // 1つ前にいた頂点を保存する配列（経路復元用）
    vector<int> prev_points(points.size(), -1);

    // (距離, 頂点)のペアをpush
    priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> queue;
    queue.emplace(0, i);
    dijkstra_dist[i] = 0;

    while (!queue.empty())
    {
        auto [d, v] = queue.top();
        queue.pop();
        if (d > dijkstra_dist[v])
        {
            continue;
        }

        for (int next = 0; next < points.size(); next++)
        {
            int next_d = d + calc_energy(v, next);
            if (next_d < dijkstra_dist[next])
            {
                // 1つ前の頂点を保存しておく
                prev_points[next] = v;
                dijkstra_dist[next] = next_d;
                queue.emplace(next_d, next);
            }
        }
    }

    // ここから経路復元
    // ゴールから1つずつ原点を辿っていく
    // パンくずみたいな感じ
    int v = j;
    vector<int> path;

    while (v != i)
    {
        path.push_back(v);
        v = prev_points[v];
    }

    // pathには経路が逆順で入っているのでひっくり返す
    reverse(path.begin(), path.end());

    return path;
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

    // ワーシャルフロイド
    vector<vector<int>> distances(points.size(), vector<int>(points.size(), 0));

    // 全点間エネルギーを計算
    for (int i = 0; i < points.size(); i++)
    {
        for (int j = 0; j < points.size(); j++)
        {
            distances[i][j] = calc_energy(i, j);
        }
    }

    // ワーシャルフロイド
    for (int k = 0; k < points.size(); k++)
    {
        for (int i = 0; i < points.size(); i++)
        {
            for (int j = 0; j < points.size(); j++)
            {
                int d = distances[i][k] + distances[k][j];
                chmin(distances[i][j], d);
            }
        }
    }

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
            if (distances[v][next] < nearest_dist)
            {
                nearest_dist = distances[v][next];
                nearest_v = next;
            }
        }
        // パスを復元
        vector<int> path = dijkstra(v, nearest_v);
        // pythonでいうextend, rustでいうappendの挙動をする関数がわからなくて…
        for (auto p : path)
        {
            route.push_back(p);
        }
        // 次の頂点に移動
        v = nearest_v;
        visited[v] = true;
    }
    // 最後に惑星1に戻る必要がある
    vector<int> path = dijkstra(v, 0);
    for (auto p : path)
    {
        route.push_back(p);
    }

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
