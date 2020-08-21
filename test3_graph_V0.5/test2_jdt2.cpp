#include "test2_jdt2.h"

#include <iostream>

#include "is_in_line.h"
#include "util.h"

using namespace std;
using namespace phoenix::common;

int count_one_bits(long long n);
// bool is_in_line(int start_tag1, int end_tag1, int start_tag2, int end_tag2);
int connect_vertex_count(long long n, int vertex_index);

extern double coordx[200];
extern double coordy[200];
//extern int count;

int g_valid_graph_count = 0;
long long g_valid_graph_tag[MAX_VALID_GRAPH_COUNT] = {0};
int g_lines_count = 8 * (8 - 1) / 2;
int g_lines[200][2] = {0};
int g_n = 10;

void update(int index) {
  cout << "Enter into update " << index << endl;
  if (index < MAX_VALID_GRAPH_COUNT && index < g_valid_graph_count) {
    long long tag = g_valid_graph_tag[index];
    int line_count = count_one_bits(tag);

    cout << "tag is " << tag << endl;
    cout << "line count is " << line_count << endl;
    cout << "line list: " << endl;

    long long tag_tmp = tag;
    int start_point = 0; // g_lines[tag][0];
    int end_point   = 0; // g_lines[tag][1];
    int line_tag_tmp = 0;
    double offset_x = 10.0;
    double offset_y = 10.0;
    double factor_x = 100.0;
    double factor_y = 100.0;

    g_count = 0;
    for (int i = 0; i < g_lines_count; ++i) {
      if ((tag_tmp & 1) > 0) {
        start_point = g_lines[i][0];
        end_point   = g_lines[i][1];
        cout << "  line " << line_tag_tmp++ << " : " << start_point << " -> " << end_point << "  "; 

        if (start_point == 0) {
          coordx[g_count] = offset_x + 0 * factor_x;
          coordy[g_count] = offset_y + 0 * factor_y;
        } else if (start_point == 1) {
          coordx[g_count] = offset_x + 1 * factor_x;
          coordy[g_count] = offset_y + 0 * factor_y;
        } else if (start_point == 2) {
          coordx[g_count] = offset_x + 0 * factor_x;
          coordy[g_count] = offset_y + 1 * factor_y;
        } else if (start_point == 3) {
          coordx[g_count] = offset_x + 1 * factor_x;
          coordy[g_count] = offset_y + 1 * factor_y;
        } else if (start_point == 4) {
          coordx[g_count] = offset_x + 2 * factor_x;
          coordy[g_count] = offset_y + 0 * factor_y;
        } else if (start_point == 5) {
          coordx[g_count] = offset_x + 2 * factor_x;
          coordy[g_count] = offset_y + 1 * factor_y;
        } else if (start_point == 6) {
          coordx[g_count] = offset_x + 0 * factor_x;
          coordy[g_count] = offset_y + 2 * factor_y;
        } else if (start_point == 7) {
          coordx[g_count] = offset_x + 1 * factor_x;
          coordy[g_count] = offset_y + 2 * factor_y;
        } else if (start_point == 8) {
          coordx[g_count] = offset_x + 2 * factor_x;
          coordy[g_count] = offset_y + 2 * factor_y;
        } 

        if (end_point == 0) {
          coordx[g_count + 1] = offset_x + 0 * factor_x;
          coordy[g_count + 1] = offset_y + 0 * factor_y;
        } else if (end_point == 1) {
          coordx[g_count + 1] = offset_x + 1 * factor_x;
          coordy[g_count + 1] = offset_y + 0 * factor_y;
        } else if (end_point == 2) {
          coordx[g_count + 1] = offset_x + 0 * factor_x;
          coordy[g_count + 1] = offset_y + 1 * factor_y;
        } else if (end_point == 3) {
          coordx[g_count + 1] = offset_x + 1 * factor_x;
          coordy[g_count + 1] = offset_y + 1 * factor_y;
        } else if (end_point == 4) {
          coordx[g_count + 1] = offset_x + 2 * factor_x;
          coordy[g_count + 1] = offset_y + 0 * factor_y;
        } else if (end_point == 5) {
          coordx[g_count + 1] = offset_x + 2 * factor_x;
          coordy[g_count + 1] = offset_y + 1 * factor_y;
        } else if (end_point == 6) {
          coordx[g_count + 1] = offset_x + 0 * factor_x;
          coordy[g_count + 1] = offset_y + 2 * factor_y;
        } else if (end_point == 7) {
          coordx[g_count + 1] = offset_x + 1 * factor_x;
          coordy[g_count + 1] = offset_y + 2 * factor_y;
        } else if (end_point == 8) {
          coordx[g_count + 1] = offset_x + 2 * factor_x;
          coordy[g_count + 1] = offset_y + 2 * factor_y;
        } else if (end_point == 9) {
          coordx[g_count + 1] = offset_x + 3 * factor_x;
          coordy[g_count + 1] = offset_y + 0 * factor_y;
        } 
        cout << "(" << coordx[g_count]     << ", " << coordy[g_count]     << ") -> " <<
                "(" << coordx[g_count + 1] << ", " << coordy[g_count + 1] << ")" << endl;
        g_count += 2;
      }
      tag_tmp = tag_tmp >> 1;
    }

  }
}

