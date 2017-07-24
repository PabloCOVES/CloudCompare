#include <potasse/estimator/closestPointToNLines.hpp>

#include <Eigen/Dense>

namespace potasse
{
  namespace estimator
  {
    template <typename PointCloud, typename NormalCloud>
    typename ClosestPointToNLines<PointCloud, NormalCloud>::Point
    ClosestPointToNLines<PointCloud, NormalCloud>::compute(PointCloud const &pointCloud,
                                                           NormalCloud const &normalCloud,
                                                           Indices const &indices)
    {
      typedef typename Point::value_type Scalar;
      static const std::size_t dimension = std::tuple_size<Point>::value;

      typedef Eigen::Matrix<Scalar, dimension, dimension> Matrix;
      typedef Eigen::Matrix<Scalar, dimension, 1> Vector;

      Matrix matrix(Matrix::Zero());
      Vector vector(Vector::Zero());

      for (std::size_t const &i : indices)
      {
        Vector p(pointCloud[i].point());
        Vector n(normalCloud[i].normal());

        matrix(0, 0) += n(1) * n(1) + n(2) * n(2);
        matrix(0, 1) += -n(0) * n(1);
        matrix(0, 2) += -n(0) * n(2);
        matrix(1, 0) += -n(0) * n(1);
        matrix(1, 1) += n(0) * n(0) + n(2) * n(2);
        matrix(1, 2) += -n(1) * n(2);
        matrix(2, 0) += -n(0) * n(2);
        matrix(2, 1) += -n(1) * n(2);
        matrix(2, 2) += n(0) * n(0) + n(1) * n(1);

        vector(0) += p(0) * (n(1) * n(1) + n(2) * n(2)) - n(0) * (p(1) * n(1) + p(2) * n(2));
        vector(1) += p(1) * (n(0) * n(0) + n(2) * n(2)) - n(1) * (p(0) * n(0) + p(2) * n(2));
        vector(2) += p(2) * (n(0) * n(0) + n(1) * n(1)) - n(2) * (p(0) * n(0) + p(1) * n(1));
      }

      Point output;
      output.point() = matrix.householderQr().solve(vector);
      return output;
    }
  }
}

#include <potasse/cloud/cloud.hpp>
#include <potasse/cloud/normal.hpp>
#include <potasse/cloud/point.hpp>

typedef potasse::cloud::Point<double, 3> Point;
typedef potasse::cloud::Cloud<Point> PointCloud;

typedef potasse::cloud::Normal<double, 3> Normal;
typedef potasse::cloud::Cloud<Normal> NormalCloud;

template class potasse::estimator::ClosestPointToNLines<PointCloud, NormalCloud>;
