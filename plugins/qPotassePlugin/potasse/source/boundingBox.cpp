#include <potasse/cloud/boundingBox.hpp>
#include <potasse/cloud/cloud.hpp>
#include <potasse/cloud/normal.hpp>
#include <potasse/cloud/vertex.hpp>

namespace potasse
{
  namespace cloud
  {
    template class BoundingBox<VertexCloud3>;
    template class BoundingBox<VertexCloud2>;
  }
}
