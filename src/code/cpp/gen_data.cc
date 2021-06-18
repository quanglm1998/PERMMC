#include <bits/stdc++.h>

#include "utils.h"

using namespace std;

/* 
 * add a BASE-STATION as a dummy sensor indexed 0
 * ensure that the map is CONNECTED by changing kConnectionRange large enough
 */

// const unsigned int kSeed = 0; // for randomizing
const int kSize = 250; // kSize * kSize plane with (0,0) being the BS
const int kConnectionRange = 160; // radius of a sensor, ensure that the map is CONNECTED
int kNumberOfSensors = 100;

int kDefaultConsumingRate = 5;
int kEMax = 100'000;
int kEMcMax = 5'000'000;
const int kToSensorRate = 400; // r
const int kFromMcRate = 401; // r_c
const int kMovingRate = 20; // r_m
const int kToMcRate = 80'000; // r_mc

void VisualizeMap() {
  vector<vector<char>> matrix(kSize * 2 + 1, vector<char>(kSize * 2 + 1, '.'));
  for (auto sensor : sensors) {
    matrix[sensor.x + kSize][sensor.y + kSize] = '*';
  }
  matrix[kSize][kSize] = '0';
  for (auto row : matrix) {
    for (auto element : row) {
      cout << element;
    }
    cout << endl;
  }
}

void GeneratePointList() {
  vector<Sensor> point_list;
  point_list.push_back({0, 0, 0});
  for (int x = -kSize; x <= kSize; x++) {
    for (int y = -kSize; y <= kSize; y++) {
      if (x || y) point_list.push_back({x, y, 0});
    }
  }
  random_shuffle(point_list.begin() + 1, point_list.end());
  sensors = vector<Sensor>(point_list.begin(), point_list.begin() + kNumberOfSensors + 1);
}

bool Dijkstra() {
  set<pair<double, int>> s;
  vector<double> dist(sensors.size(), kInf);
  vector<bool> used(sensors.size(), 0);
  vector<int> pre(sensors.size(), 0);

  dist[0] = 0;
  s.insert({0, 0});
  pre[0] = -1;
  vector<int> list;
  while (!s.empty()) {
    auto u = s.begin()->second;
    s.erase(s.begin());
    if (used[u]) continue;
    used[u] = 1;
    list.push_back(u);
    for (int i = 0; i <= kNumberOfSensors; i++) {
      if (!used[i] && GetDistance(sensors[u], sensors[i]) <= kConnectionRange &&
          dist[i] > dist[u] + GetDistance(sensors[u], sensors[i])) {

        dist[i] = dist[u] + GetDistance(sensors[u], sensors[i]);
        s.insert({dist[i], i});
        pre[i] = u;
      }
    }
  }

  // for (int i = 0; i <= kNumberOfSensors; i++) {
  //   if (!used[i]) {
  //     cout << i << endl;
  //   }
  // }

  if (!all_of(used.begin(), used.end(), [](bool u) {
    return u;
  })) {
    return false;
  }

  // assert(all_of(used.begin(), used.end(), [](bool u) {
  //   return u;
  // }));

  for (int i = (int)list.size() - 1; i >= 0; i--) {
    auto id = list[i];
    sensors[id].consuming_rate += kDefaultConsumingRate;
    if (pre[id] != -1) sensors[pre[id]].consuming_rate += sensors[id].consuming_rate;
  }
  return true;
}

string GetFilePath() {
  string file_path = "data/data_foo.txt";
  // file_path += '_' + to_string(kSeed);
  // file_path += '_' + to_string(kSize);
  // file_path += '_' + to_string(kConnectionRange);
  // file_path += '_' + to_string(kNumberOfSensors);
  // file_path += '_' + to_string(kDefaultConsumingRate);
  // file_path += ".txt";
  return file_path;
}

void DumpDataToFile() {
  string file_path = GetFilePath();
  fstream ofs(file_path, ofstream::out);
  ofs << kEMax << ' ' << kEMcMax << ' ' << kToSensorRate << ' ' << kFromMcRate << ' '
      << kMovingRate << ' ' << kToMcRate << '\n';

  ofs << kNumberOfSensors << '\n';
  for (int i = 1; i < (int)sensors.size(); i++) {
    auto sensor = sensors[i];
    ofs << sensor.x << ' ' << sensor.y << ' ' << sensor.consuming_rate << '\n';
  }
  ofs.close();
}

// main seed #sensor e_max e_mc_max consuming_rate
 
int main(int argc, char* argv[]) {
  ios_base::sync_with_stdio(0);
  cin.tie(0);
  int seed = atoi(argv[1]);
  kNumberOfSensors = atoi(argv[2]);
  kEMax = atoi(argv[3]);
  kEMcMax = atoi(argv[4]);
  kDefaultConsumingRate = atoi(argv[5]);

  seed ^= kSize;
  seed ^= kConnectionRange;
  seed ^= kNumberOfSensors;
  seed ^= kDefaultConsumingRate;

  seed ^= kEMax;
  seed ^= kEMcMax;
  seed ^= kToSensorRate;
  seed ^= kFromMcRate;
  seed ^= kMovingRate;
  seed ^= kToMcRate;
  srand(seed);
  int it = 0;
  cerr << "generating" << endl;
  while (1) {
    cerr << "it = " << ++it << endl;
    GeneratePointList();
    if (!Dijkstra()) continue;
    DumpDataToFile();
    break;
  }
  return 0;
}