#ifndef _SHAPEDETECTOR_SHAPEDETECTOR_HPP_
#define _SHAPEDETECTOR_SHAPEDETECTOR_HPP_

#include <potasse/cloud/cloud.hpp>
#include <potasse/cloud/normal.hpp>
#include <potasse/cloud/vertex.hpp>

#include <potasse/shape/shape.hpp>

#include <memory>

namespace potasse
{
  namespace shapeDetector
  {
    template <std::size_t S>
      class ShapeDetector
      {
        public:
          typedef cloud::Vertex<S> Vertex;
          typedef cloud::Normal<S> Normal;

          typedef cloud::Cloud<Vertex> VertexCloud;
          typedef cloud::Cloud<Normal> NormalCloud;

          typedef typename Vertex::size_type Size;

          typedef typename VertexCloud::Indices Indices;

          typedef shape::Shape<S> Shape;
          typedef std::shared_ptr<Shape> ShapePtr;

          struct Parameter
          {
            SCALAR distance_;
            SCALAR angle_;
            Size tries_;
            Size cardinal_;
            explicit operator bool() const;
          } parameter_;

          explicit ShapeDetector(Parameter const &parameter);

          struct DetectedShape
          {
            ShapePtr shapePtr_;
            Indices inliers_;
            Indices outliers_;
          };

          DetectedShape detect(VertexCloud const &vertexCloud,
              NormalCloud const &normalCloud,
              Indices const &indices) const;

        protected:
          virtual Size minimum() const = 0;
          virtual ShapePtr shapePtr(VertexCloud const &vertexCloud,
              NormalCloud const &normalCloud,
              Indices const &inliers) const = 0;

        private:
      };
  }
}

#include "impl/shapeDetector.hpp"

#endif  //_SHAPEDETECTOR_SHAPEDETECTOR_HPP_
