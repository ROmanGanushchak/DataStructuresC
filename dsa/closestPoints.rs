#[derive(Debug)]
struct Point {
    x: i32,
    y: i32
}

impl Point {
    fn distance(&self, p: &Point) -> u32 {
        ((self.x - p.x).pow(2) + (self.y - p.y).pow(2)) as u32
    }
}

struct Solution {
    xpoints: Vec<Point>,
    yidxs: Vec<usize>,
    d: u32
}

fn lower_bound(x: &Vec<Point>, val: i32, mut l: usize, mut r: usize) -> usize {
    while l < r {
        let m = (l + r) / 2;
        if x[m].x < val {
            l = m + 1;
        } else {
            r = m;
        }
    }
    l
}

impl Solution {
    fn new (mut x: Vec<Point>) -> Self {
        x.sort_by(|a, b| a.x.cmp(&b.x));
        
        let mut y_ordered: Vec<usize> = (0..x.len()).collect();
        y_ordered.sort_by(|&a, &b| x[a].y.cmp(&x[b].y));

        Solution {
            xpoints: x,
            yidxs: y_ordered,
            d: u32::MAX
        }
    }

    fn _solve(&mut self, l: usize, r: usize) {
        if r - l <= 4 {
            let points = &mut self.xpoints;
            for i in l..r {
                for j in (i+1)..=r {
                    self.d = self.d.min( points[i].distance(&points[j]) );
                }
            }
            return;
        }

        let m = (l + r) / 2;
        assert!(l != m && r != m+1);
        self._solve(l, m);
        self._solve(m+1, r);
        let points = &mut self.xpoints;

        let mid_val: i32 = (points[m].x + points[m+1].x)/2;
        let edges: (i32, i32) = (mid_val - self.d as i32, mid_val + self.d as i32);
        let edges: (usize, usize) = (
            lower_bound(points, edges.0, l, r), 
            lower_bound(points, edges.1 + 1, l, r)
        );

        let mut q: Vec<usize> = Vec::new();
        for i in &self.yidxs {
            let i = *i;
            if i > edges.1 || i < edges.0 { continue; }
            for j in q.len().saturating_sub(13)..q.len() {
                let j = q[j];
                assert!(i != j);
                self.d = self.d.min( points[i].distance(&points[j]) );
            }
            q.push(i);
        }
    }

    fn solve(&mut self) -> f64 {
        self.d = u32::MAX;
        self._solve(0, self.xpoints.len()-1);

        (self.d as f64).sqrt()
    }
}

fn main() {    
    let x: Vec<Point> = vec![Point {x: 0, y: 0}, Point {x: 1, y: 1}, Point {x: 2, y: 2}, Point {x: 3, y: 3}, Point {x: 4, y: 4}, Point {x: 10, y: 10}, Point {x: 5, y: 5}, Point {x: 10, y: 0}, Point {x: 0, y: 10}, Point {x: 20, y: 0}, Point {x: 0, y: 20}, Point {x: 30, y: 0}, Point {x: 0, y: 30}, Point {x: 50, y: 50}, Point {x: 0, y: 2}];
    let mut s = Solution::new(x);

    print!("The solution: {}\n", s.solve());
}
