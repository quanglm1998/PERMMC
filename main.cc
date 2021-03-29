#include <bits/stdc++.h>
#include "algorithms.h"

using namespace std;

const string kInput = "input.txt";
const string KOuput = "mysolution.txt";

// distance = time to move

const int kSeed = 0;
const int kEMax = 50000;
const int kEMcMax = 70000;
const int to_sensor_rate = 10000; // r
const int from_mc_rate = 10010; // r_c
const int moving_rate = 100; // r_m
const int to_mc_rate = 100000; // r_mc

struct Sensor {
  int x, y;
  int consuming_rate;

  static Sensor GetDummy() {
    return {0, 0, 0};
  }
};

int number_of_sensors;
vector<Sensor> sensors;

void ReadInput(const string &file_path) {
  fstream ifs(file_path, ifstream::in);
  
  ifs >> number_of_sensors;
  sensors.resize(number_of_sensors);
  for (auto &sensor : sensors) {
    ifs >> sensor.x >> sensor.y >> sensor.consuming_rate;
  }

  ifs.close();
}

void WriteToFile(const vector<int> &cluster, const string &file_path) {
  fstream ofs(file_path, ifstream::out);
  int num = 0;
  for (const auto &id : cluster) {
    ofs << num++ << ' ' << sensors[id].x << ' ' << sensors[id].y << '\n';
  }
  ofs.close();
}

double GetRateOnSingleCluster(const vector<int> &cluster) {
  auto sum_consuming_rate = 0;
  auto max_consuming_rate = 0;
  for (const auto &id : cluster) {
    sum_consuming_rate += sensors[id].consuming_rate;
    max_consuming_rate = max(max_consuming_rate, sensors[id].consuming_rate);
  }
  WriteToFile(cluster, kInput);
  tsp test(kInput.c_str());              // read in command line input
  auto t_m = test.nearest_neighbor();        // run nearest neighbor function
  // cerr << t_m << endl;
  double numer = double(t_m * (1 + double(moving_rate) / to_mc_rate)) /
      (double(to_sensor_rate) / sum_consuming_rate - 1 - double(from_mc_rate) / to_mc_rate);
  
  double denom = min(
    double(kEMax * sum_consuming_rate) / (max_consuming_rate * to_sensor_rate),
    double(kEMcMax - t_m * moving_rate) / from_mc_rate
  );
  // cout << fixed << setprecision(2) << numer << ' ' << denom << endl;
  return numer / denom;
}

double GetRate(const vector<vector<int>> &clusters) {
  double res = 0.0;
  for (auto cluster : clusters) {
    res = max(res, GetRateOnSingleCluster(cluster));
  }
  return res;
}

vector<vector<int>> SwapClusters(vector<vector<int>> clusters) {
  int u = rand() % (int)clusters.size();
  int v = rand() % ((int)clusters.size() - 1);
  if (u == v) v++;
  int id_in_u = rand() % (int)clusters[u].size();
  int id_in_v = rand() % (int)clusters[v].size();
  swap(clusters[u][id_in_u], clusters[v][id_in_v]);
  return clusters;
}

vector<vector<int>> MoveClusters(vector<vector<int>> clusters) {
  int u;
  while (1) {
    u = rand() % (int)clusters.size();
    if (clusters[u].size() > 1) break;
  }
  int v = rand() % ((int)clusters.size() - 1);
  if (u == v) v++;

  int id_in_u = rand() % (int)clusters[u].size();
  swap(clusters[u][id_in_u], clusters[u].back());
  clusters[v].push_back(clusters[u].back());
  clusters[u].pop_back();
  return clusters;
}

// check if we can arrange `m` MCs that our network is working perpetually
bool CanArrange(int m) {
  vector<int> order(number_of_sensors, 0);
  iota(order.begin(), order.end(), 0);
  sort(order.begin(), order.end(), [&](int u, int v) {
    return sensors[u].consuming_rate > sensors[v].consuming_rate;
  });
  auto cluster_size = number_of_sensors / m;
  auto number_of_big_clusters = number_of_sensors % m;
  vector<vector<int>> clusters(m, vector<int>());
  auto cur_id = 0;
  for (int i = 0; i < m; i++) {
    for (int j = 0; j < cluster_size + (i < number_of_big_clusters); j++) {
      clusters[i].push_back(order[cur_id++]);
    }
  }
  auto rate = GetRate(clusters);
  auto starting_time = clock();
  if (m > 1) {
    while ((double)(clock() - starting_time) / CLOCKS_PER_SEC < 0.2) {
      auto swap_clusters = SwapClusters(clusters);
      auto swap_rate = GetRate(swap_clusters);
      auto move_clusters = MoveClusters(clusters);
      auto move_rate = GetRate(move_clusters);
      if (rate > swap_rate) {
        rate = swap_rate;
        clusters = swap_clusters;
      }
      if (rate > move_rate) {
        rate = move_rate;
        clusters = move_clusters;
      }
    }
  }
  cerr << fixed << setprecision(10) << rate << endl;
  return rate < 1.0;
}

int main() {
  ios_base::sync_with_stdio(0);
  cin.tie(0);
  srand(kSeed);
  string file_path = "data/data_0_50_20_100_10.txt";
  ReadInput(file_path);
  int low = 0, high = number_of_sensors + 1;
  while (high - low > 1) {
    cerr << "low = " << low << " high = " << high << endl;
    int mid = (low + high) / 2;
    if (CanArrange(mid)) high = mid;
    else low = mid;
  }
  cout << high << endl;
  return 0;
}