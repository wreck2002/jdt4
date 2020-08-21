#include "is_in_line.h"

bool is_in_line(int start_tag1, int end_tag1, int start_tag2, int end_tag2) {
  if (is_in_line_collinear(start_tag1, end_tag1, start_tag2, end_tag2)) {
    return true;
  }

  if (is_in_line_cross(start_tag1, end_tag1, start_tag2, end_tag2)) {
    return true;
  }

  return false;
}
