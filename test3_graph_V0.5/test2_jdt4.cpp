// Copyright 2019 Bo Chen
// test2_jdt4.cpp
// auther: boc
// Time: 2019-11-29
//


#include "./test2_jdt4.h"

#include <iostream>
#include <vector>
#include <utility>
#include <string>
#include <cstring>
#include <fstream>

// #include <fstream>
// #include <string>
// #include <vector>
#include <sstream>
#include <cmath>

#include "./is_in_line.h"
#include "./util.h"

// using namespace std;
using std::cout;
using std::endl;
using std::vector;
using std::swap;
using namespace phoenix::common;

// 最多有效图形的个数
#define MAX_VALID_GRAPH_COUNT 2000

extern double coordx[200];
extern double coordy[200];
static int g_valid_graph_count = 0;
static long long g_valid_graph_tag[MAX_VALID_GRAPH_COUNT] = {0};

struct JDTNode {
  int x;  // row index
  int y;  // column index

  bool operator==(const JDTNode& jdt_node) const {
    return ((x == jdt_node.x) && (y == jdt_node.y));
  }
  bool operator>(const JDTNode& jdt_node) const {
    if (x > jdt_node.x) {
      return true;
    }
    if (x == jdt_node.x && y > jdt_node.y) {
      return true;
    }
    return false;
  }
  bool operator>=(const JDTNode& jdt_node) const {
    if (x > jdt_node.x) {
      return true;
    }
    if (x == jdt_node.x && y >= jdt_node.y) {
      return true;
    }
    return false;
  }
};

struct JDTLineNode {
  JDTNode point_start;
  JDTNode point_end;
  vector<JDTNode> points;
};

struct JDTFeature {
  // (line count, zone count)
  int line_count;
  int zone_count;

  // (convex angle count, concave angle count)
  int convex_angle_count;
  int concave_angle_count;

  // feature1
  vector<int> feature1;

  // feature2
  vector<int> feature2;
};

struct JDT {
  vector<JDTNode> jdt;
  vector<JDTLineNode> lines;
  JDTFeature feature;
};


static bool check_valid_same(const vector<JDTNode>& node_list);
static bool check_valid_order(const vector<JDTNode>& node_list);
static bool check_valid(const vector<JDTNode>& node_list);
static int IncreaseNode(JDTNode *node, int index);
static int get_jdt_line(const JDTNode& node1, const JDTNode& node2,
  const JDTNode& node3, const JDTNode& node4, JDTLineNode *jdt_line);
static bool judge_two_jdt_line_cross(const JDTLineNode& line1,
    const JDTLineNode& line2);
static bool check_valid_no_cross(const vector<JDTNode>& node_list);
static bool check_symmetry_no_repeat(const vector<JDTNode>& node_list);
static int get_jdt_feature(const vector<JDTNode>& node_list,
  JDTFeature *feature);
static bool jdt3_no_repeat_feature(const vector<JDTFeature>& jdt_list_feature,
    const JDTFeature& feature);
static void print_feature(const JDTFeature& f, char* buff, size_t size_buff);
static int main_jdt4_read_from_file();
static int get_jdt_lines(const vector<JDTNode>& node_list, JDTFeature *feature,
    vector< JDTLineNode >* line_list);

static vector<JDTNode> jdt3_node_list;
static vector<JDTNode> node_list_tmp;
static vector<JDT> jdt_graph_list;
static int g_increase_index = JDT4_N - 1;

static int si_dg = 0;
static int si_display = false;

void update4(int index) {
  cout << "Enter into update " << index << endl;
  if (index < MAX_VALID_GRAPH_COUNT && index < g_valid_graph_count) {
    int line_count = jdt_graph_list[index].lines.size();

    cout << "line count is " << line_count << endl;
    cout << "line list: " << endl;

    int start_point = 0;  // g_lines[tag][0];
    int end_point   = 0;  // g_lines[tag][1];
    int line_tag_tmp = 0;
    double offset_x = 10.0;
    double offset_y = 10.0;
    double factor_x = 50.0;
    double factor_y = 50.0;
    g_count = 0;
    for (int i = 0; i < line_count; ++i) {
      coordx[g_count]     = offset_x +
        jdt_graph_list[index].lines[i].point_start.x * factor_x;
      coordy[g_count]     = offset_y +
        jdt_graph_list[index].lines[i].point_start.y * factor_y;
      coordx[g_count + 1] = offset_x +
        jdt_graph_list[index].lines[i].point_end.x * factor_x;
      coordy[g_count + 1] = offset_y +
        jdt_graph_list[index].lines[i].point_end.y * factor_y;
      g_count += 2;
    }

  }
}