int main_jdt2() {


  cout << "Hello, world!" << endl;

  long long i = 0;
  const int n = g_n; // 5 / 6 / 7 / 8
  const int lines_count = n * (n - 1) / 2;
  int lines[lines_count][2] = {0};
  int j = 0;
  int index = 0;
  

  for (i = 0; i < n; i++) {
    for (j = (i + 1); j < n; j++) {
        // 第index线对应的端点
        lines[index][0] = i;
        lines[index][1] = j;
        g_lines[index][0] = i;
        g_lines[index][1] = j;
        index++;
    }
  }
  g_lines_count = lines_count;

  for (i = 0; i < lines_count; i++) {
    cout << i << " : (" << lines[i][0] << ", " << lines[i][1] << ")" << endl;
  }

  if (lines_count > 30) {
    //cout << "lines too many!" << endl;
    //return -1;
  }

  if (g_option == 2) {
    read_graph_from_file();
    return 1;
  } else if (g_option == 3) {
    read_graph_from_file();
    for (j = 0; j < g_valid_graph_count; j++) {
      update2(j);
    }
    cout << "g_option 3 done." << endl;
    return 2;
  }

  int graph = 0;
  long long graph_count = 100LL;
  int valid_graph_count = 0;
  long long valid_graph_tag[MAX_VALID_GRAPH_COUNT] = {0LL};

  int max_bit = 40;
  if (lines_count > max_bit) {
    graph_count = (1LL << max_bit);
  } else {
    graph_count = (1LL << lines_count);
  }
  cout << "graph_count : " << graph_count << endl;
  //graph_count = 20;
//  long long start_tag = 0;
//  long long end_tag   = graph_count;
//  for (i = 0; i < graph_count; i++) {
  long long start_tag_search = 1LL << 39;//;1LL << 38;
  long long end_tag_search   = graph_count;
  int cross_j = 0;
  int cross_k = 0;
  //end_tag_search = start_tag_search + 10;
  for (i = start_tag_search; i < end_tag_search; i++) {
    if (i % (1LL << (max_bit - 6)) == 0) {
      cout << "progress : " << ((double)i * 100.0 / graph_count) << "%, " << i << endl;
    }
    // 获取线条的个数，要求至少有3条线
    int line_count_tmp = count_one_bits(i);
    if (line_count_tmp < 4) {
      continue;
    }
    // 获取点的个数，要求每个点至少有2条线经过
    int vertex_lines[n] = {0};
    long long tag_tmp = i;
    int j = 0;
    int k = 0;
    for (j = 0; j < lines_count; ++j) {
      if ((tag_tmp & 1) > 0) {
        int start_tag = lines[j][0];
        int end_tag   = lines[j][1];
        vertex_lines[start_tag]++;
        vertex_lines[end_tag]++;
      }
      tag_tmp = tag_tmp >> 1;
    }
    int vertax_count_min = VERTAX_COUNT_MIN;
    int vertax_count = 0;
    for (j = 0; j < n; j++) {
      if (vertex_lines[j] >= 2) {
        vertax_count++;
      }
    }
    if (vertax_count < vertax_count_min) {
      continue;
    }
    bool vertex_valid = true;
    for (j = 0; j < n; j++) {
      if (vertex_lines[j] == 1) {
        vertex_valid = false;
        break;
      }
    }
    if (!vertex_valid) {
      continue;
    }

    // 要求任意两条相邻的线不能共线
    long long tag_tmp1 = i;
    // 两条线是否相交或共线
    bool exist_in_line = false;
    for (j = 0; j < lines_count; ++j) {
      int start_tag1 = 0;
      int end_tag1 = 0;
      int start_tag2 = 0;
      int end_tag2 = 0;
      if ((tag_tmp1 & 1) > 0) {
        start_tag1 = lines[j][0];
        end_tag1   = lines[j][1];
        cross_j = j;

        long long tag_tmp2 = (tag_tmp1 >> 1);
        for (k = (j + 1); k < lines_count; ++k) {
          if ((tag_tmp2 & 1) > 0) {
            start_tag2 = lines[k][0];
            end_tag2   = lines[k][1];
            cross_k = k;
            if (cross_j != cross_k) {
              if (is_in_line(start_tag1, end_tag1, start_tag2, end_tag2)) {
                exist_in_line = true;
                break;
              } else {
                // update_coord_by_index0();
                double line1_sx = 0;
                double line1_sy = 0;
                double line1_ex = 0;
                double line1_ey = 0;
                double line2_sx = 0;
                double line2_sy = 0;
                double line2_ex = 0;
                double line2_ey = 0;
                double c1x = 0;
                double c1y = 0;
                double c2x = 0;
                double c2y = 0;
                double s = 0;
                double t = 0;
                bool is_in_one_line = false;
                update_coord_by_index0(start_tag1, &line1_sx, &line1_sy);
                update_coord_by_index0(end_tag1,   &line1_ex, &line1_ey);
                update_coord_by_index0(start_tag2, &line2_sx, &line2_sy);
                update_coord_by_index0(end_tag2,   &line1_ex, &line2_ey);
                double dist = ClosestPtSegToSeg_2D(line1_sx, line1_sy,
                  line1_ex, line1_ey,
                  line2_sx, line2_sy, line2_ex, line2_ey,
                  &c1x, &c1y, &c2x, &c2y, &s, &t);
                if ((line1_ey - line1_sy) * (line2_ex - line2_sx) ==
                    (line1_ex - line1_sx) * (line2_ey - line2_sy)) {
                  is_in_one_line = true;
                }
                if ((dist < 0.01) &&
                    ((s > 0.01 && s < 0.99) || (t > 0.01 && t < 0.99))) {
                  // 相交
                  // cout << "dist = " << dist << ", s = " << s << " "
                  //   << t <<endl;
                  // dist_min = dist;
                  // s_min = s;
                  // t_min = t;
                  exist_in_line = true;
                  break;
                } else if (dist < 0.01 && is_in_one_line) {
                  //  共线
                  // dist_min = 0;
                  // s_min = 0.5;
                  // t_min = 0.5;
                  exist_in_line = true;
                  break;
                }
              }
            }
          }
          tag_tmp2 = tag_tmp2 >> 1;
          if (tag_tmp2 == 0) {
            break;
          }
        }
      }
      tag_tmp1 = tag_tmp1 >> 1;
      if (tag_tmp1 == 0) {
        break;
      }
      if (exist_in_line) {
        break;
      }
    }
    if (exist_in_line) {
      if (cross_j >= 0 && cross_j <= 63) {
        i = i + (1LL << cross_j) - 1;
      }
      continue;
    }

    int i_connect_vertex = connect_vertex_count(i, -1);
    // need the graph is continue
    if (vertax_count != i_connect_vertex) {
      continue;
    }
    cout << valid_graph_count << ", " << i;
    cout << " : " << line_count_tmp << " ";
    if (line_count_tmp < 3) {
      cout << "Not valid.";
    }
    cout << ", " << i_connect_vertex << endl;
    for (j = 0; j < n; j++) {
      cout << vertex_lines[j] << "  ";
    }
    cout << endl;

    {

      // log
      phoenix::common::LogFile log_file_status_(".");
      char str_buff_status_[1024*2];
      log_file_status_.Open();
      log_file_status_.Enable(true);
      std::snprintf(str_buff_status_, sizeof(str_buff_status_)-1,
                  "%lld\n", i);
      log_file_status_.Write(str_buff_status_);
      // log_file_status_.Close();
      // log_file_status_.Open();
    }


    if (valid_graph_count < MAX_VALID_GRAPH_COUNT) {
      valid_graph_tag[valid_graph_count] = i;
      g_valid_graph_tag[valid_graph_count] = i;
    }
    valid_graph_count++;

    if (valid_graph_count > 1200) {
      break;
    }
  }
  g_valid_graph_count = valid_graph_count;
  cout << "Valid graph count: " << valid_graph_count << endl;
  cout << "This is the end of the program." << endl;

  return 0;
}


