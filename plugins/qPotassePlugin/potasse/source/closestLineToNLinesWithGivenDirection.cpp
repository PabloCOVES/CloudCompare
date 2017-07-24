#include <potasse/estimator/closestLineToNLinesWithGivenDirection.hpp>

#include <Eigen/Dense>

#include<iostream> // TODO Delete

namespace potasse
{
  namespace estimator
  {
    template <typename PointCloud, typename NormalCloud>
    typename ClosestLineToNLinesWithGivenDirection<PointCloud, NormalCloud>::Point
    ClosestLineToNLinesWithGivenDirection<PointCloud, NormalCloud>::compute(PointCloud const &pointCloud,
                                                                            NormalCloud const &normalCloud,
                                                                            Indices const &indices,
                                                                            Normal const &normal)
    {
      typedef typename Point::value_type Scalar;
      static const std::size_t dimension = std::tuple_size<Point>::value;

      typedef Eigen::Matrix<Scalar, dimension, dimension> Matrix;
      typedef Eigen::Matrix<Scalar, dimension, 1> Vector;

      Matrix matrix(Matrix::Zero());
      Vector vector(Vector::Zero());

      for (auto const &i : indices)
      {
        Vector p(pointCloud[i].point());
        Vector n(normalCloud[i].normal());
        Vector m(n.cross(normal.normal()));
        Scalar q(m.dot(p));

        matrix(0, 0) += m(0) * m(0); matrix(0, 1) += m(0) * m(1); matrix(0, 2) += m(0) * m(2); 
        matrix(1, 0) += m(0) * m(1); matrix(1, 1) += m(1) * m(1); matrix(1, 2) += m(1) * m(2); 
        matrix(2, 0) += m(0) * m(2); matrix(2, 1) += m(1) * m(2); matrix(2, 2) += m(2) * m(2); 

        vector(0) += m(0) * q;
        vector(1) += m(1) * q;
        vector(2) += m(2) * q;
      }
      std::cout << std::endl << matrix << std::endl << std::endl << vector.transpose() << std::endl;

      Point output; output.point() = matrix.householderQr().solve(vector);
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

template class potasse::estimator::ClosestLineToNLinesWithGivenDirection<PointCloud, NormalCloud>;
