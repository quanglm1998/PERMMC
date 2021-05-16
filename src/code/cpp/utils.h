#pragma once

#include <bits/stdc++.h>

using namespace std;

const string kInput = "data/input.txt"; 
const string KOuput = "data/mysolution.txt";

const int kInf = 1e9;

int e_max;
int e_mc_max;
int to_sensor_rate; // r
int from_mc_rate; // r_c
int moving_rate; // r_m
int to_mc_rate; // r_mc

struct Sensor {
  int x, y;
  int consuming_rate;

  static Sensor GetDummy() {
    return {0, 0, 0};
  }
};

int number_of_sensors;
vector<Sensor> sensors;

double GetDistance(const Sensor &u, const Sensor &v) {
  return sqrt(
      1.0 * (u.x - v.x) * (u.x - v.x) +
      1.0 * (u.y - v.y) * (u.y - v.y)
  ); 
}

void ReadInput(const string &file_path) {
  fstream ifs(file_path, ifstream::in);

  ifs >> e_max >> e_mc_max >> to_sensor_rate >> from_mc_rate >> moving_rate >> to_mc_rate;

  ifs >> number_of_sensors;
  sensors.resize(number_of_sensors);
  for (auto &sensor : sensors) {
    ifs >> sensor.x >> sensor.y >> sensor.consuming_rate;
  }

  ifs.close();
}

void WriteToFile(const vector<int> &cluster, const string &file_path, bool print_dummy = true) {
  fstream ofs(file_path, ofstream::out);
  int num = 0;
  for (const auto &id : cluster) {
    ofs << num++ << ' ' << sensors[id].x << ' ' << sensors[id].y << '\n';
  }
  if (print_dummy) {
    ofs << num++ << ' ' << 0 << ' ' << 0 << '\n';
  }
  ofs.close();
}

vector<int> ReadResult(const string &file_path) {
  fstream ifs(file_path, ifstream::in);
  int dist;
  ifs >> dist;
  vector<int> res;
  int u;
  while (ifs >> u) res.push_back(u);
  return res;
}

vector<int> Permutate(vector<int> list, const vector<int> &perm) {
  if (list.size() < perm.size()) list.push_back(-1);
  vector<int> res(list.size());
  for (int i = 0; i < (int)perm.size(); i++) {
    res[i] = list[perm[i]];
  };
  return res;
}