//模2除2法，该方法不能判断负数，因为负数模2的结果总是0 
int count_one_bits(long long n) { 
  int count = 0; 
  while (n != 0) { 
    //模2可以得到最低位的数，如果模2的结果是1，则将count++ 
    // if (n % 2 == 1) { 
    //   count++; 
    // }  
    count += (n & 0x01);
    // n = n / 2;//除2可以去掉最末位的数
    n = n >> 1;//除2可以去掉最末位的数  
  } 
  return count; 
}

// 计算连通的顶点个数
int connect_vertex_count(long long n, int vertex_index) {
  int count = 0;
  long long tag_tmp = n;
  int fill_count = 0;
  int fill_vertex[16] = {-1};
  int candidate_count = 1;
  int candidate_vertex[16] = {-1};
  int prepare_count = 0;
  int prepare_vertex[16] = {-1};
  int i = 0;
  int start_point = 0;
  int end_point = 0;
  count = 0;
  candidate_vertex[0] = vertex_index;

  if (vertex_index == -1) {
    // search the first vertex
    tag_tmp = n;
    for (i = 0; i < g_lines_count; ++i) {
      if ((tag_tmp & 1) > 0) {
        start_point = g_lines[i][0];
        end_point   = g_lines[i][1];
        vertex_index = start_point;
        break;
        
        //cout << "  line " << line_tag_tmp++ << " : " << start_point << " -> " << end_point << "  "; 


        //cout << "(" << coordx[count]     << ", " << coordy[count]     << ") -> " <<
        //        "(" << coordx[count + 1] << ", " << coordy[count + 1] << ")" << endl;
        //count += 2;
      }
      tag_tmp = tag_tmp >> 1;
    }

    candidate_vertex[0] = vertex_index;
  }

  //cout << "start_point = " << start_point << endl;
  int line_tag_tmp2 = 0;
  while (candidate_count > 0) {
    tag_tmp = n;
    for (i = 0; i < g_lines_count; ++i) {
      if ((tag_tmp & 1) > 0) {
        start_point = g_lines[i][0];
        end_point   = g_lines[i][1];
        //cout << "start_point = " << start_point << ", candidate_vertex[0]=" << candidate_vertex[0] << endl;
        if (start_point == candidate_vertex[0]) {
          bool found_prepare = false;
          for (int j = 0; j < prepare_count; j++) {
            if (prepare_vertex[j] == end_point) {
              found_prepare = true;
              break;
            }
          }
          bool found_fill = false;
          for (int j = 0; j < fill_count; j++) {
            if (fill_vertex[j] == end_point) {
              found_fill = true;
              break;
            }
          }
          //cout << "found_prepare = " << found_prepare << endl;
          // not found, add it to prepare list
          if ((!found_prepare) && (!found_fill) && (prepare_count < 16)) {
            //cout << "not found, add end_point to prepare list" << endl;
            prepare_vertex[prepare_count] = end_point;
            prepare_count++;
          }
        } else if (end_point == candidate_vertex[0]) {
          bool found_prepare = false;
          for (int j = 0; j < prepare_count; j++) {
            if (prepare_vertex[j] == start_point) {
              found_prepare = true;
              break;
            }
          }
          bool found_fill = false;
          for (int j = 0; j < fill_count; j++) {
            if (fill_vertex[j] == start_point) {
              found_fill = true;
              break;
            }
          }
          // not found, add it to prepare list
          if ((!found_prepare) && (!found_fill) && (prepare_count < 16)) {
            prepare_vertex[prepare_count] = start_point;
            prepare_count++;
          }
        }
        //cout << "  line " << line_tag_tmp2++ << " : " << start_point << " -> " << end_point << "  " << endl; 


        //cout << "(" << coordx[count]     << ", " << coordy[count]     << ") -> " <<
        //        "(" << coordx[count + 1] << ", " << coordy[count + 1] << ")" << endl;
        //count += 2;
      }
      tag_tmp = tag_tmp >> 1;
    }

    candidate_count = 0;
    if (fill_count < 16) {
      fill_vertex[fill_count] = candidate_vertex[0];
      fill_count++;
    } else {
      break;
    }
    //cout << "prepare_count = " << prepare_count << endl;
    if (prepare_count > 0) {
      candidate_count = 1;
      candidate_vertex[0] = prepare_vertex[prepare_count - 1];
      prepare_count--;
    }
  }

  return fill_count;
}

