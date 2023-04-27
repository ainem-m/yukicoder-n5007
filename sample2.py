import heapq

INF = 1000000000

# ========================
#       入力読み込み
# ========================
n, m = map(int, input().split())

# 各経由地の座標
points = []

for _ in range(n):
    x, y = map(int, input().split())
    points.append((x, y))

# 宇宙ステーションの座標は適当に決め打ち
#
#   x  x  x
#   x     x
#   x  x  x
#
# みたいにする
points.append((300, 300))
points.append((300, 500))
points.append((300, 700))
points.append((500, 300))
points.append((500, 700))
points.append((700, 300))
points.append((700, 500))
points.append((700, 700))


# ========================
#    ワーシャルフロイド
# ========================


def calc_energy(i, j):
    """
    2点間の消費エネルギーを計算する関数
    """
    x1, y1 = points[i]
    x2, y2 = points[j]
    dx = x1 - x2
    dy = y1 - y2
    energy = dx * dx + dy * dy

    # 惑星かどうかは i < n で判定可能
    if i < n:
        energy *= 5
    if j < n:
        energy *= 5
    return energy


distances = [[0] * len(points) for _ in range(len(points))]

# 全点間エネルギーを計算
for i in range(len(points)):
    for j in range(len(points)):
        distances[i][j] = calc_energy(i, j)

# ワーシャルフロイド
for k in range(len(points)):
    for i in range(len(points)):
        for j in range(len(points)):
            d = distances[i][k] + distances[k][j]
            distances[i][j] = min(distances[i][j], d)


# ========================
#    経路の作成（貪欲法）
# ========================


def dijkstra(i, j):
    """
    ダイクストラを行い、経由点iから経由点jへの最短経路を復元する関数
    """
    dijkstra_dist = [INF for _ in range(len(points))]

    # 1つ前にいた頂点を保存する配列（経路復元用）
    prev_points = [-1 for _ in range(len(points))]

    # (距離, 頂点)のペアをpush
    queue = [(0, i)]
    dijkstra_dist[i] = 0

    while queue:
        d, v = heapq.heappop(queue)
        if d > dijkstra_dist[v]:
            continue

        for next in range(len(points)):
            next_d = d + calc_energy(v, next)
            if next_d < dijkstra_dist[next]:
                # 1つ前の頂点を保存しておく
                prev_points[next] = v
                dijkstra_dist[next] = next_d
                heapq.heappush(queue, (next_d, next))

    # ここから経路復元
    # ゴールから1つずつ頂点を辿っていく
    # パンくずみたいな感じ
    v = j
    path = []

    # スタートに戻るまでループ
    while v != i:
        path.append(v)
        v = prev_points[v]

    # pathには経路が逆順で入っているのでひっくり返す
    path.reverse()

    return path


# 惑星1から出発し、一番近い惑星を貪欲に選び続ける（Nearest Neighbour法）
v = 0
visited = [False] * n
visited[0] = True
route = [0]

# 惑星1以外のN-1個の惑星を訪問していく
for _ in range(n - 1):
    nearest_dist = INF
    nearest_v = -1

    # 一番近い惑星を探す
    for next in range(n):
        if visited[next]:
            continue

        if distances[v][next] < nearest_dist:
            nearest_dist = distances[v][next]
            nearest_v = next

    # パスを復元
    path = dijkstra(v, nearest_v)
    route.extend(path)

    # 次の頂点に移動
    v = nearest_v
    visited[v] = True

# 最後に惑星1に戻る必要がある
path = dijkstra(v, 0)
route.extend(path)


# ========================
#         解の出力
# ========================


# 宇宙ステーションの座標を出力
for x, y in points[n:]:
    print(f"{x} {y}")

# 経路の長さを出力
print(len(route))

# 経路を出力
for v in route:
    if v < n:
        print(f"1 {v + 1}")
    else:
        print(f"2 {v - n + 1}")
