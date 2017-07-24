#ifndef _POTASSE_ESTIMATOR_MOSTORTHOGONALVECTORTONVECTORS_HPP_
#define _POTASSE_ESTIMATOR_MOSTORTHOGONALVECTORTONVECTORS_HPP_

#include <list>

namespace potasse
{
  namespace estimator
  {
    template <typename NormalCloud>
    class MostOrthogonalVectorToNVectors
    {
     public:
      typedef typename NormalCloud::value_type Normal;
      typedef std::list<std::size_t> Indices;

      static Normal compute(NormalCloud const &normalCloud,
                            Indices const &indices);
    };
  }
}

#endif  //_POTASSE_ESTIMATOR_MOSTORTHOGONALVECTORTONVECTORS_HPP_