int main_jdt4() {
  cout << "Hello, world jdt4! " << endl;
  if (g_option == 10) {
    main_jdt4_read_from_file();
    return 0;
  }
  cout << "points: " << JDT4_N << ", grid_size: " << JDT4_GRID_SIZE << endl;
  long long iterate_size = 0LL;
  int valid_count = 0;
  int i = 0;
  int j = 0;
  int size_tmp = 0;
  int progress_total = 0;
  int progress_count = 0;
  int progress_tag = 0;
  int progress_tag_previous = 0;
  JDTNode node1;
  JDTNode node2;
  JDTNode node3;
  JDTNode node4;
  JDTNode node5;
  vector<JDTFeature> jdt3_list_feature;
  char str_buff_status_[1024*2] = {0};
  char str_buff_status_2[1024*2] = {0};
  char str_buff_tmp[1024*2] = {0};
  char file_name[256] = "jdt4.log";

  {
    // log
    phoenix::common::LogFile log_file_status_(".");
    log_file_status_.Open(file_name);
    log_file_status_.Enable(true);
    snprintf(str_buff_status_, sizeof(str_buff_status_),
      "Hello, world jdt4! \n");
    log_file_status_.Write(str_buff_status_);
    snprintf(str_buff_status_, sizeof(str_buff_status_),
      "points: %d, grid_size: %d\n", JDT4_N, JDT4_GRID_SIZE);
    log_file_status_.Write(str_buff_status_);
  }

  node1.x = 0;
  node1.y = 0;
  node2 = node1;
  node3 = node1;
  node4 = node1;
  node5 = node1;

  for (i = 0; i < JDT4_N; ++i) {
    node_list_tmp.push_back(node1);
  }
#if (JDT4_N == (4 * JDT4_GRID_SIZE))
  node_list_tmp[JDT4_N - 2].x = JDT4_GRID_SIZE - 1;
  node_list_tmp[JDT4_N - 3].x = JDT4_GRID_SIZE - 1;
  node_list_tmp[JDT4_N - 4].x = JDT4_GRID_SIZE - 1;
#endif

  /// test code start
  node_list_tmp[0].x = 0;
  node_list_tmp[0].y = 2;
  node_list_tmp[1].x = 0;
  node_list_tmp[1].y = 3;
  node_list_tmp[2].x = 0;
  node_list_tmp[2].y = 5;
  node_list_tmp[3].x = 0;
  node_list_tmp[3].y = 6;
  node_list_tmp[4].x = 1;
  node_list_tmp[4].y = 1;
  // node_list_tmp[5].x = 6;
  // node_list_tmp[5].y = 4;
  // node_list_tmp[6].x = 6;
  // node_list_tmp[6].y = 6;
  // node_list_tmp[7].x = 6;
  // node_list_tmp[7].y = 6;
  // node_list_tmp[8].x = 2;
  // node_list_tmp[8].y = 0;
  // node_list_tmp[9].x = 2;
  // node_list_tmp[9].y = 1;
  // node_list_tmp[10].x = 2;
  // node_list_tmp[10].y = 3;
  // node_list_tmp[11].x = 2;
  // node_list_tmp[11].y = 4;
  /// test code end

  // 3 point
  progress_total = (JDT4_GRID_SIZE) * (JDT4_GRID_SIZE * JDT4_GRID_SIZE) *
    (JDT4_GRID_SIZE * JDT4_GRID_SIZE);
  // fix the last point in last row
  node_list_tmp[JDT4_N - 1].x = JDT4_GRID_SIZE - 1;
  node_list_tmp[JDT4_N - 1].y = JDT4_GRID_SIZE / 2;
  jdt3_node_list = node_list_tmp;

  // /// test start debug
  // cout << "iterate_size = " << iterate_size << endl;
  // for (j = 0; j < JDT4_N; j++) {
  //   cout << "(" << jdt3_node_list[j].x << ", " <<
  //     jdt3_node_list[j].y << ") ";
  // }
  // cout << endl;
  // /// test end debug

  // while (iterate_size < 2176782336LL) {  // 531441
  while (true) {
    // si_display = false;
    // increase the node list
    size_tmp = node_list_tmp.size();
    if (size_tmp < 3) {
      break;
    }
    progress_tag_previous = progress_tag;
    progress_tag = node_list_tmp[0].y * (JDT4_GRID_SIZE * JDT4_GRID_SIZE) *
      (JDT4_GRID_SIZE * JDT4_GRID_SIZE) + node_list_tmp[1].x * JDT4_GRID_SIZE *
      (JDT4_GRID_SIZE * JDT4_GRID_SIZE) + node_list_tmp[1].y *
      (JDT4_GRID_SIZE * JDT4_GRID_SIZE) + node_list_tmp[2].x * JDT4_GRID_SIZE +
      node_list_tmp[2].y;
    if (progress_tag_previous != progress_tag) {
      progress_count++;
    }
    if (iterate_size % 40000000000LL == 0) {
      // if (progress_count % (progress_total / 1000) == 0) {
        cout << "progress: " << 100.0 * progress_count / progress_total <<
          "% , " << progress_tag << "  " << iterate_size << endl;
        std::snprintf(str_buff_tmp, sizeof(str_buff_tmp) - 1,
                  "progress: %f%% , %d  %lld\n",
                  100.0 * progress_count / progress_total,
                  progress_tag, iterate_size);
        memset(str_buff_status_2, 0, sizeof(str_buff_status_2));
        snprintf(str_buff_status_, sizeof(str_buff_status_), "%s",
          str_buff_tmp);

        cout << "  " << valid_count << " : " << iterate_size << ", ";
        std::snprintf(str_buff_tmp, sizeof(str_buff_tmp) - 1,
                  "  %d , %lld, ", valid_count, iterate_size);
        snprintf(str_buff_status_2, sizeof(str_buff_status_2),
          "%s", str_buff_status_);
        snprintf(str_buff_status_, sizeof(str_buff_status_), "%s%s",
          str_buff_status_2, str_buff_tmp);
        for (j = 0; j < JDT4_N; j++) {
          cout << "(" << jdt3_node_list[j].x << ", " <<
            jdt3_node_list[j].y << ") ";
          std::snprintf(str_buff_tmp, sizeof(str_buff_tmp) - 1,
                  "(%d, %d) ", jdt3_node_list[j].x, jdt3_node_list[j].y);
          snprintf(str_buff_status_2, sizeof(str_buff_status_2),
            "%s", str_buff_status_);
          snprintf(str_buff_status_, sizeof(str_buff_status_), "%s%s",
            str_buff_status_2, str_buff_tmp);
        }
        cout << endl;
        snprintf(str_buff_status_2, sizeof(str_buff_status_2),
            "%s\n", str_buff_status_);
        snprintf(str_buff_status_, sizeof(str_buff_status_), "%s",
          str_buff_status_2);
        {
          // log
          phoenix::common::LogFile log_file_status_(".");
          log_file_status_.Open(file_name);
          log_file_status_.Enable(true);
          log_file_status_.Write(str_buff_status_);
        }
      // }
    }
    // need the graph's height is JDT3_N
    if (node_list_tmp[0].x > 0) {
      break;
    }
    // i = size_tmp - 1;
    i = g_increase_index;
    // int increase_result = 0;
    int increase_result = IncreaseNode(&node_list_tmp[i], i);
    /// test start debug
    if (si_display) {
      cout << "Ln244. iterate_size = " << iterate_size << endl;
      for (j = 0; j < JDT4_N; j++) {
        cout << "(" << jdt3_node_list[j].x << ", " <<
          jdt3_node_list[j].y << ") ";
      }
      cout << endl;
    }
    /// test end debug
    for (i = i - 1; i >= 0; i--) {
      if (increase_result == 1) {
        increase_result = IncreaseNode(&node_list_tmp[i], i);
      }

      /// test start debug
      if (si_display) {
        cout << "Ln259. " << i << " ,  iterate_size = " << iterate_size <<
          endl;
        for (j = 0; j < JDT4_N; j++) {
          cout << "(" << node_list_tmp[j].x << ", " <<
            node_list_tmp[j].y << ") ";
        }
        cout << endl;
      }
      /// test end debug
    }
    if (increase_result > 0) {
      break;
    }

    iterate_size++;
    // if (iterate_size % 1 == 0 && iterate_size > 1) {
    //   si_display = true;
    //   cout << "Ln276. iterate_size = " << iterate_size <<
    //     ", increase_index = " << g_increase_index << endl;
    //   for (j = 0; j < JDT4_N; j++) {
    //     cout << "(" << node_list_tmp[j].x << ", " <<
    //       node_list_tmp[j].y << ") ";
    //   }
    //   if (iterate_size > 10) {
    //     break;
    //   }
    //   cout << endl;
    //   cout << "Ln285" << endl;
    // }
    jdt3_node_list = node_list_tmp;
    // bool idt3_valid_same = check_valid_same(node_list_tmp);
    // if (!idt3_valid_same) {
    //   for (j = g_increase_index + 1; j < (JDT4_N - 1); ++j) {
    //     node_list_tmp[j] = node_list_tmp[j - 1];
    //   }
    //   if (node_list_tmp[JDT4_N - 2].x == (JDT4_GRID_SIZE - 1) &&
    //     node_list_tmp[JDT4_N - 2].y >= node_list_tmp[JDT4_N - 1].y) {
    //     node_list_tmp[JDT4_N - 1].y = node_list_tmp[JDT4_N - 2].y;
    //   } else {
    //     node_list_tmp[JDT4_N - 1].y = JDT4_GRID_SIZE / 2;
    //   }
    //   continue;
    // }
    // cout << "Ln235" << endl;

    /// test start debug
    if (si_display) {
      cout << "Ln295. iterate_size = " << iterate_size << endl;
      for (j = 0; j < JDT4_N; j++) {
        cout << "(" << jdt3_node_list[j].x << ", " <<
          jdt3_node_list[j].y << ") ";
      }
      cout << endl;
    }
    /// test end debug
    bool idt3_valid_order = check_valid_order(node_list_tmp);
    if (!idt3_valid_order) {
      for (j = g_increase_index + 1; j < (JDT4_N - 1); ++j) {
        node_list_tmp[j] = node_list_tmp[j - 1];
      }
      if (node_list_tmp[JDT4_N - 2].x == (JDT4_GRID_SIZE - 1) &&
        node_list_tmp[JDT4_N - 2].y >= node_list_tmp[JDT4_N - 1].y) {
        node_list_tmp[JDT4_N - 1].y = node_list_tmp[JDT4_N - 2].y;
      } else {
        node_list_tmp[JDT4_N - 1].y = JDT4_GRID_SIZE / 2;
      }
      continue;
    }
    /// test start debug
    if (si_display) {
      cout << "Ln309. iterate_size = " << iterate_size << endl;
      for (j = 0; j < JDT4_N; j++) {
        cout << "(" << jdt3_node_list[j].x << ", " <<
          jdt3_node_list[j].y << ") ";
      }
      cout << endl;
    }
    // cout << "Ln240" << endl;
    // cout << "iterate_size = " << iterate_size << endl;
    // for (j = 0; j < JDT4_N; j++) {
    //   cout << "(" << jdt3_node_list[j].x << ", " <<
    //     jdt3_node_list[j].y << ") ";
    // }
    // if (iterate_size > 6586) {
    //   break;
    // }
    // cout << endl;
    // iterate_size++;
    // jdt3_node_list = node_list_tmp;
    si_dg++;
    bool idt3_valid = check_valid(node_list_tmp);
    if (!idt3_valid) {
      // if (si_dg < 3) {
      //   cout << "idt3_valid = false" << endl;

      //   cout << iterate_size << " : ";
      //   for (j = 0; j < JDT3_N; j++) {
      //     cout << "(" << jdt3_node_list[j].x << ", " <<
      //       jdt3_node_list[j].y << ") ";
      //   }
      //   cout << endl;
      // }
      continue;
    }

    // cout << "Ln268" << endl;
    bool idt3_valid_no_cross = check_valid_no_cross(node_list_tmp);
    if (!idt3_valid_no_cross) {
      continue;
    }

    // cout << "Ln274" << endl;

    bool jdt_valid_check_symmetry = check_symmetry_no_repeat(node_list_tmp);
    if (!jdt_valid_check_symmetry) {
      continue;
    }

    // cout << "Ln281" << endl;

    // no repeat
    JDTFeature jdt_feature;
    int feature_get_result = get_jdt_feature(node_list_tmp, &jdt_feature);
    bool jdt3_valid_no_repeat_feature = jdt3_no_repeat_feature(
      jdt3_list_feature, jdt_feature);
    if (!jdt3_valid_no_repeat_feature) {
      // continue;
    }
    jdt3_list_feature.push_back(jdt_feature);

    // valid, print result (8303)
    cout << valid_count << " : " << iterate_size << ", ";
    std::snprintf(str_buff_tmp, sizeof(str_buff_tmp) - 1,
                  "%d : %lld, ", valid_count, iterate_size);
    memset(str_buff_status_2, 0, sizeof(str_buff_status_2));
    snprintf(str_buff_status_, sizeof(str_buff_status_), "%s%s",
      str_buff_status_2, str_buff_tmp);
    // cout << endl << "Ln229. " << str_buff_status_ << endl;
    for (j = 0; j < JDT4_N; j++) {
      cout << "(" << jdt3_node_list[j].x << ", " << jdt3_node_list[j].y << ") ";
      std::snprintf(str_buff_tmp, sizeof(str_buff_tmp)-1,
                  "(%d, %d) ", jdt3_node_list[j].x, jdt3_node_list[j].y);
      // strcat(str_buff_status_, str_buff_tmp);
      snprintf(str_buff_status_2, sizeof(str_buff_status_2),
        "%s", str_buff_status_);
      snprintf(str_buff_status_, sizeof(str_buff_status_), "%s%s",
        str_buff_status_2, str_buff_tmp);
    }
    // cout << endl << "Ln238. " << str_buff_status_ << endl;
    cout << " , ";
    print_feature(jdt_feature, str_buff_tmp, sizeof(str_buff_tmp));
    cout << endl;
    snprintf(str_buff_status_2, sizeof(str_buff_status_2),
        "%s", str_buff_status_);
    snprintf(str_buff_status_, sizeof(str_buff_status_), "%s%s\n",
      str_buff_status_2, str_buff_tmp);

    // cout << "Ln320" << endl;

    {
      // log
      phoenix::common::LogFile log_file_status_(".");
      log_file_status_.Open(file_name);
      log_file_status_.Enable(true);
      log_file_status_.Write(str_buff_status_);
      // log_file_status_.Close();
      // log_file_status_.Open();
    }

    JDT jdt;
    jdt.jdt = node_list_tmp;
    jdt_graph_list.push_back(jdt);

    valid_count++;
    if (valid_count > 2000000) {
      break;
    }
  }

  cout << "complete." << endl;
  cout << iterate_size << ", valid_count = " << valid_count << endl;

  {
    // log
    phoenix::common::LogFile log_file_status_(".");
    log_file_status_.Open(file_name);
    log_file_status_.Enable(true);
    snprintf(str_buff_status_, sizeof(str_buff_status_),
      "complete.\n%lld, valid_count = %d\n", iterate_size, valid_count);
    log_file_status_.Write(str_buff_status_);
  }

  return 0;
}

