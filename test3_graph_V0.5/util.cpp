/* Copyright 2018,2019 Kotei Co., Ltd.
 ******************************************************************************
 ** Some common operator
 ******************************************************************************
 *
 *  Some common operator
 *
 *  @file       util.cc
 *
 *  @author     kotei
 *  @date       2018.11.22
 *  @version    001 2018.11.22  新规作成
 *
 ******************************************************************************
 *  All Rights Reserved. Copyright(C) 2018 Kotei Co., Ltd.
 ******************************************************************************/
#include "util.h"

#include <vector>
#include <cmath>
#include <iostream>
using namespace std;

namespace phoenix {
namespace common {


char LogFile::log_dir_[1024] = { 0 };

void LogFile::SetLogDir(const std::string& log_dir) {
  std::snprintf(log_dir_, sizeof(log_dir_)-2, "%s", log_dir.c_str());
}

LogFile::LogFile(const char* module_name) {
  static int file_index = 1;

  if (nullptr != module_name) {
    std::snprintf(module_name_, MAX_NAME_SIZE-1, "%s", module_name);
  } else {
    std::snprintf(module_name_, MAX_NAME_SIZE-1, "null_name_%d", file_index);
  }

  file_is_open_ = false;
  enable_ = false;
}

LogFile::~LogFile() {
  Close();
}

bool LogFile::Open(void) {
  if (file_is_open_) {
    return (true);
  }

  snprintf(file_name_, MAX_FILE_NAME-1, "./jdt2.log");

    //std::cout<<"log_dir="<<log_dir_<<", file_name="<<file_name_<<std::endl;

  file_ = fopen(file_name_, "at+");
  if (nullptr == file_) {
    // AERROR << "Open file \"" << file_name_ << "\" failed.";
    return (false);
  }

  file_is_open_ = true;

  return (true);
}

bool LogFile::Open(const char *filename) {
  if (file_is_open_) {
    return (true);
  }

  snprintf(file_name_, MAX_FILE_NAME-1, "./%s", filename);

    //std::cout<<"log_dir="<<log_dir_<<", file_name="<<file_name_<<std::endl;

  file_ = fopen(file_name_, "at+");
  if (nullptr == file_) {
    // AERROR << "Open file \"" << file_name_ << "\" failed.";
    return (false);
  }

  file_is_open_ = true;

  return (true);
}

bool LogFile::Close(void) {
  if (file_is_open_) {
    file_is_open_ = false;
    enable_ = false;
    if (NULL != file_) {
      fclose(file_);
      file_ = NULL;
    }
  }

  return (true);
}

void LogFile::Write(const char* str) {
  if (file_is_open_ && enable_ && nullptr != str) {
    fputs(str, file_);
  }
}


/******************************************************************************/
/** 计算线段P1Q1与线段P2Q2之间的最近点，及最近点间的平方距离
 ******************************************************************************
 *  @param      p1           (IN)          线段P1Q1的起点
 *              q1           (IN)          线段P1Q1的终点
 *              p2           (IN)          线段P2Q2的起点
 *              q2           (IN)          线段P2Q2的终点
 *              c1           (OUT)         线段P1Q1上的最近点
 *              c2           (OUT)         线段P2Q2上的最近点
 *              s            (OUT)         线段P1Q1上的最近点在线段P1Q1上的比例
 *              t            (OUT)         线段P2Q2上的最近点在线段P2Q2上的比例
 *
 *  @retval     线段P1Q1与线段P2Q2之间的平方距离
 *
 *  @version    001 2018.11.22   pengc         新规作成
 *
 *  <Function>
 *       Computes closest points C1 and C2 of S1(s)=P1+s*(Q1-P1) and
 *       S2(t)=P2+t*(Q2-P2), returning s and t. Function result is squared
 *       distance between between S1(s) and S2(t).
 *
 *  <Attention>
 *       None
 *
 */
double ClosestPtSegToSeg_2D(
    const double p1x, const double p1y,
    const double q1x, const double q1y,
    const double p2x, const double p2y,
    const double q2x, const double q2y,
    double *c1x, double *c1y,
    double *c2x, double *c2y,
    double* s, double* t) {
  // Vec2d d1 = q1 - p1;   // Direction vector of segment S1
  double d1x = q1x - p1x;
  double d1y = q1y - p1y;
  // Vec2d d2 = q2 - p2;   // Direction vector of segment S2
  double d2x = q2x - p2x;
  double d2y = q2y - p2y;
  // Vec2d r = p1 - p2;
  double rx = p1x - p2x;
  double ry = p1y - p2y;
  // Squared length of segment S1, always nonnegative
  // double a = d1.LengthSquare();
  double a = d1x * d1x + d1y * d1y;
  // Squared length of segment S2, always nonnegative
  // double e = d2.LengthSquare();
  double e = d2x * d2x + d2y * d2y;
  // double f = d2.InnerProd(r);
  double f = d2x * rx + d2y * ry;

  // Check if either or both segments degenerate into points
  if ((a <= kGeometryEpsilon) && (e <= kGeometryEpsilon)) {
    // Both segments degenerate into points
    *s = 0.0;
    *t = 0.0;
    *c1x = p1x;
    *c1y = p1y;
    *c2x = p2x;
    *c2y = p2y;
    //return ((*c1 - *c2).LengthSquare());
    return (*c1x - *c2x) * (*c1x - *c2x) + (*c1y - *c2y) * (*c1y - *c2y);
  }
  if (a <= kGeometryEpsilon) {
    // First segment degenerates into a point
    *s = 0.0;
    // s = 0 => t = (b*s + f) / e = f / e
    *t = f / e;
    // *t = Clamp(*t, 0.0, 1.0);
    if (*t < 0.0) {
      *t = 0.0;
    } else if (*t > 1.0) {
      *t = 1.0;
    }
  } else {
    // double c = d1.InnerProd(r);
    double c = d1x * rx + d1y * ry;
    if (e <= kGeometryEpsilon) {
      // Second segment degenerates into a point
      // t = 0 => s = (b*t - c) / a = -c / a
      *t = 0.0;
      // *s = Clamp(-c/a, 0.0, 1.0);
      *s = -c/a;
      if (*s < 0.0) {
        *s = 0.0;
      } else if (*s > 1.0) {
        *s = 1.0;
      }
    } else {
      // The general nondegenerate case starts here
      // double b = d1.InnerProd(d2);
      double b = d1x * d2x + d1y * d2y;
      double denom = a * e - b * b;   // Always nonnegative

      // If segments not parallel, compute closest point on L1 to L2 and
      // clamp to segment S1. Else pick arbitrary s (here 0)
      if (denom != 0.0f) {
        // *s = Clamp((b*f - c*e) / denom, 0.0, 1.0);
        *s = (b*f - c*e) / denom;
        if (*s < 0.0) {
          *s = 0.0;
        } else if (*s > 1.0) {
          *s = 1.0;
        }
      } else {
        *s = 0.0;
      }
      // Compute point on L2 closest to S1(s) using
      // t = Dot((P1 + D1*s) - P2,D2) / Dot(D2,D2) = (b*s + f) / e
      *t = (b*(*s) + f) / e;

      // If t in [0,1] done. Else clamp t, recompute s for the new value
      // of t using s = Dot((P2 + D2*t) - P1,D1) / Dot(D1,D1)= (t*b - c) / a
      // and clamp s to [0, 1]
      if (*t < 0.0) {
        *t = 0.0;
        // *s = Clamp(-c / a, 0.0, 1.0);
        *s = -c / a;
        if (*s < 0.0) {
          *s = 0.0;
        } else if (*s > 1.0) {
          *s = 1.0;
        }
      } else if (*t > 1.0) {
        *t = 1.0;
        // *s = Clamp((b - c) / a, 0.0, 1.0);
        *s = (b - c) / a;
        if (*s < 0.0) {
          *s = 0.0;
        } else if (*s > 1.0) {
          *s = 1.0;
        }
      }
    }
  }

  // *c1 = p1 + (*s) * d1;
  *c1x = p1x + (*s) * d1x;
  *c1y = p1y + (*s) * d1y;
  // *c2 = p2 + (*t) * d2;
  *c2x = p2x + (*t) * d2x;
  *c2y = p2y + (*t) * d2y;

  return (*c1x - *c2x) * (*c1x - *c2x) + (*c1y - *c2y) * (*c1y - *c2y);
}



}  // namespace common
}  // namespace phoenix
