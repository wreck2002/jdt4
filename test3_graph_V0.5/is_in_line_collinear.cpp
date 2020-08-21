#include "is_in_line.h"


bool is_in_line_collinear(int start_tag1, int end_tag1, int start_tag2, int end_tag2) {
  // 重叠
  if (start_tag1 == 0 && end_tag1 == 1 && start_tag2 == 0 && end_tag2 == 4) {
    return true;
  }
  if (start_tag1 == 0 && end_tag1 == 1 && start_tag2 == 1 && end_tag2 == 4) {
    return true;
  }
  if (start_tag1 == 0 && end_tag1 == 4 && start_tag2 == 1 && end_tag2 == 2) {
    return true;
  }
  if (start_tag1 == 0 && end_tag1 == 4 && start_tag2 == 1 && end_tag2 == 3) {
    return true;
  }
  if (start_tag1 == 0 && end_tag1 == 4 && start_tag2 == 1 && end_tag2 == 4) {
    return true;
  }
  if (start_tag1 == 0 && end_tag1 == 3 && start_tag2 == 2 && end_tag2 == 5) {
    return true;
  }
  if (start_tag1 == 0 && end_tag1 == 4 && start_tag2 == 1 && end_tag2 == 5) {
    return true;
  }
  if (start_tag1 == 1 && end_tag1 == 3 && start_tag2 == 2 && end_tag2 == 5) {
    return true;
  }
  if (start_tag1 == 2 && end_tag1 == 3 && start_tag2 == 2 && end_tag2 == 5) {
    return true;
  }
  if (start_tag1 == 2 && end_tag1 == 3 && start_tag2 == 3 && end_tag2 == 5) {
    return true;
  }
  if (start_tag1 == 2 && end_tag1 == 5 && start_tag2 == 3 && end_tag2 == 4) {
    return true;
  }
  if (start_tag1 == 2 && end_tag1 == 5 && start_tag2 == 3 && end_tag2 == 5) {
    return true;
  }
  if (start_tag1 == 0 && end_tag1 == 2 && start_tag2 == 0 && end_tag2 == 6) {
    return true;
  }
  if (start_tag1 == 0 && end_tag1 == 2 && start_tag2 == 2 && end_tag2 == 6) {
    return true;
  }
  if (start_tag1 == 0 && end_tag1 == 6 && start_tag2 == 1 && end_tag2 == 2) {
    return true;
  }
  if (start_tag1 == 0 && end_tag1 == 6 && start_tag2 == 2 && end_tag2 == 3) {
    return true;
  }
  if (start_tag1 == 0 && end_tag1 == 6 && start_tag2 == 2 && end_tag2 == 4) {
    return true;
  }
  if (start_tag1 == 0 && end_tag1 == 3 && start_tag2 == 4 && end_tag2 == 6) {
    return true;
  }
  if (start_tag1 == 0 && end_tag1 == 6 && start_tag2 == 2 && end_tag2 == 6) {
    return true;
  }
  if (start_tag1 == 1 && end_tag1 == 3 && start_tag2 == 4 && end_tag2 == 6) {
    return true;
  }
  if (start_tag1 == 2 && end_tag1 == 3 && start_tag2 == 4 && end_tag2 == 6) {
    return true;
  }
  if (start_tag1 == 2 && end_tag1 == 5 && start_tag2 == 3 && end_tag2 == 6) {
    return true;
  }
  if (start_tag1 == 2 && end_tag1 == 5 && start_tag2 == 3 && end_tag2 == 7) {
    return true;
  }
  if (start_tag1 == 3 && end_tag1 == 4 && start_tag2 == 3 && end_tag2 == 6) {
    return true;
  }
  if (start_tag1 == 3 && end_tag1 == 4 && start_tag2 == 4 && end_tag2 == 6) {
    return true;
  }
  if (start_tag1 == 3 && end_tag1 == 5 && start_tag2 == 4 && end_tag2 == 6) {
    return true;
  }
  if (start_tag1 == 3 && end_tag1 == 6 && start_tag2 == 4 && end_tag2 == 6) {
    return true;
  }
  if (start_tag1 == 0 && end_tag1 == 3 && start_tag2 == 1 && end_tag2 == 7) {
    return true;
  }
  if (start_tag1 == 1 && end_tag1 == 3 && start_tag2 == 1 && end_tag2 == 7) {
    return true;
  }
  if (start_tag1 == 1 && end_tag1 == 3 && start_tag2 == 3 && end_tag2 == 7) {
    return true;
  }
  if (start_tag1 == 1 && end_tag1 == 7 && start_tag2 == 3 && end_tag2 == 4) {
    return true;
  }
  if (start_tag1 == 1 && end_tag1 == 7 && start_tag2 == 3 && end_tag2 == 5) {
    return true;
  }
  if (start_tag1 == 1 && end_tag1 == 7 && start_tag2 == 3 && end_tag2 == 7) {
    return true;
  }
  if (start_tag1 == 2 && end_tag1 == 3 && start_tag2 == 1 && end_tag2 == 7) {
    return true;
  }
  if (start_tag1 == 3 && end_tag1 == 6 && start_tag2 == 1 && end_tag2 == 7) {
    return true;
  }
  if (start_tag1 == 0 && end_tag1 == 3 && start_tag2 == 0 && end_tag2 == 8) {
    return true;
  }
  if (start_tag1 == 4 && end_tag1 == 5 && start_tag2 == 4 && end_tag2 == 8) {
    return true;
  }
  if (start_tag1 == 4 && end_tag1 == 5 && start_tag2 == 5 && end_tag2 == 8) {
    return true;
  }
  if (start_tag1 == 6 && end_tag1 == 7 && start_tag2 == 6 && end_tag2 == 8) {
    return true;
  }
  if (start_tag1 == 6 && end_tag1 == 7 && start_tag2 == 7 && end_tag2 == 8) {
    return true;
  }
  if (start_tag1 == 0 && end_tag1 == 1 && start_tag2 == 0 && end_tag2 == 9) {
    return true;
  }
  if (start_tag1 == 0 && end_tag1 == 1 && start_tag2 == 1 && end_tag2 == 9) {
    return true;
  }
  if (start_tag1 == 0 && end_tag1 == 4 && start_tag2 == 0 && end_tag2 == 9) {
    return true;
  }
  if (start_tag1 == 0 && end_tag1 == 4 && start_tag2 == 1 && end_tag2 == 9) {
    return true;
  }
  if (start_tag1 == 0 && end_tag1 == 4 && start_tag2 == 4 && end_tag2 == 9) {
    return true;
  }
  if (start_tag1 == 0 && end_tag1 == 9 && start_tag2 == 1 && end_tag2 == 4) {
    return true;
  }
  if (start_tag1 == 0 && end_tag1 == 9 && start_tag2 == 1 && end_tag2 == 9) {
    return true;
  }
  if (start_tag1 == 0 && end_tag1 == 9 && start_tag2 == 4 && end_tag2 == 9) {
    return true;
  }
  if (start_tag1 == 1 && end_tag1 == 4 && start_tag2 == 1 && end_tag2 == 9) {
    return true;
  }

  return false;
}