int read_graph_from_file() {
  // int i = 0;
  int read_count = 0;
  // long long i_file = 0LL;
  long long i_file = 0LL;
  // FILE *file = fopen("jdt2_file.log", "rt+");
  // valid4.csv.log
  FILE *file = fopen("valid5.csv", "rt+");
  if (nullptr == file) {
    // AERROR << "Open file \"" << file_name_ << "\" failed.";
    return (-1);
  }

  int valid_graph_count = 0;
  while(!feof(file)) {
    if (fscanf(file, "%lld", &i_file)) {
      //cout << "read from file, " << read_count << " : " << i_file << endl;
      if (valid_graph_count < MAX_VALID_GRAPH_COUNT) {
        //valid_graph_tag[valid_graph_count] = i;
        g_valid_graph_tag[valid_graph_count] = i_file;
      }
      valid_graph_count++;

      if (valid_graph_count > 1000) {
        break;
      }
    }
    
    read_count++;
    if (read_count > 1000) {
      break;
    }
  }
  g_valid_graph_count = valid_graph_count;
  cout << "g_valid_graph_count = " <<  g_valid_graph_count << endl;
  //fscanf(file, "%lld", &i_file);

  return 0;

}

void update2(int index) {
  int i = 0;
  int j = 0;
  double line1_sx = 0, line1_sy = 0;
  double line1_ex = 0, line1_ey = 0;
  double line2_sx = 0, line2_sy = 0;
  double line2_ex = 0, line2_ey = 0;
  double dist = 0.0;
  // cout << "Enter into update " << index << endl;
  if (index < MAX_VALID_GRAPH_COUNT && index < g_valid_graph_count) {
    long long tag = g_valid_graph_tag[index];
    int line_count = count_one_bits(tag);

    // cout << "tag is " << tag << endl;
    // cout << "line count is " << line_count << endl;
    // cout << "line list: " << endl;

    long long tag_tmp = tag;
    int start_point = 0; // g_lines[tag][0];
    int end_point   = 0; // g_lines[tag][1];
    int line_tag_tmp = 0;


    g_count = 0;
    for (i = 0; i < g_lines_count; ++i) {
      if ((tag_tmp & 1) > 0) {
        start_point = g_lines[i][0];
        end_point   = g_lines[i][1];
        // cout << "  line " << line_tag_tmp++ << " : " << start_point << " -> " << end_point << "  "; 

        update_coord_by_index(start_point, end_point);
        // cout << "(" << coordx[g_count]     << ", " << coordy[g_count]     << ") -> " <<
        //        "(" << coordx[g_count + 1] << ", " << coordy[g_count + 1] << ")" << endl;
        g_count += 2;
      }
      tag_tmp = tag_tmp >> 1;
    }

    double c1x = 0;
    double c1y = 0;
    double c2x = 0;
    double c2y = 0;
    double s = 0;
    double t = 0;
    double dist_min = 10.0;
    double s_min = 0.0;
    double t_min = 0.0;
    for (i = 0; i < g_count; i += 2) {
      line1_sx = coordx[i];
      line1_sy = coordy[i];
      line1_ex = coordx[i + 1];
      line1_ey = coordy[i + 1];
      for (j = i + 2; j < g_count; j += 2) {
        line2_sx = coordx[j];
        line2_sy = coordy[j];
        line2_ex = coordx[j + 1];
        line2_ey = coordy[j + 1];
        bool is_in_one_line = false;
        if ((line1_ey - line1_sy) * (line2_ex - line2_sx) == (line1_ex - line1_sx) * (line2_ey - line2_sy)) {
          is_in_one_line = true;
        }
        dist = ClosestPtSegToSeg_2D(line1_sx, line1_sy, line1_ex, line1_ey,
          line2_sx, line2_sy, line2_ex, line2_ey, 
          &c1x, &c1y, &c2x, &c2y, &s, &t);
        if ((dist < dist_min) &&
            ((s > 0.01 && s < 0.99) || (t > 0.01 && t < 0.99))) {
          //cout << "dist = " << dist << ", s = " << s << " " << t <<endl;
          dist_min = dist;
          s_min = s;
          t_min = t;
        } else if (dist < 0.01 && is_in_one_line) { //  共线
          dist_min = 0;
          s_min = 0.5;
          t_min = 0.5;
        }
      }
    }

    //cout << "dist_min = " << dist_min << endl;
    if (dist_min < 0.01 && 
      ((s_min > 0.01 && s_min < 0.99) || (t_min > 0.01 && t_min < 0.99))) {
        // intersect
    } else {
      // log
      phoenix::common::LogFile log_file_status_(".");
      char str_buff_status_[1024*2];
      
      log_file_status_.Open("jdt2_filter.log");
      log_file_status_.Enable(true);
      std::snprintf(str_buff_status_, sizeof(str_buff_status_)-1,
                  "%lld\n", tag);
      log_file_status_.Write(str_buff_status_);
      //log_file_status_.Close();
      //log_file_status_.Open();
    
    }
  }
}

