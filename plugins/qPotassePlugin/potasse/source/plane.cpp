#include<potasse/detector/plane.hpp>

#include<potasse/cloud/cloud.hpp>
#include<potasse/cloud/normal.hpp>
#include<potasse/cloud/point.hpp>

typedef potasse::cloud::Point<double, 3> Point;
typedef potasse::cloud::Normal<double, 3> Normal;

typedef potasse::cloud::Cloud<Point> PointCloud;
typedef potasse::cloud::Cloud<Normal> NormalCloud;

//template class potasse::detector::Plane<PointCloud, NormalCloud>;
