#ifndef UTILITIES_UTILITIESUNITS_I
#define UTILITIES_UTILITIESUNITS_I

#ifdef SWIGPYTHON
%module openstudioutilitiesunits
#endif


%include <utilities/UtilitiesAPI.hpp>
%include <utilities/core/CommonInclude.i>
%import <utilities/core/CommonImport.i>
%import <utilities/UtilitiesCore.i>

//Ignore common stream related functions
//that will not be supported by SWIG


%ignore *::toText(std::ostream&) const;
%ignore *::fromText(std::istream&);
%ignore *::toXml(std::ostream&) const;
%ignore *::fromXml(std::istream&);
%ignore *::print(std::ostream&) const;
%ignore *::fromXmlStream(std::istream&);
%ignore *::toXmlStream(std::ostream&) const;

// Unit conflicts with test/unit in Ruby, rename "globally"
%rename(ZUnit) openstudio::Unit;


%{
  namespace openstudio{}
  using namespace openstudio;

  #include <utilities/idf/IdfObject.hpp>
  #include <utilities/idf/WorkspaceObject.hpp>

%}


// order that these are loaded matters, e.g TimeSeries in data needs to know about DateTime in time
%include <utilities/units/Scale.i>
%include <utilities/units/ScaleFactory.i>
%include <utilities/units/Unit.i>
%include <utilities/units/QuantityRegex.i>
%include <utilities/units/UnitFactory.i>
%include <utilities/units/Quantity.i>
%include <utilities/units/QuantityFactory.i>
%include <utilities/units/QuantityConverter.i>

#endif // UTILITIES_UTILITIESUNITS_I
