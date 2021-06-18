#include <bits/stdc++.h>

#include "tsp/algorithms.h"
#include "utils.h"

using namespace std;

// distance = time to move

const int kSeed = 0;
// const int kTry = 20; // number of iteration to try to swap and move

mt19937 rng(kSeed);

double GetRateOnSingleCircle(const vector<int> &cluster) {
  auto sum_consuming_rate = 0;
  auto max_consuming_rate = 0;
  for (const auto &id : cluster) {
    sum_consuming_rate += sensors[id].consuming_rate;
    max_consuming_rate = max(max_consuming_rate, sensors[id].consuming_rate);
  }
  WriteToFile(cluster, kInput, true);
  tsp test(kInput.c_str());            // read in command line input
  auto t_m = test.nearest_neighbor();  // run nearest neighbor function
  // cerr << t_m << endl;

  if (double(to_sensor_rate) / sum_consuming_rate - 1 -
          double(from_mc_rate) / to_mc_rate <=
      0) {
    return 1e9;
  }

  double numer = double(t_m * (1 + double(moving_rate) / to_mc_rate)) /
                 (double(to_sensor_rate) / sum_consuming_rate - 1 -
                  double(from_mc_rate) / to_mc_rate);

  double denom = min(double(e_max * sum_consuming_rate) /
                         (max_consuming_rate * to_sensor_rate),
                     double(e_mc_max - t_m * moving_rate) / from_mc_rate);

  if (denom <= 0) return 1e9;
  // cout << fixed << setprecision(2) << numer << ' ' << denom << endl;
  // if (numer < denom) {
  //   cerr << "both" << endl;
  //   cerr << numer << endl;
  //   cerr << double(e_max * sum_consuming_rate) /(max_consuming_rate * to_sensor_rate) << endl;
  //   cerr << double(e_mc_max - t_m * moving_rate) / from_mc_rate << endl;
  // }
  return numer / denom;
}

double GetRateOnSingleCluster(vector<int> cluster) {
  // sorted descending by consuming_rate (p_i)
  sort(cluster.begin(), cluster.end(), [&](int u, int v) {
    return sensors[u].consuming_rate > sensors[v].consuming_rate;
  });

  // auto sum_consuming_rate = 0;
  // auto max_consuming_rate = 0;

  // double last_circle_time = 0;
  // double last_wait_time = 0;

  // vector<int> current;

  // total time a sensor has to wait
  double tot_time = 0;
  double min_time = 1e18;

  int start = 0;
  while (start < (int)cluster.size()) {
    double last_circle_time = 0;
    double last_wait_time = 0;
    int low = start, high = (int)cluster.size() + 1;
    while (high - low > 1) {
      int mid = (low + high) >> 1;
      auto sum_consuming_rate = 0;
      auto max_consuming_rate = 0;
      vector<int> current;
      for (int i = start; i < mid; i++) {
        int id = cluster[i];
        sum_consuming_rate += sensors[id].consuming_rate;
        max_consuming_rate =
            max(max_consuming_rate, sensors[id].consuming_rate);
        current.push_back(id);
      }

      WriteToFile(current, kInput, true);
      tsp test(kInput.c_str());            // read in command line input
      auto t_m = test.nearest_neighbor();  // run nearest neighbor function
      double foo = double(t_m * (1 + double(moving_rate) / to_mc_rate));
      double bar = double(to_sensor_rate) / sum_consuming_rate - 1 -
                   double(from_mc_rate) / to_mc_rate;
      double numer = foo / bar;

      double denom = min(double(e_max * sum_consuming_rate) /
                             (max_consuming_rate * to_sensor_rate),
                         double(e_mc_max - t_m * moving_rate) / from_mc_rate);

      if (numer > denom) {
        high = mid;
      } else {
        last_circle_time =
            denom + t_m +
            (denom * from_mc_rate + t_m * moving_rate) / to_mc_rate;
        last_wait_time = denom * to_sensor_rate / sum_consuming_rate;
        low = mid;
      }
    }

    if (low == start) {
      return 1e18;
    }
    tot_time += last_circle_time;
    min_time = min(min_time, last_wait_time);
    if (tot_time > min_time) return tot_time / min_time;
    start = low;

    // auto id = cluster[i];

    // auto new_sum_consuming_rate = sum_consuming_rate +
    // sensors[id].consuming_rate; auto new_max_consuming_rate =
    // max(max_consuming_rate, sensors[id].consuming_rate);

    // current.push_back(id);
    // WriteToFile(current, kInput, true);
    // tsp test(kInput.c_str());              // read in command line input
    // auto t_m = test.nearest_neighbor();        // run nearest neighbor
    // function double foo = double(t_m * (1 + double(moving_rate) /
    // to_mc_rate)); double bar = double(to_sensor_rate) /
    // new_sum_consuming_rate - 1 - double(from_mc_rate) / to_mc_rate; double
    // numer = foo / bar;

    // double denom = min(
    //   double(e_max * new_sum_consuming_rate) / (new_max_consuming_rate *
    //   to_sensor_rate), double(e_mc_max - t_m * moving_rate) / from_mc_rate
    // );

    // if (numer > denom) {
    //   // can't even charge 1 sensor
    //   if (!sum_consuming_rate) {
    //     return 1e9;
    //   }
    //   tot_time += last_circle_time;
    //   min_time = min(min_time, last_wait_time);

    //   current.clear();
    //   sum_consuming_rate = 0;
    //   max_consuming_rate = 0;
    //   last_circle_time = 0;
    //   last_wait_time = 0;
    //   --i;
    // } else {
    //   sum_consuming_rate = new_sum_consuming_rate;
    //   max_consuming_rate = new_max_consuming_rate;
    //   last_circle_time = denom + t_m + (denom * from_mc_rate + t_m *
    //   moving_rate) / to_mc_rate; last_wait_time = denom * to_sensor_rate /
    //   sum_consuming_rate;
    // }
  }
  // tot_time += last_circle_time;
  // min_time = min(min_time, last_wait_time);
  return tot_time / min_time;
}