// valid, return ture; otherwise, return false.
bool check_valid_same(const vector<JDTNode>& node_list) {
  int list_size = node_list.size();
  for (int i = 0; i < list_size; i++) {
    const JDTNode& node1 = node_list[i];
    int j = i + 1;
    if (j < list_size) {
      const JDTNode& node2 = node_list[j];
      // if (node1.x == node2.x && node1.y == node2.y) {
      if (node1 == node2) {
        g_increase_index = j;
        return false;
      }
    }
  }

  g_increase_index = JDT4_N - 1;
  return true;
}

// in order, return ture; otherwise, return false.
bool check_valid_order(const vector<JDTNode>& node_list) {
  int list_size = node_list.size();
  for (int i = 0; i < list_size; i++) {
    const JDTNode& node1 = node_list[i];
    int j = i + 1;
    if (j < list_size) {
      const JDTNode& node2 = node_list[j];
      // if (node1.x > node2.x) {
      //   g_increase_index = j;
      //   return false;
      // } else if (node1.x == node2.x && node1.y > node2.y) {
      //   g_increase_index = j;
      //   return false;
      // }
      if (node1 >= node2) {
        g_increase_index = j;
        return false;
      }
    }
  }

  g_increase_index = JDT4_N - 1;
  return true;
}

int IncreaseNode(JDTNode *node, int index) {
  int return_value = 0;

  /// test start debug
  if (si_display) {
    int j = 0;
    cout << "Ln497. IncreaseNode = " << index << endl;
    for (j = 0; j < JDT4_N; j++) {
      cout << "(" << node_list_tmp[j].x << ", " <<
        node_list_tmp[j].y << ") ";
    }
    cout << endl;
  }
  /// test end debug

  node->y += 1;
  if (node->y >= JDT4_GRID_SIZE) {
    node->y = 0;
    if (index == (JDT4_N - 1)) {
      node->y = JDT4_GRID_SIZE / 2;
      // return 1;
      return_value = 1;
    } else {
      node->x += 1;
      // upper index x in range 0 to grid / 2
      if (index > 0 && index <= ((JDT4_N - 1) / 2)) {
        if (node->x > ((JDT4_GRID_SIZE - 1) / 2)) {
          node->x = 0;
          return_value = 1;
        }
      }
    #if((JDT4_N == (4 * JDT4_GRID_SIZE)) || \
        (JDT4_N == (4 * JDT4_GRID_SIZE - 1)))
      if (index == 1 || index == 2 || index == 3) {
        node->x = 0;
        return_value = 1;
      }
      if (index == 4 || index == 5 || index == 6) {
        if (node->x > 1) {
          node->x = 1;
          return_value = 1;
        }
      }
      if ((index == 8 || index == 9 || index == 10) && index < JDT4_N) {
        if (node->x > 2) {
          node->x = 2;
          return_value = 1;
        }
      }
    #endif

    /// test start debug
    if (si_display) {
      int j = 0;
      cout << "Ln514. IncreaseNode = " << index << endl;
      for (j = 0; j < JDT4_N; j++) {
        cout << "(" << node_list_tmp[j].x << ", " <<
          node_list_tmp[j].y << ") ";
      }
      cout << endl;
    }
    /// test end debug

    #if((JDT4_N == (4 * JDT4_GRID_SIZE - 2)) || \
        (JDT4_N == (4 * JDT4_GRID_SIZE - 3)))
      if (index == 1) {
        node->x = 0;
        return_value = 1;
      }
    #endif
      if (node->x >= JDT4_GRID_SIZE) {
        node->x = 0;
        // return 1;
        return_value = 1;
      }
    #if(JDT4_N == (4 * JDT4_GRID_SIZE - 1))
      if (index == (JDT4_N - 2)) {
        if (node->x < (JDT4_GRID_SIZE - 1)) {
          node->x = JDT4_GRID_SIZE - 1;
        }
      }
    #elif(JDT4_N == (4 * JDT4_GRID_SIZE))
      if (index == (JDT4_N - 2) || index == (JDT4_N - 3)) {
        if (node->x < (JDT4_GRID_SIZE - 1)) {
          node->x = JDT4_GRID_SIZE - 1;
        }
      }
    #endif
    }
  }
  if (index > 0 && index < (JDT4_N - 1)) {
    if ((node->x < node_list_tmp[index - 1].x) ||
        (node->x == node_list_tmp[index - 1].x &&
            node->y <= node_list_tmp[index - 1].y)) {
      node->x = node_list_tmp[index - 1].x;
      node->y = node_list_tmp[index - 1].y + 1;

      if (node->y >= JDT4_GRID_SIZE) {
        node->y = 0;
        node->x += 1;
        if (node->x >= JDT4_GRID_SIZE) {
          node->x = 0;
          return_value = 1;
        }
      }
    }
  }

  /// test start debug
  if (si_display) {
    int j = 0;
    cout << "Ln602. IncreaseNode = " << index << ", return_value = " <<
      return_value << endl;
    for (j = 0; j < JDT4_N; j++) {
      cout << "(" << node_list_tmp[j].x << ", " <<
        node_list_tmp[j].y << ") ";
    }
    cout << endl;
  }
  /// test end debug
  return return_value;
}

static bool co_linear(const JDTNode& node1, const JDTNode& node2,
  const JDTNode& node3);
