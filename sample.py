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
#      エネルギー計算
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


# ========================
#    経路の作成（貪欲法）
# ========================


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

        d = calc_energy(v, next)
        if d < nearest_dist:
            nearest_dist = d
            nearest_v = next

    # 次の頂点に移動
    v = nearest_v
    visited[v] = True
    route.append(nearest_v)

# 最後に惑星1に戻る必要がある
route.append(0)


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