double GetRate(const vector<vector<int>> &clusters,
               vector<double> &rate_vector) {
  double res = 0.0;
  int cnt = 0;
  for (auto cluster : clusters) {
    auto rate = GetRateOnSingleCircle(cluster);
    res = max(res, rate);
    rate_vector[cnt++] = rate;
  }
  return res;
}

// vector<vector<int>> SwapClusters(vector<vector<int>> clusters) {
//   int u = rng() % (int)clusters.size();
//   int v = rng() % ((int)clusters.size() - 1);
//   if (u == v) v++;
//   int id_in_u = rng() % (int)clusters[u].size();
//   int id_in_v = rng() % (int)clusters[v].size();
//   swap(clusters[u][id_in_u], clusters[v][id_in_v]);
//   return clusters;
// }

vector<vector<int>> MoveClusters(vector<vector<int>> clusters,
                                 const vector<double> &rate_vector,
                                 bool use_rate_vector = true) {
  int u = 0;
  int v = 0;
  if (use_rate_vector) {
    u = max_element(rate_vector.begin(), rate_vector.end()) -
        rate_vector.begin();
    if (clusters[u].size() == 1) {
      // cerr << "rate " << fixed << setprecision(5) << rate_vector[u] << endl;
      // cerr << "node " << clusters[u][0] << endl;
      // cerr << "can't even support only 1 node" << endl;

      // auto sum_consuming_rate = sensors[clusters[u][0]].consuming_rate;

      // cout << "rate ";
      // cout << double(to_sensor_rate) / sum_consuming_rate - 1 -
      //             double(from_mc_rate) / to_mc_rate
      //      << endl;
    }
    // assert(clusters[u].size() > 1);
    if (clusters[u].size() <= 1) {
      cout << number_of_sensors;
      exit(0);
    }
    vector<int> min_id;
    for (int i = 0; i < 5; i++) {
      int cur = -1;
      for (int j = 0; j < (int)rate_vector.size(); j++) {
        if (find(min_id.begin(), min_id.end(), j) != min_id.end() || j == u)
          continue;
        if (cur == -1)
          cur = j;
        else {
          if (rate_vector[j] < rate_vector[cur]) {
            cur = j;
          }
        }
      }
      if (cur == -1) break;
      min_id.push_back(cur);
    }
    if (min_id.size() == 0) {
      assert(0);
    }
    v = min_id[rng() % min_id.size()];
    assert(u != v);
  } else {
    while (1) {
      u = rng() % (int)clusters.size();
      if (clusters[u].size() > 1) break;
    }
    v = rng() % ((int)clusters.size() - 1);
    if (u == v) v++;
  }

  int id_in_u = rng() % (int)clusters[u].size();
  swap(clusters[u][id_in_u], clusters[u].back());
  clusters[v].push_back(clusters[u].back());
  clusters[u].pop_back();
  return clusters;
}

