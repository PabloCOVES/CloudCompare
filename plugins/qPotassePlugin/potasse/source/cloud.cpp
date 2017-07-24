#include <potasse/cloud/cloud.hpp>

#include <potasse/cloud/vertex.hpp>
#include <potasse/cloud/normal.hpp>

namespace potasse
{
  namespace cloud
  {
    template class Cloud<Vertex2>;
    template class Cloud<Vertex3>;
  }
}
