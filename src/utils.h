#include <vector>
#include <opencv2/opencv.hpp>

struct Point2 {
  int x, y;
  Point2(int xx = 0, int yy = 0): x(xx), y(yy) {}
};

struct ETNode {
  double x;
  double dx;
  int yMax;

  ETNode(double xx = 0, double ddx = 0, int yyMax = 0): x(xx), dx(ddx), yMax(yyMax) {}
};

struct Color3 {
  int r, g, b;

  Color3(int rr, int gg, int bb): r(rr), g(gg), b(bb) {}
};

struct Color4{
  int r, g, b, a;

  Color4(int rr, int gg, int bb, int aa): r(rr), g(gg), b(bb), a(aa) {}
};


void drawPixel4(cv::Mat & image, int x, int y, int r, int g, int b);

void drawPixel3(cv::Mat &image, int x, int y, int r, int g, int b, int a);

void drawLine(cv::Mat &image, int x1, int y1, int x2, int y2, const Color3 &c);

void fill(cv::Mat &image, const std::vector<Point2>& vp, const Color3 &c);

void drawCircle(cv::Mat &image, int x, int y, int r, int ang1, int ang2, const Color3 &c);

bool inArc(int x, int y, int ang1, int ang2);

void circlePoint(cv::Mat &image, int x, int y, int cx, int cy, int ang1, int ang2, const Color3 &c);

void polygonCircle(cv::Mat &image, int x, int y, int t, int ang1, int ang2, const Color3 &c);

void midCircle(cv::Mat &image, int x, int y, int r, int ang1, int ang2, const Color3 &c);
