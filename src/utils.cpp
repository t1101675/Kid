#include <list>
#include <cmath>
#include <vector>
#include <assert.h>
#include <iostream>

#include "utils.h"

const double PI = 3.14159265;
const int N = 100;
const double DEG_TO_PI = PI / 180.0;
const double PI_TO_DEG = 180.0 / PI;

void drawPixel3(cv::Mat &image, int x, int y, int r, int g, int b) {
  image.at<cv::Vec3b>(x, y)[0] = b;
  image.at<cv::Vec3b>(x, y)[1] = g;
  image.at<cv::Vec3b>(x, y)[2] = r;
}

void drawPixel4(cv::Mat &image, int x, int y, int r, int g, int b, double a) {
  image.at<cv::Vec3b>(x, y)[0] = 255 - int((255 - b) * a);
  image.at<cv::Vec3b>(x, y)[1] = 255 - int((255 - g) * a);
  image.at<cv::Vec3b>(x, y)[2] = 255 - int((255 - r) * a);
}

void drawLine(cv::Mat &image, int x1, int y1, int x2, int y2, const Color3 &c) {
  int dx = x2 - x1, dy = y2 - y1;
  int ux = dx > 0 ? 1 : -1, uy = dy > 0 ? 1 : -1;
  int dxAbs = abs(dx), dyAbs = abs(dy);
  int dxAbs2 = dxAbs << 1, dyAbs2 = dyAbs << 1;
  if (dxAbs > dyAbs) {
    int e = 0;
    for (int x = x1, y = y1; x != x2; x += ux) {
      double a = double(abs(e)) / dxAbs2;
      drawPixel4(image, x, y, c.r, c.g, c.b, 1 - a);
      drawPixel4(image, x, y + (e > 0 ? uy : -uy), c.r, c.g, c.b, a);
      e += dyAbs2;
      if (e >= dxAbs) {
        y += uy;
        e -= dxAbs2;
      }
    }
  }
  else {
    int e = 0;
    for (int y = y1, x = x1; y != y2; y += uy) {
      double a = double(abs(e)) / dyAbs2;
      drawPixel4(image, x, y, c.r, c.g, c.b, 1 - a);
      drawPixel4(image, x - (e > 0 ? uy : -uy), y, c.r, c.g, c.b, a);
      e += dxAbs2;
      if (e >= dyAbs) {
        x += ux;
        e -= dyAbs2;
      }
    }
  }
  drawPixel3(image, x2, y2, c.r, c.g, c.b);
}

void fill(cv::Mat &image, const std::vector<Point2>& vp, const Color3 &c) {
  if (vp.size() <= 0) return;
  int yMin = 0x7fffffff;
  int yMax = yMin + 1;
  for (int i = 0; i < vp.size(); i++) {
    if (vp[i].y > yMax) yMax = vp[i].y;
    if (vp[i].y < yMin) yMin = vp[i].y;
  }

  std::list<ETNode>* NET = new std::list<ETNode>[yMax - yMin + 1];
  for (int y = yMin, k = 0; y <= yMax; y++, k++) {
    for (int i = 0; i < vp.size(); i++) {
      if (vp[i].y == y) {
        int prev = i > 0 ? i - 1 : (vp.size() - 1);
        if (vp[prev].y > y) {
          int dy = vp[prev].y - vp[i].y;
          NET[k].push_back(ETNode((double)vp[i].x, dy ? (vp[prev].x - vp[i].x) / (double)dy : 0, vp[prev].y));
        }
        int post = (i + 1) < vp.size() ? i + 1 : 0;
        if (vp[post].y > y) {
          int dy = vp[post].y - vp[i].y;
          NET[k].push_back(ETNode((double)vp[i].x, dy ? (vp[post].x - vp[i].x) / (double)dy : 0, vp[post].y));
        }
      }
    }
  }

  std::list<ETNode> AET;
  for (int y = yMin, k = 0; y <= yMax; y++, k++) {
    std::list<ETNode>::iterator itNET = NET[k].begin(), itAET = AET.begin();

    /*delete edges y > yMax*/
    while (itAET != AET.end()) {
      if ((*itAET).yMax <= y) itAET = AET.erase(itAET);
      else ++itAET;
    }

    /*insert new edges*/
    for (; itNET != NET[k].end(); ++itNET) {
      itAET = AET.begin();
      while (itAET != AET.end()) {
        if ((*itAET).x < (*itNET).x) ++itAET;
        else if (((*itAET).x == (*itNET).x) && ((*itAET).dx < (*itNET).dx)) itAET++;
        else break;
      }
      AET.insert(itAET, *itNET);
    }
    /*after insertion, number of nodes in AET should %2 = 0*/

    /*draw vertex in a line*/
    itAET = AET.begin();
    while (itAET != AET.end()) {
      int xMin = (int)(*itAET).x;
      itAET++;
      int xMax = (int)(*itAET).x;
      itAET++;
      for (int x = xMin; x <= xMax; x++) {
        drawPixel3(image, x, y, c.b, c.g, c.r);
      }
    }

    /*update x*/
    for (itAET = AET.begin(); itAET != AET.end(); ++itAET) {
      (*itAET).x += (*itAET).dx;
    }
  }
  delete[] NET;
}

