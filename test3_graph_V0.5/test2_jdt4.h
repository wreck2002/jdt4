/** @copyright Copyright (c) 2020 Wreck.
 ******************************************************************************
// Filename: test2_jdt4.h
//
 ******************************************************************************/

#ifndef TEST2_JDT4_H_
#define TEST2_JDT4_H_


#define JDT_K   4
#define JDT4_GRID_SIZE  7
// point count
#define JDT4_N  16
// minimum line count per point
#define JDT4_MIN_INCIDENCE  2

int main_jdt4();
void update4(int index);

extern int g_count;
// 1 : reserve
// 10 : load graphs from file, and display
// 11 : load graphs from file, but not display
extern int g_option;

#endif  // TEST2_JDT4_H_


