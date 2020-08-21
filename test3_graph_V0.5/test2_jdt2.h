// Filename: test2_jdt2.h
//

#ifndef TEST2_JDT2_H
#define TEST2_JDT2_H

// 最多有效图形的个数
#define MAX_VALID_GRAPH_COUNT 2000
// vertax_count_min
#define VERTAX_COUNT_MIN  9

void update(int index);
int main_jdt2();
int read_graph_from_file();
int update_coord_by_index(int start_point, int end_point);
int update_coord_by_index0(int index_point, double *px, double *py);
void update2(int index);

extern int g_count;
// 1 : display image
// 2 : load graph index from files
// 3 : load graph index , but not display
extern int g_option;


#endif // TEST2_JDT2_H




