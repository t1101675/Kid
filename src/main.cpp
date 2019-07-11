#include <vector>
#include <iostream>

#include "utils.h"

int main(int argc, char** argv) {
  cv::Mat image(2000, 1600, CV_8UC3, cv::Scalar(255, 255, 255));
  Color3 c(0, 0, 0);
  /*hat*/
  drawLine(image, 800, 1280, 1130, 1335, c);
  drawLine(image, 800, 1280, 863, 900, c);
  drawLine(image, 1130, 1335, 1193, 955, c);
  drawLine(image, 663, 867, 1393, 988, c);
  drawLine(image, 850, 980, 1180, 1035, c);

  /*face*/
  drawLine(image, 1193, 955, 1210, 855, c);
  drawCircle(image, 928, 880, 102, 172, 386, c);
  drawLine(image, 1210, 855, 1020, 926, c);
  Point2 p[3] = {Point2(1210, 855), Point2(1020, 926), Point2(1193, 955)};
  std::vector<Point2> vp(p, p + 3);
  fill(image, vp, c);

  /*mouth*/
  drawCircle(image, 1044, 824, 168, 192, 373, c);
  drawLine(image, 932, 700, 1190, 743, c);
  drawLine(image, 954, 703, 960, 679, c);
  drawLine(image, 1004, 712, 1012, 661, c);
  drawLine(image, 1054, 721, 1064, 659, c);
  drawLine(image, 1104, 729, 1114, 674, c);
  drawLine(image, 1154, 737, 1160, 706, c);

  /*triangle*/
  drawLine(image, 750, 880, 750, 690, c);
  drawLine(image, 750, 690, 730, 670, c);
  drawLine(image, 750, 690, 770, 670, c);
  drawLine(image, 730, 670, 770, 670, c);
  Point2 pTriangle[3] = {Point2(750, 690), Point2(730, 670), Point2(770, 670)};
  std::vector<Point2> vpTriangle(pTriangle, pTriangle + 3);
  fill(image, vpTriangle, c);

  cv::Mat temp_image;
  cv::transpose(image, temp_image);
  cv::flip(temp_image, image, 0);
  cv::imwrite("kid.jpg", image);
  return 0;
}