bool check_valid(const vector<JDTNode>& node_list) {
  // vector< vector <JDTNode> > line_list;
  vector< int > close_list;
  int j = 0;
  if (node_list.size() != JDT4_N) {
    return false;
  }
  for (int i1 = 0; i1 < JDT4_N; ++i1) {
    JDTNode node1 = node_list[i1];
    int line_count_1 = 0;
    int i = 1;
    int colinear_cnt = 0;
    for (i = 0; i < JDT4_N; ++i) {
      close_list.clear();
      // cout << "i1 = " << i1 << ", i = " << i << endl;
      if (i == i1) {
        continue;
      }
      colinear_cnt = 0;
      const JDTNode& node2 = node_list[i];
      bool in_close_list = false;
      for (int k = 0; k < static_cast<int>(close_list.size()); ++k) {
        if (i == close_list[k]) {
          in_close_list = true;
          break;
        }
      }
      if (in_close_list) {
        // cout << "in_close_list = true" << endl;
        continue;
      }
      // cout << "Ln196. i = " << i << endl;
      for (j = i + 1; j < JDT4_N; ++j) {
        // cout << "j = " << j << endl;
        if (j == i1) {
          continue;
        }
        const JDTNode& node3 = node_list[j];
        if (co_linear(node1, node2, node3)) {
          // if (i1 == 1) {
            //  cout << "(" << i1 << i << j << ") : true" << endl;
          // }
          // if (i1 == 1 && i == 0 && j == 2) {
          //   cout << "(" << i1 << i << j << ") : true" << endl;
          // }
          if (colinear_cnt == 0) {
            close_list.push_back(i);
          }
          colinear_cnt++;
          close_list.push_back(j);
        } else {
          // if (i1 == 1 && i == 4 && j == 7) {
          //   cout << "(" << i1 << i << j << ") : false" << endl;
          // }
          // if (i1 == 1 && i == 0 && j == 2) {
          //   cout << "(" << i1 << i << j << ") : false" << endl;
          // }
        }
      }
      if (j == i1 && j < JDT4_N) {
        continue;
      }
      if (colinear_cnt > (JDT_K - 2)) {
        // cout << "colinear_cnt" << " : " << colinear_cnt << endl;
        // cout << "(" << i1 << ", " << i << ")" << endl;
        // return false;
      } else if (colinear_cnt == (JDT_K - 2)) {
        line_count_1++;
      }
    }
    // cout << "line_count_" << i1 << " : " << line_count_1 << endl;
    if (line_count_1 < JDT4_MIN_INCIDENCE) {
      return false;
    }
  }

  return true;
}

bool co_linear(const JDTNode& node1, const JDTNode& node2,
  const JDTNode& node3) {
  int result_value = (node2.y - node1.y) * (node3.x - node1.x) -
    (node3.y - node1.y) * (node2.x - node1.x);
  if (result_value == 0) {
    return true;
  }
  return false;
}

// true, if no cross line; false, otherwise
bool check_valid_no_cross(const vector<JDTNode>& node_list) {
  vector< JDTLineNode > line_list;
  vector< int > close_list;
  int i = 0;
  int j = 0;
  if (node_list.size() != JDT4_N) {
    return false;
  }
  for (int i1 = 0; i1 < JDT4_N; ++i1) {
    JDTNode node1 = node_list[i1];
    int line_count_1 = 0;
    int colinear_cnt = 0;
    for (i = i1 + 1; i < JDT4_N; ++i) {
      close_list.clear();
      if (i == i1) {
        continue;
      }
      colinear_cnt = 0;
      const JDTNode& node2 = node_list[i];
      bool in_close_list = false;
      for (int k = 0; k < static_cast<int>(close_list.size()); ++k) {
        if (i == close_list[k]) {
          in_close_list = true;
          break;
        }
      }
      if (in_close_list) {
        // cout << "in_close_list = true" << endl;
        continue;
      }
      // cout << "Ln196. i = " << i << endl;
      for (j = i + 1; j < JDT4_N; ++j) {
        // cout << "j = " << j << endl;
        if (j == i1) {
          continue;
        }
        const JDTNode& node3 = node_list[j];
        if (co_linear(node1, node2, node3)) {
          // if (si_display) {
          //   cout << "Ln763. (" << i1 << " " << i << " " << j << ")" << endl;
          // }
          // if (i1 == 1 && i == 0 && j == 2) {
          //   cout << "(" << i1 << i << j << ") : true" << endl;
          // }
          if (colinear_cnt == 0) {
            close_list.push_back(i);
          }
          colinear_cnt++;
          close_list.push_back(j);
          if (close_list.size() == 3) {
            JDTLineNode jdt_line;
            get_jdt_line(node1, node_list[close_list[0]],
              node_list[close_list[1]],
              node_list[close_list[2]], &jdt_line);
            // cout << "line node size: " << jdt_line.points.size() << endl;
            if (si_display) {
              cout << "Ln783. (" << i1 << "," <<
                close_list[0] << "," << close_list[1] << "," <<
                close_list[2] << ")" << endl;
            }
            line_list.push_back(jdt_line);
          }
          // cout << "Ln351. line node size: " <<
          //   line_list[line_list.size() - 1].points.size() << endl;
        } else {
          // if (i1 == 1 && i == 4 && j == 7) {
          //   cout << "(" << i1 << i << j << ") : false" << endl;
          // }
          // if (i1 == 1 && i == 0 && j == 2) {
          //   cout << "(" << i1 << i << j << ") : false" << endl;
          // }
        }
      }
      if (j == i1 && j < JDT4_N) {
        continue;
      }
      if (colinear_cnt != 2 && colinear_cnt != 0) {
        // cout << "colinear_cnt" << " : " << colinear_cnt << endl;
        // cout << "(" << i1 << ", " << i << ")" << endl;
        // return false;
      } else if (colinear_cnt == 2) {
        line_count_1++;
      }
    }
  }

  // Judge whether there is no cross
  int line_count = line_list.size();
  // cout << "line count : " << line_count << endl;
  for (i = 0; i < line_count; ++i) {
    const JDTLineNode& line1 = line_list[i];
    if (line1.points.size() < 4) {
      // cout << "line 1 point size: " << line1.points.size() << endl;
      continue;
    }
    if (si_display) {
        cout << "Ln815. (" << line1.points[0].x << ", " << line1.points[0].y <<
         ")" <<
          ", (" << line1.points[1].x << ", " << line1.points[1].y << ")" <<
          ", (" << line1.points[2].x << ", " << line1.points[2].y << ")" <<
          ", (" << line1.points[3].x << ", " << line1.points[3].y << ")" <<
           endl;
        cout << "(" << line1.point_start.x << ", " <<
          line1.point_start.y << ")" <<
          " - (" << line1.point_end.x << ", " <<
          line1.point_end.y << ")"<< endl;
      }
    for (j = i + 1; j < line_count; ++j) {
      const JDTLineNode& line2 = line_list[j];
      // if (i == 2 && si_display) {
      //   cout << "Ln817. (" << line1.points[0].x << ", " <<
      //    line1.points[0].y <<
      //    ")" <<
      //     ", (" << line1.points[1].x << ", " << line1.points[1].y << ")" <<
      //     ", (" << line1.points[2].x << ", " << line1.points[2].y << ")" <<
      //     ", (" << line1.points[3].x << ", " << line1.points[3].y << ")" <<
      //      endl;
      //   cout << "(" << line1.point_start.x << ", " <<
      //     line1.point_start.y << ")" <<
      //     " - (" << line1.point_end.x << ", " <<
      //     line1.point_end.y << ")"<< endl;
      //   cout << "(" << line2.point_start.x << ", " <<
      //     line2.point_start.y << ")" <<
      //     " - (" << line2.point_end.x << ", " <<
      //     line2.point_end.y << ")"<< endl;
      // }
      if (judge_two_jdt_line_cross(line1, line2)) {
        // cout << "(" << line1.points[0].x << ", " << line1.points[0].y <<
        //  ")" <<
        //   ", (" << line1.points[1].x << ", " << line1.points[1].y << ")" <<
        //   ", (" << line1.points[2].x << ", " << line1.points[2].y << ")" <<
        //    endl;
        // cout << "(" << line2.point_start.x << ", " <<
        //   line2.point_start.y << ")" <<
        //   " - (" << line2.point_end.x << ", " <<
        //   line2.point_end.y << ")"<< endl;
        return false;
      }
    }
  }

  return true;
}