int update_coord_by_index(int start_point, int end_point) {
  double offset_x = 10.0;
  double offset_y = 10.0;
  double factor_x = 100.0;
  double factor_y = 100.0;

  if (start_point == 0) {
    coordx[g_count] = offset_x + 0 * factor_x;
    coordy[g_count] = offset_y + 0 * factor_y;
  } else if (start_point == 1) {
    coordx[g_count] = offset_x + 1 * factor_x;
    coordy[g_count] = offset_y + 0 * factor_y;
  } else if (start_point == 2) {
    coordx[g_count] = offset_x + 0 * factor_x;
    coordy[g_count] = offset_y + 1 * factor_y;
  } else if (start_point == 3) {
    coordx[g_count] = offset_x + 1 * factor_x;
    coordy[g_count] = offset_y + 1 * factor_y;
  } else if (start_point == 4) {
    coordx[g_count] = offset_x + 2 * factor_x;
    coordy[g_count] = offset_y + 0 * factor_y;
  } else if (start_point == 5) {
    coordx[g_count] = offset_x + 2 * factor_x;
    coordy[g_count] = offset_y + 1 * factor_y;
  } else if (start_point == 6) {
    coordx[g_count] = offset_x + 0 * factor_x;
    coordy[g_count] = offset_y + 2 * factor_y;
  } else if (start_point == 7) {
    coordx[g_count] = offset_x + 1 * factor_x;
    coordy[g_count] = offset_y + 2 * factor_y;
  } else if (start_point == 8) {
    coordx[g_count] = offset_x + 2 * factor_x;
    coordy[g_count] = offset_y + 2 * factor_y;
  } 

  if (end_point == 0) {
    coordx[g_count + 1] = offset_x + 0 * factor_x;
    coordy[g_count + 1] = offset_y + 0 * factor_y;
  } else if (end_point == 1) {
    coordx[g_count + 1] = offset_x + 1 * factor_x;
    coordy[g_count + 1] = offset_y + 0 * factor_y;
  } else if (end_point == 2) {
    coordx[g_count + 1] = offset_x + 0 * factor_x;
    coordy[g_count + 1] = offset_y + 1 * factor_y;
  } else if (end_point == 3) {
    coordx[g_count + 1] = offset_x + 1 * factor_x;
    coordy[g_count + 1] = offset_y + 1 * factor_y;
  } else if (end_point == 4) {
    coordx[g_count + 1] = offset_x + 2 * factor_x;
    coordy[g_count + 1] = offset_y + 0 * factor_y;
  } else if (end_point == 5) {
    coordx[g_count + 1] = offset_x + 2 * factor_x;
    coordy[g_count + 1] = offset_y + 1 * factor_y;
  } else if (end_point == 6) {
    coordx[g_count + 1] = offset_x + 0 * factor_x;
    coordy[g_count + 1] = offset_y + 2 * factor_y;
  } else if (end_point == 7) {
    coordx[g_count + 1] = offset_x + 1 * factor_x;
    coordy[g_count + 1] = offset_y + 2 * factor_y;
  } else if (end_point == 8) {
    coordx[g_count + 1] = offset_x + 2 * factor_x;
    coordy[g_count + 1] = offset_y + 2 * factor_y;
  } else if (end_point == 9) {
    coordx[g_count + 1] = offset_x + 3 * factor_x;
    coordy[g_count + 1] = offset_y + 0 * factor_y;
  } 
}

