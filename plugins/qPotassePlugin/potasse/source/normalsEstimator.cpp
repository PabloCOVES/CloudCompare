#include <potasse/estimator/normalsEstimator.hpp>

#include <potasse/estimator/exception.hpp>

#include <Eigen/Dense>

#include <iostream>  // TODO delete

namespace potasse
{
  namespace estimator
  {
    template <typename PointCloud, typename NormalCloud>
    NormalsEstimator<PointCloud, NormalCloud>::NormalsEstimator(Strategy const &strategy)
        : strategy_(strategy)
    {
    }

    template <typename PointCloud, typename NormalCloud>
    NormalCloud
    NormalsEstimator<PointCloud, NormalCloud>::compute(PointCloud const &pointCloud)
    {
      if (!static_cast<bool>(strategy_))
        throw Exception("Null strategy pointer.", __FILE__, __LINE__);

      typedef typename NormalCloud::value_type Normal;
      typedef typename Normal::value_type Scalar;
      static const size_t Dimension = std::tuple_size<Normal>::value;

      typedef Eigen::Matrix<Scalar, Dimension, Dimension> Matrix;
      typedef Eigen::Matrix<Scalar, Dimension, 1> Vector;

      NormalCloud output;
      output.resize(pointCloud.size());

      std::cout << "Computing neighbors:\t" << std::flush;
      typename Strategy::element_type::Neighbors neighbors(strategy_->compute(pointCloud));
      std::cout << "Done." << std::endl;

#pragma omp parallel for
      for (size_t i = 0; i < pointCloud.size(); ++i)
      {
        Vector centroid(Vector::Zero());
        for (size_t const &id : neighbors.indices_[i])
          centroid += pointCloud[id].point();
        centroid /= neighbors.indices_[i].size();

        Matrix matrix(Matrix::Zero());
        for (size_t j(0); j < neighbors.indices_[i].size(); ++j)
        {
          Vector const vector(pointCloud[neighbors.indices_[i][j]].point() - centroid);
          matrix += vector * vector.transpose() / std::exp(neighbors.distances_[i][j]);
        }
        output[i].normal() = Eigen::SelfAdjointEigenSolver<Matrix>(matrix).eigenvectors().col(0);
      }

      return output;
    }

    template <typename PointCloud, typename NormalCloud>
    NormalCloud
    NormalsEstimator<PointCloud, NormalCloud>::compute(Strategy const &strategy,
                                                       PointCloud const &pointCloud)
    {
      NormalsEstimator normalEstimator(strategy);
      return normalEstimator.compute(pointCloud);
    }
  }
}

#include <potasse/estimator/neighborsEstimator.hpp>

#include <potasse/cloud/cloud.hpp>
#include <potasse/cloud/normal.hpp>
#include <potasse/cloud/point.hpp>

typedef potasse::cloud::Point<double, 3> Point;
typedef potasse::cloud::Cloud<Point> PointCloud;

typedef potasse::cloud::Normal<double, 3> Normal;
typedef potasse::cloud::Cloud<Normal> NormalCloud;

template struct potasse::estimator::NormalsEstimator<PointCloud, NormalCloud>;