#define SYMMETRY_LR       1  // left - right
#define SYMMETRY_BT       2
#define SYMMETRY_XY       3
#define SYMMETRY_L1       4
#define SYMMETRY_L2       5
#define SYMMETRY_LR_BT    6
#define SYMMETRY_LR_XY    7
#define SYMMETRY_LR_L1    8
#define SYMMETRY_BT_XY    9
#define SYMMETRY_BT_L1    10
#define SYMMETRY_XY_L1    11
#define SYMMETRY_LR_BT_XY 12
#define SYMMETRY_LR_BT_L1 13
#define SYMMETRY_LR_XY_L1 14
#define SYMMETRY_BT_XY_L1 15
#define SYMMETRY_LR_BT_XY_L1    16
// true if no symmetry the same as the past graph.
static bool check_symmetry_no_repeat_single(const vector<JDTNode>& node_list,
    int symmetry_type) {
  bool no_repeat = true;
  vector<JDTNode> node_list_symmetry_lr;  // left - right
  vector<JDTNode> node_list_symmetry_bt;  // bottom - top
  vector<JDTNode> node_list_symmetry_lr_bt;  // left - right & bottom - top
  // vector<JDTNode> node_list_symmetry_xy;  // symmtry is x = y
  int node_size = node_list.size();
  int i = 0;
  int j = 0;
  int grid_size = JDT4_GRID_SIZE;

  for (i = 0; i < node_size; ++i) {
    JDTNode node = node_list[i];
    switch (symmetry_type) {
      case SYMMETRY_LR:  // 1
      {
        node.y = grid_size - 1 - node.y;
        break;
      }
      case SYMMETRY_BT:  // 2
      {
        node.x = grid_size - 1 - node.x;
        break;
      }
      case SYMMETRY_XY:  // 3
      {
        swap(node.x, node.y);
        break;
      }
      case SYMMETRY_L1:  // 4
      {
        node.y -= 1;
        break;
      }
      case SYMMETRY_L2:  // 5
      {
        node.y -= 2;
        break;
      }
      case SYMMETRY_LR_BT:  //
      {
        node.y = grid_size - 1 - node.y;
        node.x = grid_size - 1 - node.x;
        break;
      }
      case SYMMETRY_LR_XY:  //
      {
        node.y = grid_size - 1 - node.y;
        swap(node.x, node.y);
        break;
      }
      case SYMMETRY_LR_L1:  //
      {
        node.y = grid_size - 1 - node.y;
        node.y -= 1;
        break;
      }
      case SYMMETRY_BT_XY:  // 8
      {
        node.x = grid_size - 1 - node.x;
        swap(node.x, node.y);
        break;
      }
      case SYMMETRY_BT_L1:  // 9
      {
        node.x = grid_size - 1 - node.x;
        node.y -= 1;
        break;
      }
      case SYMMETRY_XY_L1:  // 10
      {
        swap(node.x, node.y);
        node.y -= 1;
        break;
      }
      case SYMMETRY_LR_BT_XY:  // 11
      {
        node.y = grid_size - 1 - node.y;
        node.x = grid_size - 1 - node.x;
        swap(node.x, node.y);
        break;
      }
      case SYMMETRY_LR_BT_L1:  // 12
      {
        node.y = grid_size - 1 - node.y;
        node.x = grid_size - 1 - node.x;
        node.y -= 1;
        break;
      }
      case SYMMETRY_LR_XY_L1:  // 13
      {
        node.y = grid_size - 1 - node.y;
        swap(node.x, node.y);
        node.y -= 1;
        break;
      }
      case SYMMETRY_BT_XY_L1:  // 14
      {
        node.x = grid_size - 1 - node.x;
        swap(node.x, node.y);
        node.y -= 1;
        break;
      }
      case SYMMETRY_LR_BT_XY_L1:  // 15
      {
        node.y = grid_size - 1 - node.y;
        node.x = grid_size - 1 - node.x;
        swap(node.x, node.y);
        node.y -= 1;
        break;
      }
      default:
      {
        break;
      }
    }
    node_list_symmetry_lr.push_back(node);
  }
  // sort the nodes
  // int tmp_x = 0;
  // int tmp_y = 0;
  for (i = 0; i < node_size; i++) {
    for (j = i + 1; j < node_size; ++j) {
      if ((node_list_symmetry_lr[i].x > node_list_symmetry_lr[j].x) ||
          (node_list_symmetry_lr[i].x == node_list_symmetry_lr[j].x &&
              node_list_symmetry_lr[i].y > node_list_symmetry_lr[j].y)) {
        swap(node_list_symmetry_lr[i].x, node_list_symmetry_lr[j].x);
        swap(node_list_symmetry_lr[i].y, node_list_symmetry_lr[j].y);
      }
    }
  }

  int jdt_num = jdt_graph_list.size();
  for (i = 0; i < jdt_num; ++i) {
    const JDT& jdt = jdt_graph_list[i];
    if (jdt.jdt.size() != node_size) {
      continue;
    }
    for (j = 0; j < node_size; ++j) {
      if (jdt.jdt[j].x != node_list_symmetry_lr[j].x ||
          jdt.jdt[j].y != node_list_symmetry_lr[j].y) {
        break;
      }
    }
    if (j == node_size) {
      return false;
    }
  }

  return true;
}

// true if no symmetry the same as the past graph.
bool check_symmetry_no_repeat(const vector<JDTNode>& node_list) {
  bool no_repeat = true;
  if (!check_symmetry_no_repeat_single(node_list, SYMMETRY_LR)) {
    return false;
  }
  if (!check_symmetry_no_repeat_single(node_list, SYMMETRY_BT)) {
    return false;
  }
  if (!check_symmetry_no_repeat_single(node_list, SYMMETRY_XY)) {
    return false;
  }
  if (!check_symmetry_no_repeat_single(node_list, SYMMETRY_L1)) {
    return false;
  }
  if (!check_symmetry_no_repeat_single(node_list, SYMMETRY_L2)) {
    return false;
  }
  if (!check_symmetry_no_repeat_single(node_list, SYMMETRY_LR_BT)) {
    return false;
  }
  if (!check_symmetry_no_repeat_single(node_list, SYMMETRY_LR_XY)) {
    return false;
  }
  if (!check_symmetry_no_repeat_single(node_list, SYMMETRY_LR_L1)) {
    return false;
  }
  if (!check_symmetry_no_repeat_single(node_list, SYMMETRY_BT_XY)) {
    return false;
  }
  if (!check_symmetry_no_repeat_single(node_list, SYMMETRY_BT_L1)) {
    return false;
  }
  if (!check_symmetry_no_repeat_single(node_list, SYMMETRY_XY_L1)) {
    return false;
  }
  if (!check_symmetry_no_repeat_single(node_list, SYMMETRY_LR_BT_XY)) {
    return false;
  }
  if (!check_symmetry_no_repeat_single(node_list, SYMMETRY_LR_BT_L1)) {
    return false;
  }
  if (!check_symmetry_no_repeat_single(node_list, SYMMETRY_LR_XY_L1)) {
    return false;
  }
  if (!check_symmetry_no_repeat_single(node_list, SYMMETRY_BT_XY_L1)) {
    return false;
  }
  if (!check_symmetry_no_repeat_single(node_list, SYMMETRY_LR_BT_XY_L1)) {
    return false;
  }

  return true;
}

