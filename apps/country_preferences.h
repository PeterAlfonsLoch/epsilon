#ifndef COUNTRY_PREFERENCES_H
#define COUNTRY_PREFERENCES_H

#include <poincare/preferences.h>

class CountryPreferences {
public:
  enum class AvailableExamModes : uint8_t {
    StandardOnly,
    All
  };

  enum class MethodForQuartiles : uint8_t {
    MedianOfSublist,
    CumulatedFrequency
  };

  enum class HomeAppsLayout : uint8_t {
    Default,
    Variant1,
    Variant2,
    Variant3
  };

  enum class DiscriminantSymbol : uint8_t {
    Delta,
    D
  };

  constexpr CountryPreferences(AvailableExamModes availableExamModes, MethodForQuartiles methodForQuartiles, Poincare::Preferences::UnitFormat unitFormat, HomeAppsLayout homeAppsLayout, DiscriminantSymbol discriminantSymbol) :
    m_availableExamModes(availableExamModes),
    m_methodForQuartiles(methodForQuartiles),
    m_unitFormat(unitFormat),
    m_homeAppsLayout(homeAppsLayout),
    m_discriminantSymbol(discriminantSymbol)
  {}

  constexpr AvailableExamModes availableExamModes() const { return m_availableExamModes; }
  constexpr MethodForQuartiles methodForQuartiles() const { return m_methodForQuartiles; }
  constexpr Poincare::Preferences::UnitFormat unitFormat() const { return m_unitFormat; }
  constexpr HomeAppsLayout homeAppsLayout() const { return m_homeAppsLayout; }
  constexpr const char * discriminantSymbol() const { return m_discriminantSymbol == DiscriminantSymbol::Delta ? "Δ" : "D"; }

private:
  const AvailableExamModes m_availableExamModes;
  const MethodForQuartiles m_methodForQuartiles;
  const Poincare::Preferences::UnitFormat m_unitFormat;
  const HomeAppsLayout m_homeAppsLayout;
  const DiscriminantSymbol m_discriminantSymbol;
};

#endif
