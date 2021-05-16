#include <bits/stdc++.h>

#include "tsp/algorithms.h"
#include "utils.h"

using namespace std;

double GetMinDist(int mask, vector<int> &cluster) {
  double res = 1e18;
  vector<int> min_cluster = cluster;
  while (1) {
    double cur = 0;
    Sensor last = Sensor::GetDummy();
    for (auto id : cluster) {
      cur += GetDistance(last, sensors[id]);
      last = sensors[id];
    }
    cur += GetDistance(last, Sensor::GetDummy());
    if (res > cur) {
      res = cur;
      min_cluster = cluster;
    }
    if (!next_permutation(cluster.begin(), cluster.end())) break;
  }
  cluster = min_cluster;
  return res;
}

int main() {
  ios_base::sync_with_stdio(0);
  cin.tie(0);
  string file_path = "data/data_foo.txt";
  ReadInput(file_path);
  assert(number_of_sensors <= 10);
  vector<bool> can_mc(1 << number_of_sensors, false);
  vector<vector<int>> min_cluster(1 << number_of_sensors, vector<int>());
  can_mc[0] = true;
  for (int mask = 1; mask < (1 << number_of_sensors); ++mask) {
    vector<int> cluster;
    for (int i = 0; i < number_of_sensors; ++i) {
      if (mask >> i & 1) cluster.push_back(i);
    }
    int t_m = GetMinDist(mask, cluster);
    min_cluster[mask] = cluster;
    auto sum_consuming_rate = 0;
    auto max_consuming_rate = 0;
    for (const auto &id : cluster) {
      sum_consuming_rate += sensors[id].consuming_rate;
      max_consuming_rate = max(max_consuming_rate, sensors[id].consuming_rate);
    }

    if (double(to_sensor_rate) / sum_consuming_rate - 1 -
            double(from_mc_rate) / to_mc_rate <=
        0) {
      continue;
    }

    double numer = double(t_m * (1 + double(moving_rate) / to_mc_rate)) /
                   (double(to_sensor_rate) / sum_consuming_rate - 1 -
                    double(from_mc_rate) / to_mc_rate);

    double denom = min(double(e_max * sum_consuming_rate) /
                           (max_consuming_rate * to_sensor_rate),
                       double(e_mc_max - t_m * moving_rate) / from_mc_rate);

    if (denom <= 0) continue;
    can_mc[mask] = numer < denom;
  }

  vector<int> dp(1 << number_of_sensors, 1e9);
  vector<int> trace(1 << number_of_sensors, -1);
  dp[0] = 0;
  for (int mask = 1; mask < (1 << number_of_sensors); ++mask) {
    for (int submask = mask; submask > 0; submask = (submask - 1) & mask) {
      if (can_mc[submask]) {
        if (dp[mask] > 1 + dp[mask ^ submask]) {
          dp[mask] = 1 + dp[mask ^ submask];
          trace[mask] = submask;
        }
      }
    }
  }
  int curmask = (1 << number_of_sensors) - 1;
  cout << dp[curmask] << endl; 
  // int cnt = 0;
  // while (curmask) {
  //   cout << "# " << ++cnt << endl;
  //   int submask = trace[curmask];
  //   for (auto u : min_cluster[submask]) {
  //     cout << u << ' ' << sensors[u].consuming_rate << endl;
  //   }
  //   curmask ^= submask;
  // }
  return 0;
}