int get_jdt_feature(const vector<JDTNode>& node_list, JDTFeature *feature) {
  int return_value = 1;
  vector< JDTLineNode > line_list;
  vector< int > close_list;
  int i = 0;
  int j = 0;
  int k = 0;
  if (node_list.size() != JDT4_N) {
    return 0;
  }
  for (int i1 = 0; i1 < JDT4_N; ++i1) {
    JDTNode node1 = node_list[i1];
    int line_count_1 = 0;
    int colinear_cnt = 0;

    for (i = i1 + 1; i < JDT4_N; ++i) {
      close_list.clear();
      colinear_cnt = 0;
      const JDTNode& node2 = node_list[i];
      bool in_close_list = false;
      for (k = 0; k < static_cast<int>(close_list.size()); ++k) {
        if (i == close_list[k]) {
          in_close_list = true;
          break;
        }
      }
      if (in_close_list) {
        // cout << "in_close_list = true" << endl;
        continue;
      }
      // cout << "Ln196. i = " << i << endl;
      for (j = i + 1; j < JDT4_N; ++j) {
        // cout << "j = " << j << endl;
        if (j == i1) {
          continue;
        }
        const JDTNode& node3 = node_list[j];
        if (co_linear(node1, node2, node3)) {
          // cout << "(" << i1 << i << j << ")" << endl;
          // if (i1 == 1 && i == 0 && j == 2) {
          //   cout << "(" << i1 << i << j << ") : true" << endl;
          // }
          if (colinear_cnt == 0) {
            close_list.push_back(i);
          }
          colinear_cnt++;
          close_list.push_back(j);
          if (close_list.size() == 3) {
            JDTLineNode jdt_line;
            get_jdt_line(node1, node_list[close_list[0]],
              node_list[close_list[1]],
              node_list[close_list[2]],
              &jdt_line);
            // cout << "line node size: " << jdt_line.points.size() << endl;
            // cout << "Ln629. " << i1 << "," << close_list[0] << "," <<
            //   close_list[1] << "," << close_list[2] << endl;
            line_list.push_back(jdt_line);
          }
          // cout << "Ln351. line node size: " <<
          //   line_list[line_list.size() - 1].points.size() << endl;
        } else {
          // if (i1 == 1 && i == 4 && j == 7) {
          //   cout << "(" << i1 << i << j << ") : false" << endl;
          // }
          // if (i1 == 1 && i == 0 && j == 2) {
          //   cout << "(" << i1 << i << j << ") : false" << endl;
          // }
        }
      }
      if (j == i1 && j < JDT4_N) {
        continue;
      }
      if (colinear_cnt != 2 && colinear_cnt != 0) {
        // cout << "colinear_cnt" << " : " << colinear_cnt << endl;
        // cout << "(" << i1 << ", " << i << ")" << endl;
        // return false;
      } else if (colinear_cnt == 2) {
        line_count_1++;
      }
    }
  }

  int line_count = line_list.size();
  // cout << "line count : " << line_count << endl;
  const int max_point_node_count = 40;
  const int max_edge_count_per_point_node =
    (max_point_node_count - 1) / (JDT_K - 1);
  int feature1_array[max_point_node_count] = {0};
  int feature1[max_edge_count_per_point_node + 1] = {0};

  // feature line count
  feature->line_count = line_count;
  for (i = 0; i < line_count; ++i) {
    const JDTLineNode& line1 = line_list[i];
    int point_in_line_count = line1.points.size();
    if (point_in_line_count < 1) {
      // cout << "line 1 point size: " << line1.points.size() << endl;
      continue;
    }

    for (j = 0; j < point_in_line_count; ++j) {
      const JDTNode& node_tmp = line1.points[j];
      // cout << "(" << node_tmp.x << "," << node_tmp.y << ") ";
      for (k = 0; k < static_cast<int>(node_list.size()); ++k) {
        if (node_tmp.x == node_list[k].x && node_tmp.y == node_list[k].y) {
          feature1_array[k]++;
          break;
        }
      }
    }
    // cout << endl;
  }

  for (i = 0; i < max_edge_count_per_point_node; ++i) {
    for (j = 0; j < max_point_node_count; ++j) {
      if (feature1_array[j] == i && feature1_array[j] > 0) {
        feature1[i]++;
      }
    }
  }

  int feature1_count = 0;
  for (i = max_edge_count_per_point_node; i >= 0; --i) {
    if (feature1[i] > 0) {
      feature1_count = i;
      break;
    }
  }

  feature->feature1.clear();
  // cout << "feature1 size: " << feature1_count << endl;
  for (i = 0; i <= feature1_count; ++i) {
    feature->feature1.push_back(feature1[i]);
  }

  /// get feature2
  int feature2_array[max_point_node_count] = {0};
  int feature2[max_edge_count_per_point_node * 2 + 1] = {0};
  for (i = 0; i < line_count; ++i) {
    const JDTLineNode& line1 = line_list[i];
    int point_in_line_count = line1.points.size();
    if (point_in_line_count < 1) {
      // cout << "line 1 point size: " << line1.points.size() << endl;
      continue;
    }

    for (j = 0; j < point_in_line_count; ++j) {
      const JDTNode& node_tmp = line1.points[j];
      for (k = 0; k < static_cast<int>(node_list.size()); ++k) {
        if (node_tmp.x == node_list[k].x && node_tmp.y == node_list[k].y) {
          if (node_tmp.x == line1.point_start.x &&
              node_tmp.y == line1.point_start.y) {
            feature2_array[k]++;
          } else if (node_tmp.x == line1.point_end.x &&
              node_tmp.y == line1.point_end.y) {
            feature2_array[k]++;
          } else {
            feature2_array[k] += 2;
          }
          break;
        }
      }
    }
  }

  for (i = 0; i < (max_edge_count_per_point_node * 2 + 1); ++i) {
    for (j = 0; j < max_point_node_count; ++j) {
      if (feature2_array[j] == i && feature2_array[j] > 0) {
        feature2[i]++;
      }
    }
  }

  int feature2_count = 0;
  for (i = (max_edge_count_per_point_node * 2); i >= 0; --i) {
    if (feature2[i] > 0) {
      feature2_count = i;
      break;
    }
  }

  feature->feature2.clear();
  // cout << "feature2 size: " << feature2_count << endl;
  for (i = 0; i <= feature2_count; ++i) {
    feature->feature2.push_back(feature2[i]);
  }

  return return_value;
}

bool jdt3_no_repeat_feature(const vector<JDTFeature>& jdt_list_feature,
    const JDTFeature& feature) {
  int feature_count = jdt_list_feature.size();
  int i = 0;
  int k = 0;
  int k2 = 0;
  bool is_repeat = false;
  for (; i < feature_count; ++i) {
    is_repeat = false;
    const JDTFeature& f1 = jdt_list_feature[i];
    int f1_feature1_size = f1.feature1.size();
    int f2_feature1_size = feature.feature1.size();
    int f1_feature2_size = f1.feature2.size();
    int f2_feature2_size = feature.feature2.size();
    if (f1.line_count != feature.line_count) {
      continue;
    }
    if (f1_feature1_size != f2_feature1_size) {
      continue;
    }
    if (f1_feature2_size != f2_feature2_size) {
      continue;
    }
    for (k = 0; k < f1_feature1_size; ++k) {
      if (f1.feature1[k] != feature.feature1[k]) {
        break;
      }
    }
    for (k2 = 0; k2 < f1_feature2_size; ++k2) {
      if (f1.feature2[k2] != feature.feature2[k2]) {
        break;
      }
    }

    if (k == f1_feature1_size && k2 == f1_feature2_size) {
      is_repeat = true;
      break;
    }
  }

  if (is_repeat) {
    return false;
  }
  return true;
}

// return true, if these two jdt lines cross, false otherwise.
bool judge_two_jdt_line_cross(const JDTLineNode& line1,
    const JDTLineNode& line2) {
  bool return_value = true;
  double line1_sx = line1.point_start.x;
  double line1_sy = line1.point_start.y;
  double line1_ex = line1.point_end.x;
  double line1_ey = line1.point_end.y;
  double line2_sx = line2.point_start.x;
  double line2_sy = line2.point_start.y;
  double line2_ex = line2.point_end.x;
  double line2_ey = line2.point_end.y;
  double c1x = 0;
  double c1y = 0;
  double c2x = 0;
  double c2y = 0;
  double s = 0;
  double t = 0;
  double dist = 0.0;
  // bool is_in_one_line = false;
  // if (si_display) {
  //   cout << "Ln1327. dist = " << dist << ", s = " << s << ", t = " <<
  //     t << endl;
  //   cout << "Ln1329. (" << line1_sx << ", " << line1_sy << ") - " <<
  //     line1_ex << ", " << line1_ey << ")" << endl;
  //   cout << "Ln1331. (" << line2_sx << ", " << line2_sy << ") - " <<
  //     line2_ex << ", " << line2_ey << ")" << endl;
  // }

  dist = ClosestPtSegToSeg_2D(line1_sx, line1_sy,
                  line1_ex, line1_ey,
                  line2_sx, line2_sy, line2_ex, line2_ey,
                  &c1x, &c1y, &c2x, &c2y, &s, &t);
  // if ((line1_ey - line1_sy) * (line2_ex - line2_sx) ==
  //     (line1_ex - line1_sx) * (line2_ey - line2_sy)) {
  //   is_in_one_line = true;
  // }
  if ((dist < 0.01) &&
      ((s > 0.01 && s < 0.99) || (t > 0.01 && t < 0.99))) {
    // 相交
    // cout << "dist = " << dist << ", s = " << s << " "
    //   << t <<endl;
    // dist_min = dist;
    // s_min = s;
    // t_min = t;
    int node_count = line1.points.size();
    int k = 0;
    double dx = 0.0;
    double dy = 0.0;
    double d2tmp = 0.0;
    for (k = 0; k < node_count; ++k) {
      const JDTNode& node_tmp = line1.points[k];
      dx = node_tmp.x - c1x;
      dy = node_tmp.y - c1y;
      d2tmp = dx * dx + dy * dy;
      if (d2tmp < 1e-6) {
        // if (si_display) {
        //   cout << "Ln1353. dist = " << dist << ", s = " << s << ", t = " <<
        //     t << endl;
        // }
        return false;
      }
    }
  } else if ((dist < 0.01) && (s < 0.01 || s > 0.99)) {
    // if (si_display) {
    //   cout << "Ln1361. dist = " << dist << ", s = " << s << ", t = " <<
    //     t << endl;
    // }
    return false;
  }
  if (dist > 0.1) {
    // if (si_display) {
    //   cout << "Ln1368. dist = " << dist << ", s = " << s << ", t = " <<
    //     t << endl;
    //   cout << "Ln1370. (" << line1_sx << ", " << line1_sy << ") - " <<
    //     line1_ex << ", " << line1_ey << ")" << endl;
    //   cout << "Ln1372. (" << line2_sx << ", " << line2_sy << ") - " <<
    //     line2_ex << ", " << line2_ey << ")" << endl;
    // }
    return false;
  }
  // if (si_display) {
  //   cout << "dist = " << dist << ", s = " << s << ", t = " << t << endl;
  // }


  return return_value;
}

