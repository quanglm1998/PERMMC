#include <bits/stdc++.h>

#include "tsp/algorithms.h"
#include "utils.h"

using namespace std;

bool FindCircle(vector<int> &list_sensors, double &t_sum, double &t_remain) {
  vector<int> cur_circle;
  double cur_dist = 0;
  double m_m = kInf;

  while (!list_sensors.empty()) {
    sort(list_sensors.begin(), list_sensors.end(), [&](int u, int v) {
      Sensor &uu = sensors[u];
      Sensor &vv = sensors[v];
      double Wu = (double)uu.consuming_rate / cur_circle.empty()
                      ? GetDistance(Sensor::GetDummy(), uu)
                      : GetDistance(sensors[cur_circle.back()], uu);
      double Wv = (double)vv.consuming_rate / cur_circle.empty()
                      ? GetDistance(Sensor::GetDummy(), vv)
                      : GetDistance(sensors[cur_circle.back()], vv);
      return Wu < Wv;
    });

    int id = list_sensors.back();
    double tot_dist = cur_dist;
    tot_dist += cur_circle.empty()
                    ? 2.0 * GetDistance(Sensor::GetDummy(), sensors[id])
                    : GetDistance(sensors[cur_circle.back()], sensors[id]) +
                          GetDistance(sensors[id], Sensor::GetDummy());
    // cur_circle.empty() ? 0.0 : GetDistance(sensors[cur_circle.back()],
    // sensors[id]) +
    //                            GetDistance(sensors[cur_circle[0]],
    //                            sensors[id]);

    double new_m_m = min(m_m, (double)e_max / sensors[id].consuming_rate);
    double new_t_m =
        (double)(cur_circle.size() + 1) * e_max / to_sensor_rate + tot_dist;

    double e_trv = tot_dist * moving_rate;

    double e_tot = e_trv + (double)(cur_circle.size() + 1) * e_max;

    bool good = true;
    good &= new_t_m <= new_m_m;
    // good &= new_t_m <= t_remain;
    good &= e_tot <= e_mc_max;
    good &= new_t_m + t_sum <= min(new_m_m, t_remain);

    if (!good) break;
    list_sensors.pop_back();
    cur_circle.push_back(id);

    WriteToFile(cur_circle, kInput, true);
    tsp test(kInput.c_str());  // read in command line input
    test.nearest_neighbor();   // run nearest neighbor function
    vector<int> perm = ReadResult(KOuput);
    rotate(perm.begin(), max_element(perm.begin(), perm.end()), perm.end());
    perm.erase(perm.begin());
    vector<int> new_circle = Permutate(cur_circle, perm);
    double new_dist = 0;
    for (int i = 0; i + 1 < (int)cur_circle.size(); ++i) {
      new_dist +=
          GetDistance(sensors[cur_circle[i]], sensors[cur_circle[i + 1]]);
    }
    // new_dist += GetDistance(sensors[cur_circle[0]],
    // sensors[cur_circle.back()]);
    new_dist += GetDistance(Sensor::GetDummy(), sensors[cur_circle[0]]);
    new_dist += GetDistance(Sensor::GetDummy(), sensors[cur_circle.back()]);

    if (new_dist < tot_dist) {
      cur_circle = new_circle;
      cur_dist = new_dist -
                 GetDistance(Sensor::GetDummy(), sensors[cur_circle.back()]);
      ;
    } else {
      if (cur_circle.size() > 1) {
        cur_dist += GetDistance(sensors[cur_circle.back()],
                                sensors[cur_circle[cur_circle.size() - 2]]);
      } else {
        cur_dist += GetDistance(Sensor::GetDummy(), sensors[cur_circle.back()]);
      }
    }
    m_m = new_m_m;
  }

  if (cur_circle.empty()) return false;
  auto tot_dist =
      cur_dist + GetDistance(Sensor::GetDummy(), sensors[cur_circle.back()]);
  double new_t_m =
      (double)cur_circle.size() * e_max / to_sensor_rate + tot_dist;
  // t_remain = min(t_remain - new_t_m, m_m - new_t_m);
  t_remain = min(t_remain, m_m);
  t_sum += new_t_m;
  return true;
}

bool FindPath(vector<int> &list_sensors) {
  int num_circle = 0;
  double t_remain = kInf;
  double t_sum = 0;
  while (1) {
    if (!FindCircle(list_sensors, t_sum, t_remain)) break;
    num_circle++;
  }
  return num_circle > 0;
}

int main(int argc, char const *argv[]) {
  ios_base::sync_with_stdio(0);
  cin.tie(0);
  string file_path = "data/data_foo.txt";
  ReadInput(file_path);

  int num_mc = 0;
  vector<int> list_sensors(number_of_sensors);
  iota(list_sensors.begin(), list_sensors.end(), 0);

  while (!list_sensors.empty()) {
    if (!FindPath(list_sensors)) {
      // cout << "Can't find solution!\n";
      cout << number_of_sensors;
      return 0;
    }
    num_mc++;
  }
  cout << num_mc;
  return 0;
}
