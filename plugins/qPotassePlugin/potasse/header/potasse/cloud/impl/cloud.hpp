namespace potasse
{
  namespace cloud
  {
    template<typename T>
      void Cloud<T>::read(std::istream &stream)
      {
        T t;
        while (stream >> t)
          this->push_back(t);
      }

    template<typename T>
      void Cloud<T>::write(std::ostream &stream) const
      {
        for (auto const &t : *this)
          stream << t << '\n';
      }

    template<typename T>
      std::istream &operator>>(std::istream &stream, potasse::cloud::Cloud<T> &cloud)
      {
        cloud.read(stream);
        return stream;
      }

    template<typename T>
      std::ostream &operator<<(std::ostream &stream, potasse::cloud::Cloud<T> const &cloud)
      {
        cloud.write(stream);
        return stream;
      }
  }
}