int get_jdt_line(const JDTNode& node1, const JDTNode& node2,
  const JDTNode& node3, const JDTNode& node4, JDTLineNode *jdt_line) {
  int return_value = 0;
  double x_min = node1.x;
  double x_max = node1.x;
  int index_start = 1;
  int index_end = 1;
  JDTNode *node_start = &jdt_line->point_start;
  JDTNode *node_end = &jdt_line->point_end;
  jdt_line->points.push_back(node1);
  jdt_line->points.push_back(node2);
  jdt_line->points.push_back(node3);
  jdt_line->points.push_back(node4);
  if (node2.x < x_min) {
    x_min = node2.x;
    index_start = 2;
  }
  if (node3.x < x_min) {
    x_min = node3.x;
    index_start = 3;
  }
  if (node4.x < x_min) {
    x_min = node4.x;
    index_start = 4;
  }
  if (node2.x > x_max) {
    x_max = node2.x;
    index_end = 2;
  }
  if (node3.x > x_max) {
    x_max = node3.x;
    index_end = 3;
  }
  if (node4.x > x_max) {
    x_max = node4.x;
    index_end = 4;
  }
  if (x_min < x_max) {
    switch (index_start) {
      case 1:
      {
        node_start->x = node1.x;
        node_start->y = node1.y;
      }
      break;
      case 2:
      {
        node_start->x = node2.x;
        node_start->y = node2.y;
      }
      break;
      case 3:
      {
        node_start->x = node3.x;
        node_start->y = node3.y;
      }
      break;
      case 4:
      {
        node_start->x = node4.x;
        node_start->y = node4.y;
      }
      break;
      default:
      break;
    }
    switch (index_end) {
      case 1:
      {
        node_end->x = node1.x;
        node_end->y = node1.y;
      }
      break;
      case 2:
      {
        node_end->x = node2.x;
        node_end->y = node2.y;
      }
      break;
      case 3:
      {
        node_end->x = node3.x;
        node_end->y = node3.y;
      }
      break;
      case 4:
      {
        node_end->x = node4.x;
        node_end->y = node4.y;
      }
      break;
      default:
      {
        return_value = -1;
      }
      break;
    }
  } else {
    // x_min == x_max
    double y_min = node1.y;
    double y_max = node1.y;
    index_start = 1;
    index_end = 1;
    if (node2.y < y_min) {
      y_min = node2.y;
      index_start = 2;
    }
    if (node3.y < y_min) {
      y_min = node3.y;
      index_start = 3;
    }
    if (node4.y < y_min) {
      y_min = node4.y;
      index_start = 4;
    }
    if (node2.y > y_max) {
      y_max = node2.y;
      index_end = 2;
    }
    if (node3.y > y_max) {
      y_max = node3.y;
      index_end = 3;
    }
    if (node4.y > y_max) {
      y_max = node4.y;
      index_end = 4;
    }
    if (y_min < y_max) {
      switch (index_start) {
        case 1:
        {
          node_start->x = node1.x;
          node_start->y = node1.y;
        }
        break;
        case 2:
        {
          node_start->x = node2.x;
          node_start->y = node2.y;
        }
        break;
        case 3:
        {
          node_start->x = node3.x;
          node_start->y = node3.y;
        }
        case 4:
        {
          node_start->x = node4.x;
          node_start->y = node4.y;
        }
        break;
        default:
        break;
      }
      switch (index_end) {
        case 1:
        {
          node_end->x = node1.x;
          node_end->y = node1.y;
        }
        break;
        case 2:
        {
          node_end->x = node2.x;
          node_end->y = node2.y;
        }
        break;
        case 3:
        {
          node_end->x = node3.x;
          node_end->y = node3.y;
        }
        break;
        case 4:
        {
          node_end->x = node4.x;
          node_end->y = node4.y;
        }
        break;
        default:
          return_value = -1;
        break;
      }
    }
  }

  // cout << "index_start = " << index_start << ", " <<
  //   "index_end = " << index_end << endl;
  // cout << "Ln1072. (" << node_start->x << "," << node_start->y << ") - " <<
  //   "(" << node_end->x << "," << node_end->y << ")" << endl;
  // cout << "  (" << node1.x << "," << node1.y << ")  ";
  // cout << "  (" << node2.x << "," << node2.y << ")  ";
  // cout << "  (" << node3.x << "," << node3.y << ")  ";
  // cout << "  (" << node4.x << "," << node4.y << ")  " << endl;


  return return_value;
}

void print_feature(const JDTFeature& f, char* buff, size_t size_buff) {
  char str_tmp[512] = {0};
  cout << f.line_count << "_";
  // snprintf(buff, size_buff, "");
  // memset(buff, 0, size_buff);
  snprintf(buff, size_buff, "%d_", f.line_count);
  if (f.feature1.size() > 0) {
    cout << "(";
    snprintf(str_tmp, sizeof(str_tmp), "%s", buff);
    snprintf(buff, size_buff, "%s(", str_tmp);
    for (int i = 2; i < static_cast< int >(f.feature1.size()); ++i) {
      cout << f.feature1[i];
      snprintf(str_tmp, sizeof(str_tmp), "%s", buff);
      snprintf(buff, size_buff, "%s%d", str_tmp, f.feature1[i]);
      if (i < (static_cast< int >(f.feature1.size()) - 1)) {
        cout << ",";
        snprintf(str_tmp, sizeof(str_tmp), "%s", buff);
        snprintf(buff, size_buff, "%s,", str_tmp);
      }
    }
    cout << ")";
    snprintf(str_tmp, sizeof(str_tmp), "%s", buff);
    snprintf(buff, size_buff, "%s)", str_tmp);
  }

  cout << "_";
  snprintf(str_tmp, sizeof(str_tmp), "%s", buff);
  snprintf(buff, size_buff, "%s_", str_tmp);
  if (f.feature2.size() > 0) {
    cout << "(";
    snprintf(str_tmp, sizeof(str_tmp), "%s", buff);
    snprintf(buff, size_buff, "%s(", str_tmp);
    for (int i = 2; i < static_cast< int >(f.feature2.size()); ++i) {
      cout << f.feature2[i];
      snprintf(str_tmp, sizeof(str_tmp), "%s", buff);
      snprintf(buff, size_buff, "%s%d", str_tmp, f.feature2[i]);
      if (i < (static_cast< int >(f.feature2.size()) - 1)) {
        cout << ",";
        snprintf(str_tmp, sizeof(str_tmp), "%s", buff);
        snprintf(buff, size_buff, "%s,", str_tmp);
      }
    }
    cout << ")";
    snprintf(str_tmp, sizeof(str_tmp), "%s", buff);
    snprintf(buff, size_buff, "%s)", str_tmp);
  }
}

