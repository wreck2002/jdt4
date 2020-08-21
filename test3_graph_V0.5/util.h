/* Copyright 2018,2019 Kotei Co., Ltd.
 ******************************************************************************
 ** Some common operator
 ******************************************************************************
 *
 *  Some common operator
 *
 *  @file       util.h
 *
 *  @author     kotei
 *  @date       2018.11.22
 *  @version    001 2018.11.22  新规作成
 *              002 2019.05.21  将使用ROS函数获取耗时的方法修改为
 *                              使用linux系统函数的方式
 *
 ******************************************************************************
 *  All Rights Reserved. Copyright(C) 2018,2019 Kotei Co., Ltd.
 ******************************************************************************/

/**
 * @file
 * @brief Some util functions.
 */

#ifndef PHOENIX_COMMON_UTIL_H_
#define PHOENIX_COMMON_UTIL_H_

#include <algorithm>
#include <iostream>
#include <memory>
#include <string>
#include <unordered_set>
#include <utility>
#include <vector>

/**
 * @namespace phoenix::common
 * @brief phoenix::common
 */
namespace phoenix {
namespace common {

const double kGeometryEpsilon = 1e-6;

double ClosestPtSegToSeg_2D(
    const double p1x, const double p1y,
    const double q1x, const double q1y,
    const double p2x, const double p2y,
    const double q2x, const double q2y,
    double *c1x, double *c1y,
    double *c2x, double *c2y,
    double* s, double* t);

class LogFile {
 public:
  explicit LogFile(const char* module_name);
  ~LogFile();

  static void SetLogDir(const std::string& log_dir);

  bool IsEnable(void) { return (enable_); }
  void Enable(bool enable) {
    if (file_is_open_) {
      enable_ = enable;
    } else {
      enable_ = false;
    }
  }
  bool Open(void);
  bool Open(const char *filename);
  bool Close(void);
  void Write(const char* str);

 private:
  enum { MAX_NAME_SIZE = 64 };
  char module_name_[MAX_NAME_SIZE];
  bool enable_;
  bool file_is_open_;
  FILE* file_;
  enum { MAX_FILE_NAME = 1024 };
  char file_name_[MAX_FILE_NAME];

  // static std::string log_dir_;
  static char log_dir_[1024];
};


}  // namespace common
}  // namespace phoenix



#endif  // PHOENIX_COMMON_UTIL_H_
