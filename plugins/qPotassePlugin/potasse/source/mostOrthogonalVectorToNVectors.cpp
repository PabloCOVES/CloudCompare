#include <potasse/estimator/mostOrthogonalVectorToNVectors.hpp>

#include <Eigen/Dense>

namespace potasse
{
  namespace estimator
  {
    template <typename NormalCloud>
    typename MostOrthogonalVectorToNVectors<NormalCloud>::Normal
    MostOrthogonalVectorToNVectors<NormalCloud>::compute(NormalCloud const &normalCloud,
                                                                     Indices const &indices)
    {
      typedef Eigen::Matrix<typename Normal::value_type, std::tuple_size<Normal>::value, std::tuple_size<Normal>::value> Matrix;
      typedef Eigen::Matrix<typename Normal::value_type, std::tuple_size<Normal>::value, 1> Vector;

      Normal output;

      Matrix matrix(Matrix::Zero());
      for (auto const &i : indices)
      {
        Vector normal(normalCloud[i].normal());

        matrix(0, 0) += normal(0) * normal(0);
        matrix(0, 1) += normal(0) * normal(1);
        matrix(0, 2) += normal(0) * normal(1);

        matrix(1, 0) += normal(0) * normal(1);
        matrix(1, 1) += normal(1) * normal(1);
        matrix(1, 2) += normal(1) * normal(2);

        matrix(2, 0) += normal(0) * normal(2);
        matrix(2, 1) += normal(1) * normal(2);
        matrix(2, 2) += normal(2) * normal(2);
      }
      output.normal() = Eigen::SelfAdjointEigenSolver<Matrix>(matrix).eigenvectors().col(0);

      return output;
    }
  }
}

#include <potasse/cloud/cloud.hpp>
#include <potasse/cloud/normal.hpp>

typedef potasse::cloud::Normal<double, 3> Normal;
typedef potasse::cloud::Cloud<Normal> NormalCloud;

template class potasse::estimator::MostOrthogonalVectorToNVectors<NormalCloud>;
