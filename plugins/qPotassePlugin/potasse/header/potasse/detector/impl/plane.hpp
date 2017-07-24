#include<potasse/detector/plane.hpp>

#include<potasse/shape/plane.hpp>

#include<Eigen/Eigenvalues>

#include<chrono>
#include<iterator>
#include<random>
#include<set>

namespace potasse
{
  namespace detector
  {
    //template<typename PointCloud, typename NormalCloud>
      //Plane<PointCloud, NormalCloud>::Plane(Parameter const &parameter):
        //ShapeDetector<PointCloud, NormalCloud>(parameter) {}

    //template<typename PointCloud, typename NormalCloud>
      //typename Plane<PointCloud, NormalCloud>::DetectedPtr
      //Plane<PointCloud, NormalCloud>::compute
      //( PointCloudConstPtr pointCloudConstPtr,
        //NormalCloudConstPtr normalCloudConstPtr,
        //VertexConstPtr const vertexConstPtr )
      //{
        //typedef typename  Indices::iterator Iterator;

        //Parameter const &parameter(this->parameter());

        //DetectedPtr output(nullptr);

        //Indices indices(vertexConstPtr->indices());
        //if (indices.size() < parameter.cardinal_)

        //std::default_random_engine generator(std::chrono::system_clock::now().time_since_epoch().count());
        //std::uniform_int_distribution<size_t> distribution(0, indices.size()-1);

        //for (size_t tries(0); tries < parameter.tries_; ++tries)
        //{
          //// Minimal support generation.
          //std::set<size_t> support;
          //while (support.size() != 3)
          //{
            //Iterator it(indices.begin());
            //for (size_t i(0), i_(distribution(generator)); i < i_; ++i, ++it);
            //support.insert(*it);
          //}

          //typedef std::shared_ptr<potasse::shape::Plane<Scalar>> PlanePtr;
          //PlanePtr planePtr(std::make_shared<potasse::shape::Plane<Scalar>>
              //( *pointCloudConstPtr[*support.begin()].point(),
                //*pointCloudConstPtr[*std::advance(support.begin(), 1)].point(),
                //*pointCloudConstPtr[*std::advance(support.begin(), 2)].point()));

          //// Minimal support validation.
          //Indices inliers, outliers;
          //for (size_t const &indice : indices)
          //{
            //Scalar distance(planePtr->distance(*pointCloudConstPtr[indice]));
            //Scalar angle(planePtr->angle(*normalCloudConstPtr[indice]));

            //if (distance < parameter.distance_ && angle < parameter.angle_)
              //inliers.push_back(indice);
            //else outliers.push(indice);
          //}
          //if (inliers.size() < parameter.cardinal_)
            //continue;

          //// Shape computation using inliers.
          //typedef typename potasse::shape::Plane<Scalar>::Vector Vector;

          //Vector centroid(Vector::Zero());
          //for (size_t const &inlier : inliers)
            //centroid += *pointCloudConstPtr[inlier].point();
          //centroid /= inliers.size();

          //typedef Eigen::Matrix<Scalar, this->Size, this->Size> Matrix;
          //Matrix matrix(Matrix::Zero());
          //for (size_t const &inlier : inliers)
          //{
            //Vector const point(*pointCloudConstPtr[inlier].point() - centroid);
            //matrix += point * point.transpose();
          //}
          //planePtr = std::make_shared<potasse::shape::Plane<Scalar>>(centroid, Eigen::SelfAdjointEigenSolver<Matrix>(matrix).eigenvectors().col(0));

          //// Output update
          //if (!static_cast<bool>(output))
          //{
            //output = std::make_shared<Detected>(planePtr, inliers, outliers);
            //output->error(this->error(*output));
          //}
         
        //}

        //return output;
      //}
  }
}