int update_coord_by_index0(int index_point, double *px, double *py) {
  double offset_x = 10.0;
  double offset_y = 10.0;
  double factor_x = 100.0;
  double factor_y = 100.0;

  if (index_point == 0) {
    *px = offset_x + 0 * factor_x;
    *py = offset_y + 0 * factor_y;
  } else if (index_point == 1) {
    *px = offset_x + 1 * factor_x;
    *py = offset_y + 0 * factor_y;
  } else if (index_point == 2) {
    *px = offset_x + 0 * factor_x;
    *py = offset_y + 1 * factor_y;
  } else if (index_point == 3) {
    *px = offset_x + 1 * factor_x;
    *py = offset_y + 1 * factor_y;
  } else if (index_point == 4) {
    *px = offset_x + 2 * factor_x;
    *py = offset_y + 0 * factor_y;
  } else if (index_point == 5) {
    *px = offset_x + 2 * factor_x;
    *py = offset_y + 1 * factor_y;
  } else if (index_point == 6) {
    *px = offset_x + 0 * factor_x;
    *py = offset_y + 2 * factor_y;
  } else if (index_point == 7) {
    *px = offset_x + 1 * factor_x;
    *py = offset_y + 2 * factor_y;
  } else if (index_point == 8) {
    *px = offset_x + 2 * factor_x;
    *py = offset_y + 2 * factor_y;
  } else if (index_point == 9) {
    *px = offset_x + 3 * factor_x;
    *py = offset_y + 0 * factor_y;
  } else if (index_point == 10) {
    *px = offset_x + 3 * factor_x;
    *py = offset_y + 1 * factor_y;
  } else if (index_point == 11) {
    *px = offset_x + 3 * factor_x;
    *py = offset_y + 2 * factor_y;
  } else if (index_point == 12) {
    *px = offset_x + 0 * factor_x;
    *py = offset_y + 3 * factor_y;
  } else if (index_point == 13) {
    *px = offset_x + 1 * factor_x;
    *py = offset_y + 3 * factor_y;
  } else if (index_point == 14) {
    *px = offset_x + 2 * factor_x;
    *py = offset_y + 3 * factor_y;
  } else if (index_point == 15) {
    *px = offset_x + 3 * factor_x;
    *py = offset_y + 3 * factor_y;
  }
}