void polygonCircle(cv::Mat &image, int x, int y, int r, int ang1, int ang2, const Color3 &c) {
  double start = ang1 * DEG_TO_PI, end = ang2 * DEG_TO_PI;
  double delta = (end - start) / N;
  for (int i = 0; i < N; i++) {
    drawLine(image, x + int(r * cos(start + delta * i)),       y + int(r * sin(start + delta * i)),
             x + int(r * cos(start + delta * (i + 1))), y + int(r * sin(start + delta * (i + 1))), c);
  }
}

void midCircle(cv::Mat &image, int x, int y, int r, int ang1, int ang2, const Color3 &c) {
  int cx = 0, cy = r;
  double d = 1.25 - r;
  circlePoint(image, x, y, cx, cy, ang1, ang2, c);
  while (cx <= cy) {
    if (d < 0) d += 2 * cx + 3;
    else {
      d += 2 * (cx - cy) + 5;
      --cy;
    }
    ++cx;
    circlePoint(image, x, y, cx, cy, ang1, ang2, c);
  }
}

void circlePoint(cv::Mat &image, int x, int y, int cx, int cy, int ang1, int ang2, const Color3 &c) {
  if (inArc(+cx, +cy, ang1, ang2)) drawPixel3(image, x + cx, y + cy, c.r, c.g, c.b);
  if (inArc(-cx, +cy, ang1, ang2)) drawPixel3(image, x - cx, y + cy, c.r, c.g, c.b);
  if (inArc(+cx, -cy, ang1, ang2)) drawPixel3(image, x + cx, y - cy, c.r, c.g, c.b);
  if (inArc(-cx, -cy, ang1, ang2)) drawPixel3(image, x - cx, y - cy, c.r, c.g, c.b);
  if (inArc(+cy, +cx, ang1, ang2)) drawPixel3(image, x + cy, y + cx, c.r, c.g, c.b);
  if (inArc(+cy, -cx, ang1, ang2)) drawPixel3(image, x + cy, y - cx, c.r, c.g, c.b);
  if (inArc(-cy, +cx, ang1, ang2)) drawPixel3(image, x - cy, y + cx, c.r, c.g, c.b);
  if (inArc(-cy, -cx, ang1, ang2)) drawPixel3(image, x - cy, y - cx, c.r, c.g, c.b);
}

void drawCircle(cv::Mat &image, int x, int y, int r, int ang1, int ang2, const Color3 &c) {
  midCircle(image, x, y, r, ang1, ang2, c);
}

bool inArc(int x, int y, int ang1, int ang2) {
  int ang = int(atan2(double(y), double(x)) * PI_TO_DEG);
  ang = ang > 0 ? ang : 360 + ang;
  if (ang < ang1) return ang + 360 <= ang2;
  else return ang <= ang2;
}
