#ifndef __CHARACTERSET_ECI__
#define __CHARACTERSET_ECI__

#include <map>
#include <DecodeHints.h>

namespace qcv {
namespace common {

class CharacterSetECI : public Counted {
private:
  static std::map<int, qcv::Ref<CharacterSetECI> > VALUE_TO_ECI;
  static std::map<std::string, qcv::Ref<CharacterSetECI> > NAME_TO_ECI;
  static const bool inited;
  static bool init_tables();

  int const* const values_;
  char const* const* const names_;

  CharacterSetECI(int const* values, char const* const* names);

  static void addCharacterSet(int const* value, char const* const* encodingNames);

public:
  char const* name() const;
  int getValue() const;

  static CharacterSetECI* getCharacterSetECIByValue(int value);
  static CharacterSetECI* getCharacterSetECIByName(std::string const& name);
};

}
}

#endif