// 从文件中读取jdt4
int main_jdt4_read_from_file() {
  char filename_wmt4[256] = "jdt4_13.log";
  // 从文件中读取输出规则
  // FILE *file = fopen("filename_rule", "rt");
  std::fstream file(filename_wmt4);
  std::string wmt4_line;
  int line_count = 0;

  while (getline(file, wmt4_line)) {
    // words.push_back(line);
    size_t posTag = wmt4_line.find(":");
    if (posTag == -1) {
      continue;
    }
    cout << "posTag = " << posTag << endl;
    cout << wmt4_line << endl;
    // 0 : 993553, (0, 0) (0, 1) (0, 3) (0, 4) (1, 1) (1, 2) (1, 3) (1, 4) (2, 3) (3, 3) (3, 4) (4, 4) 6_(12)_(3,6,3)
    // std::stringstream stream(wmt4_line);
    char ch;
    std::string str_tmp;
    int index = -1;
    // stream >> index;
    posTag = wmt4_line.find("_");
    cout << "pos of _ is " << posTag << endl;
    wmt4_line = wmt4_line.substr(0, posTag);
    posTag = wmt4_line.find(":");
    str_tmp = wmt4_line.substr(0, posTag);
    wmt4_line = wmt4_line.substr(posTag + 1, wmt4_line.length());
    cout << "str_tmp is " << str_tmp << endl;
    cout << "wmt4_line is " << wmt4_line << endl;
    index = atoi(str_tmp.c_str());
    cout << "index = " << index << endl;
    long long id = -1;
    posTag = wmt4_line.find(",");
    str_tmp = wmt4_line.substr(0, posTag);
    char *endptr = NULL;
    // errno = 0;
    // val = strtoll(nptr, &endptr, 10);
    id = strtoll(str_tmp.c_str(), &endptr, 10);
    cout << "id = " << id << endl;
    // stream >> ch;
    // stream >> id;
    // stream >> ch;
    int point_count = 0;
    int i = 0;
    int max_count = 100;
    posTag = 0;
    while (i < max_count) {
      posTag = wmt4_line.find("(", posTag + 1);
      if (posTag == -1) {
        break;
      }
      i++;
    }
    point_count = i;
    cout << "Point count is " << point_count << endl;
    if (point_count < 1) {
      continue;
    }
    // 逐个取每个点的坐标
    int x = 0;
    int y = 0;
    std::string xy_string;
    JDTNode node;
    node_list_tmp.clear();
    for (i = 0; i < point_count; i++) {
      posTag = wmt4_line.find("(");
      wmt4_line = wmt4_line.substr(posTag + 1, wmt4_line.length());
      posTag = wmt4_line.find(")");
      xy_string = wmt4_line.substr(0, posTag);
      // cout << "xy_string is " << xy_string << endl;
      std::stringstream stream(xy_string);
      stream >> x;
      stream >> ch;
      stream >> y;
      cout << "xy coordinate is (" << x << "," << y << ")" << endl;
      node.x = x;
      node.y = y;
      node_list_tmp.push_back(node);
    }

    JDTFeature jdt_feature;
    vector< JDTLineNode > line_list;
    int feature_get_result = get_jdt_lines(node_list_tmp, &jdt_feature,
      &line_list);
    cout << "Ln1824. line count : " << line_list.size() << endl;
    JDT jdt_node;
    jdt_node.lines = line_list;
    jdt_graph_list.push_back(jdt_node);
    cout << "Ln1828. jdt count : " << jdt_graph_list.size() << endl;

    int item_v = -1;
    int item_d = -1;
    int item_expect_a = -1;
    int line_length = wmt4_line.length();
    double expect_v_a = -1;

    line_count++;
    g_valid_graph_count++;
    if (line_count > 10) {
      break;
    }
  }

  cout << "jdt count : " << jdt_graph_list.size() << endl;
  cout << "jdt line count : " << endl;
  for (int i = 0; i < jdt_graph_list.size(); ++i) {
    cout << "  " << jdt_graph_list[i].lines.size() << endl;
  }
}

// 获取简单图的线列表
int get_jdt_lines(const vector<JDTNode>& node_list, JDTFeature *feature,
    vector< JDTLineNode >* line_list) {
  int return_value = 1;
  // vector< JDTLineNode > line_list;
  vector< int > close_list;
  int i = 0;
  int j = 0;
  int k = 0;
  if (node_list.size() != JDT4_N) {
    return 0;
  }
  for (int i1 = 0; i1 < JDT4_N; ++i1) {
    JDTNode node1 = node_list[i1];
    int line_count_1 = 0;
    int colinear_cnt = 0;

    for (i = i1 + 1; i < JDT4_N; ++i) {
      close_list.clear();
      colinear_cnt = 0;
      const JDTNode& node2 = node_list[i];
      bool in_close_list = false;
      for (k = 0; k < static_cast<int>(close_list.size()); ++k) {
        if (i == close_list[k]) {
          in_close_list = true;
          break;
        }
      }
      if (in_close_list) {
        // cout << "in_close_list = true" << endl;
        continue;
      }
      // cout << "Ln196. i = " << i << endl;
      for (j = i + 1; j < JDT4_N; ++j) {
        // cout << "j = " << j << endl;
        if (j == i1) {
          continue;
        }
        const JDTNode& node3 = node_list[j];
        if (co_linear(node1, node2, node3)) {
          // cout << "(" << i1 << i << j << ")" << endl;
          // if (i1 == 1 && i == 0 && j == 2) {
          //   cout << "(" << i1 << i << j << ") : true" << endl;
          // }
          if (colinear_cnt == 0) {
            close_list.push_back(i);
          }
          colinear_cnt++;
          close_list.push_back(j);
          if (close_list.size() == 3) {
            JDTLineNode jdt_line;
            get_jdt_line(node1, node_list[close_list[0]],
              node_list[close_list[1]],
              node_list[close_list[2]],
              &jdt_line);
            // cout << "line node size: " << jdt_line.points.size() << endl;
            // cout << "Ln629. " << i1 << "," << close_list[0] << "," <<
            //   close_list[1] << "," << close_list[2] << endl;
            line_list->push_back(jdt_line);
          }
          // cout << "Ln351. line node size: " <<
          //   line_list[line_list.size() - 1].points.size() << endl;
        } else {
          // if (i1 == 1 && i == 4 && j == 7) {
          //   cout << "(" << i1 << i << j << ") : false" << endl;
          // }
          // if (i1 == 1 && i == 0 && j == 2) {
          //   cout << "(" << i1 << i << j << ") : false" << endl;
          // }
        }
      }
      if (j == i1 && j < JDT4_N) {
        continue;
      }
      if (colinear_cnt != 2 && colinear_cnt != 0) {
        // cout << "colinear_cnt" << " : " << colinear_cnt << endl;
        // cout << "(" << i1 << ", " << i << ")" << endl;
        // return false;
      } else if (colinear_cnt == 2) {
        line_count_1++;
      }
    }
  }

  int line_count = line_list->size();
  // cout << "line count : " << line_count << endl;
  const int max_point_node_count = 40;
  const int max_edge_count_per_point_node =
    (max_point_node_count - 1) / (JDT_K - 1);
  int feature1_array[max_point_node_count] = {0};
  int feature1[max_edge_count_per_point_node + 1] = {0};

  // feature line count
  feature->line_count = line_count;
  for (i = 0; i < line_count; ++i) {
    const JDTLineNode& line1 = (*line_list)[i];
    int point_in_line_count = line1.points.size();
    if (point_in_line_count < 1) {
      // cout << "line 1 point size: " << line1.points.size() << endl;
      continue;
    }

    for (j = 0; j < point_in_line_count; ++j) {
      const JDTNode& node_tmp = line1.points[j];
      // cout << "(" << node_tmp.x << "," << node_tmp.y << ") ";
      for (k = 0; k < static_cast<int>(node_list.size()); ++k) {
        if (node_tmp.x == node_list[k].x && node_tmp.y == node_list[k].y) {
          feature1_array[k]++;
          break;
        }
      }
    }
    // cout << endl;
  }

  for (i = 0; i < max_edge_count_per_point_node; ++i) {
    for (j = 0; j < max_point_node_count; ++j) {
      if (feature1_array[j] == i && feature1_array[j] > 0) {
        feature1[i]++;
      }
    }
  }

  int feature1_count = 0;
  for (i = max_edge_count_per_point_node; i >= 0; --i) {
    if (feature1[i] > 0) {
      feature1_count = i;
      break;
    }
  }

  feature->feature1.clear();
  // cout << "feature1 size: " << feature1_count << endl;
  for (i = 0; i <= feature1_count; ++i) {
    feature->feature1.push_back(feature1[i]);
  }

  /// get feature2
  int feature2_array[max_point_node_count] = {0};
  int feature2[max_edge_count_per_point_node * 2 + 1] = {0};
  for (i = 0; i < line_count; ++i) {
    const JDTLineNode& line1 = (*line_list)[i];
    int point_in_line_count = line1.points.size();
    if (point_in_line_count < 1) {
      // cout << "line 1 point size: " << line1.points.size() << endl;
      continue;
    }

    for (j = 0; j < point_in_line_count; ++j) {
      const JDTNode& node_tmp = line1.points[j];
      for (k = 0; k < static_cast<int>(node_list.size()); ++k) {
        if (node_tmp.x == node_list[k].x && node_tmp.y == node_list[k].y) {
          if (node_tmp.x == line1.point_start.x &&
              node_tmp.y == line1.point_start.y) {
            feature2_array[k]++;
          } else if (node_tmp.x == line1.point_end.x &&
              node_tmp.y == line1.point_end.y) {
            feature2_array[k]++;
          } else {
            feature2_array[k] += 2;
          }
          break;
        }
      }
    }
  }

  for (i = 0; i < (max_edge_count_per_point_node * 2 + 1); ++i) {
    for (j = 0; j < max_point_node_count; ++j) {
      if (feature2_array[j] == i && feature2_array[j] > 0) {
        feature2[i]++;
      }
    }
  }

  int feature2_count = 0;
  for (i = (max_edge_count_per_point_node * 2); i >= 0; --i) {
    if (feature2[i] > 0) {
      feature2_count = i;
      break;
    }
  }

  feature->feature2.clear();
  // cout << "feature2 size: " << feature2_count << endl;
  for (i = 0; i <= feature2_count; ++i) {
    feature->feature2.push_back(feature2[i]);
  }

  return return_value;
}