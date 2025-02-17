#ifndef UTILITIES_UTILITIESPLOT_I
#define UTILITIES_UTILITIESPLOT_I

#ifdef SWIGPYTHON
%module openstudioutilitiesplot
#endif


%include <utilities/UtilitiesAPI.hpp>
%include <utilities/core/CommonInclude.i>
%import <utilities/core/CommonImport.i>
%import <utilities/UtilitiesData.i>

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
%include <utilities/plot/ProgressBar.i>

#endif // UTILITIES_UTILITIESPLOT_I
