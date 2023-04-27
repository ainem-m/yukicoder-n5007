use std::collections::BinaryHeap;

const INF: i32 = 1000000000;

/// 作問者terry_u16さんの提出から頂いた
/// 入力受け取り用のマクロ
macro_rules! get {
      ($t:ty) => {
          {
              let mut line: String = String::new();
              std::io::stdin().read_line(&mut line).unwrap();
              line.trim().parse::<$t>().unwrap()
          }
      };
      ($($t:ty),*) => {
          {
              let mut line: String = String::new();
              std::io::stdin().read_line(&mut line).unwrap();
              let mut iter = line.split_whitespace();
              (
                  $(iter.next().unwrap().parse::<$t>().unwrap(),)*
              )
          }
      };
      ($t:ty; $n:expr) => {
          (0..$n).map(|_|
              get!($t)
          ).collect::<Vec<_>>()
      };
      ($($t:ty),*; $n:expr) => {
          (0..$n).map(|_|
              get!($($t),*)
          ).collect::<Vec<_>>()
      };
      ($t:ty ;;) => {
          {
              let mut line: String = String::new();
              std::io::stdin().read_line(&mut line).unwrap();
              line.split_whitespace()
                  .map(|t| t.parse::<$t>().unwrap())
                  .collect::<Vec<_>>()
          }
      };
      ($t:ty ;; $n:expr) => {
          (0..$n).map(|_| get!($t ;;)).collect::<Vec<_>>()
      };
}

/// Rustでは入力から受け取った変数を
/// グローバルに定義することが難しいので
/// 構造体として持つと便利
#[allow(dead_code)]
#[derive(Debug, Clone)]
struct Input {
    n: usize,
    m: usize,
    points: Vec<Point>,
}

#[derive(Debug, Clone, Copy, PartialEq, Eq, PartialOrd, Ord)]
struct Point {
    x: i32,
    y: i32,
}

impl Point {
    fn new(x: i32, y: i32) -> Self {
        Self { x, y }
    }
    fn dist_sq(&self, other: &Self) -> i32 {
        let dx = self.x - other.x;
        let dy = self.y - other.y;
        (dx * dx + dy * dy) as i32
    }
}

fn main() {
    let input = read_input();
    let mut points = input.points.clone();
    //     宇宙ステーションの座標は適当に決め打ち
    //
    //   x  x  x
    //   x     x
    //   x  x  x
    //
    // みたいにする
    points.push(Point::new(300, 300));
    points.push(Point::new(300, 500));
    points.push(Point::new(300, 700));
    points.push(Point::new(500, 300));
    points.push(Point::new(500, 700));
    points.push(Point::new(700, 300));
    points.push(Point::new(700, 500));
    points.push(Point::new(700, 700));

    // ワーシャルフロイド法で全頂点対の最短経路を求める
    let distances = warshall_floyd(&input, &points);

    // 経路の作成（貪欲法）

    // 惑星1から出発し、一番近い惑星を貪欲に選び続ける（Nearest Neighbour法）
    let mut v = 0;
    let mut visited = vec![false; input.n];
    visited[0] = true;
    let mut route = vec![0];

    // 惑星1以外のN-1個の惑星を訪問していく
    for _ in 0..input.n - 1 {
        let mut nearest_dist = INF;
        let mut nearest_v = !0;

        // 一番近い惑星を探す
        for next in 0..input.n {
            if visited[next] {
                continue;
            }

            if distances[v][next] < nearest_dist {
                nearest_dist = distances[v][next];
                nearest_v = next;
            }
        }

        // パスを復元
        let mut path = dijkstra(&input, &points, v, nearest_v);
        route.append(&mut path);

        // 次の頂点に移動
        v = nearest_v;
        visited[v] = true;
    }

    // 最後に惑星1に戻る必要がある
    let mut path = dijkstra(&input, &points, v, 0);
    route.append(&mut path);

    // 解の出力

    // 宇宙ステーションの座標を出力
    for Point { x, y } in points.iter().skip(input.n) {
        println!("{x} {y}");
    }

    // 経路の長さを出力
    println!("{}", route.len());

    // 経路を出力
    for v in route {
        if v < input.n {
            println!("1 {}", v + 1);
        } else {
            println!("2 {}", v - input.n + 1);
        }
    }
}

/// 入力読み込み
fn read_input() -> Input {
    let (n, m) = get!(usize, usize);

    let mut points = vec![];

    for _ in 0..n {
        let (x, y) = get!(i32, i32);
        points.push(Point::new(x, y));
    }

    Input { n, m, points }
}

/// エネルギー計算
fn calc_energy(input: &Input, points: &Vec<Point>, i: usize, j: usize) -> i32 {
    let mut energy = points[i].dist_sq(&points[j]);
    if i < input.n {
        energy *= 5;
    }
    if j < input.n {
        energy *= 5;
    }
    energy
}

/// ワーシャルフロイド法によって、O(v^3)で全頂点対の最短経路長を求める
fn warshall_floyd(input: &Input, points: &Vec<Point>) -> Vec<Vec<i32>> {
    let mut distances = vec![vec![0; points.len()]; points.len()];
    // 全点間エネルギーを計算
    for i in 0..points.len() {
        for j in 0..points.len() {
            distances[i][j] = calc_energy(input, points, i, j);
        }
    }
    // ワーシャルフロイド
    for k in 0..points.len() {
        for i in 0..points.len() {
            for j in 0..points.len() {
                let d = distances[i][k] + distances[k][j];
                distances[i][j] = distances[i][j].min(d);
            }
        }
    }
    distances
}

/// ダイクストラ法を用いて、経由点iから経由点jへの最短経路を復元する関数
fn dijkstra(input: &Input, points: &Vec<Point>, i: usize, j: usize) -> Vec<usize> {
    let mut dijkstra_dist = vec![INF; points.len()];

    // 1つ前にいた頂点を保存する配列(経路復元用)
    let mut prev_points = vec![!0; points.len()];

    // (経路、頂点)のペアをpush
    let mut queue = BinaryHeap::from(vec![(0, i)]);
    dijkstra_dist[i] = 0;

    while let Some((d, v)) = queue.pop() {
        if d > dijkstra_dist[v] {
            continue;
        }

        for next in 0..points.len() {
            let next_d = d + calc_energy(input, points, v, next);
            if next_d < dijkstra_dist[next] {
                // 1つ前の頂点を保存しておく
                prev_points[next] = v;
                dijkstra_dist[next] = next_d;
                queue.push((next_d, next));
            }
        }
    }

    // ここから経路復元
    // ゴールから1つずつ頂点をたどっていく
    // パンくずみたいな感じ
    let mut v = j;
    let mut path = vec![];

    // スタートに戻るまでループ
    while v != i {
        path.push(v);
        v = prev_points[v];
    }

    // pathには経路が逆順で入っているのでひっくり返す
    path.reverse();

    path
}
