#include <bits/stdc++.h>

using namespace std;

/* 
 * add a BASE-STATION as a dummy sensor indexed 0
 * ensure that the map is CONNECTED by changing kConnectionRange large enough
 */

const int kInf = 1e9;

const unsigned int kSeed = 0; // for randomizing
const int kSize = 50; // kSize * kSize plane with (0,0) being the BS
const int kConnectionRange = 20; // radius of a sensor, ensure that the map is CONNECTED
const int kNumberOfSensors = 100;
const int kDefaultConsumingRate = 10;

const int kToSensorRate = 10000; // r
const int kFromMcRate = 10010; // r_c
const int kMovingRate = 100; // r_m
const int kToMcRate = 100000; // r_mc


struct Sensor {
  int x, y;
  int consuming_rate;
};

vector<Sensor> sensors;

double GetDistance(int u, int v) {
  return sqrt(
      1.0 * (sensors[u].x - sensors[v].x) * (sensors[u].x - sensors[v].x) +
      1.0 * (sensors[u].y - sensors[v].y) * (sensors[u].y - sensors[v].y)
  ); 
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
  
  // // print sensor map
  // vector<vector<char>> matrix(kSize * 2 + 1, vector<char>(kSize * 2 + 1, '.'));
  // for (auto sensor : sensors) {
  //   matrix[sensor.x + kSize][sensor.y + kSize] = '*';
  // }
  // matrix[kSize][kSize] = '0';
  // for (auto row : matrix) {
  //   for (auto element : row) {
  //     cout << element;
  //   }
  //   cout << endl;
  // }
}

void Dijkstra() {
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
      if (!used[i] && GetDistance(u, i) <= kConnectionRange &&
          dist[i] > dist[u] + GetDistance(u, i)) {

        dist[i] = dist[u] + GetDistance(u, i);
        s.insert({dist[i], i});
        pre[i] = u;
      }
    }
  }

  for (int i = 0; i <= kNumberOfSensors; i++) {
    if (!used[i]) {
      cout << i << endl;
    }
  }

  assert(all_of(used.begin(), used.end(), [](bool u) {
    return u;
  }));

  for (int i = (int)list.size() - 1; i >= 0; i--) {
    auto id = list[i];
    sensors[id].consuming_rate += kDefaultConsumingRate;
    if (pre[id] != -1) sensors[pre[id]].consuming_rate += sensors[id].consuming_rate;
  }
}

string GetFilePath() {
  string file_path = "data/data";
  file_path += '_' + to_string(kSeed);
  file_path += '_' + to_string(kSize);
  file_path += '_' + to_string(kConnectionRange);
  file_path += '_' + to_string(kNumberOfSensors);
  file_path += '_' + to_string(kDefaultConsumingRate);
  file_path += ".txt";
  return file_path;
}

void DumpDataToFile() {
  string file_path = GetFilePath();
  fstream ofs(file_path, ofstream::out);
  ofs << kNumberOfSensors << '\n';
  for (int i = 1; i < (int)sensors.size(); i++) {
    auto sensor = sensors[i];
    ofs << sensor.x << ' ' << sensor.y << ' ' << sensor.consuming_rate << '\n';
  }
  ofs.close();
}

int main() {
  ios_base::sync_with_stdio(0);
  cin.tie(0);
  srand(kSeed);

  GeneratePointList();
  Dijkstra();
  DumpDataToFile();
  return 0;
}