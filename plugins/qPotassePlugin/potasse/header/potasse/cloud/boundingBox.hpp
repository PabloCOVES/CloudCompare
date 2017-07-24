#ifndef _CLOUD_BOUNDINGBOX_HPP_
#define _CLOUD_BOUNDINGBOX_HPP_

namespace potasse
{
  namespace cloud
  {
    template <typename PC>
      class BoundingBox
      {
        public:
          typedef PC PointCloud;
          typedef typename PointCloud::value_type Point;
          typedef typename PointCloud::Indices Indices;

          explicit BoundingBox(PointCloud const &pointCloud);
          BoundingBox(PointCloud const &pointCloud, Indices const &indices);

          Point center() const;
          Point dimensions() const;

        protected:
        private:
          Point min_;
          Point max_;

      };
  }
}

#include "impl/boundingBox.hpp"

#endif  //_CLOUD_BOUNDINGBOX_HPP_