vector<vector<int>> res;

// check if we can arrange `m` MCs that our network is working perpetually
bool CanArrange(int m) {
  vector<int> order(number_of_sensors, 0);
  iota(order.begin(), order.end(), 0);
  // sort(order.begin(), order.end(), [&](int u, int v) {
  //   return sensors[u].consuming_rate > sensors[v].consuming_rate;
  // });
  shuffle(order.begin(), order.end(), rng);

  auto cluster_size = number_of_sensors / m;
  auto number_of_big_clusters = number_of_sensors % m;
  vector<vector<int>> clusters(m, vector<int>());
  auto cur_id = 0;
  for (int i = 0; i < m; i++) {
    for (int j = 0; j < cluster_size + (i < number_of_big_clusters); j++) {
      clusters[i].push_back(order[cur_id++]);
    }
  }
  vector<double> rate_vector(m, 0);
  auto rate = GetRate(clusters, rate_vector);
  auto best_clusters = clusters;
  auto best_rate = rate;
  // auto starting_time = clock();
  auto num_try = 0;
  if (m > 1 && rate >= 1.0) {
    while (num_try < 5 && rate >= 1.0) {
      bool found = false;

      // for (int it = 0; it < number_of_sensors && rate > 0.99; ++it) {
      //   auto swap_clusters = SwapClusters(clusters);
      //   auto swap_rate = GetRate(swap_clusters);
      //   if (rate > swap_rate) {
      //     rate = swap_rate;
      //     clusters = swap_clusters;
      //     found = 1;
      //   }
      // }

      for (int it = 0; it < 10 && rate >= 1.0; ++it) {
        auto move_clusters = MoveClusters(clusters, rate_vector);
        vector<double> move_rate_vector(m, 0);
        auto move_rate = GetRate(move_clusters, move_rate_vector);
        if (rate > move_rate) {
          rate = move_rate;
          clusters = move_clusters;
          rate_vector = move_rate_vector;
          found = 1;
        }
      }

      if (found) {
        if (rate < best_rate) {
          best_rate = rate;
          best_clusters = clusters;
        }
      } else {
        for (int it = 0; it < 5; it++) {
          // if (rng() % 2) clusters = SwapClusters(clusters);
          // else clusters = MoveClusters(clusters);
          clusters = MoveClusters(clusters, rate_vector, false);
        }

        num_try++;
        if (num_try == 5) shuffle(order.begin(), order.end(), rng);
        // clusters = vector<vector<int>>(m, vector<int>());
        // auto cur_id = 0;
        // for (int i = 0; i < m; i++) {
        //   for (int j = 0; j < cluster_size + (i < number_of_big_clusters);
        //   j++) {
        //     clusters[i].push_back(order[cur_id++]);
        //   }
        // }

        rate = GetRate(clusters, rate_vector);
        if (rate < best_rate) {
          best_rate = rate;
          best_clusters = clusters;
        }
      }
    }
  }
  // cerr << fixed << setprecision(10) << rate << endl;
  if (rate < 1.0) {
    res = clusters;
  }
  return rate < 1.0;
}

int main() {
  ios_base::sync_with_stdio(0);
  cin.tie(0);
  string file_path = "data/data_foo.txt";
  ReadInput(file_path);
  int low = 0, high = number_of_sensors + 1;
  while (high - low > 1) {
    // cerr << "low = " << low << " high = " << high << endl;
    int mid = (low + high) / 2;
    if (CanArrange(mid))
      high = mid;
    else
      low = mid;
  }
  cout << high;
  // int cnt = 0;
  // for (auto cluster : res) {
  //   cout << "# " << ++cnt << endl;

  //   // auto sum_consuming_rate = 0;
  //   // for (const auto &id : cluster) {
  //   //   sum_consuming_rate += sensors[id].consuming_rate;
  //   // }
  //   // cout << "elements: \n";

  //   for (auto u : cluster)
  //     cout << u << ' ' << sensors[u].consuming_rate << endl;
  //   // cout << "rate ";
  //   // cout << double(to_sensor_rate) / sum_consuming_rate - 1 -
  //   //             double(from_mc_rate) / to_mc_rate << endl;
  // }
  return 0;
}