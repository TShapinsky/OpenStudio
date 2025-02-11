/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MEASURE_OSMEASUREINFOGETTER_HPP
#define MEASURE_OSMEASUREINFOGETTER_HPP

#include "MeasureAPI.hpp"

#include "OSArgument.hpp"
#include "OSOutput.hpp"

#include "../utilities/core/Optional.hpp"
#include "../utilities/bcl/BCLMeasure.hpp"

#include <vector>
#include <string>

namespace openstudio {

class Workspace;

namespace model {
  class Model;
}

namespace measure {

  /** OSMeasureInfo contains information that can be extracted from an OSMeasure by a parser.
 */
  class MEASURE_API OSMeasureInfo
  {
   public:
    /// Constructor used when Ruby interpreter fails to get information about the script
    OSMeasureInfo(const std::string& error);

    /// Constructor used when Ruby interpreter succeeds to get information about the script
    OSMeasureInfo(const MeasureType& measureType, const std::string& className, const std::string& name, const std::string& description,
                  const std::string& taxonomy, const std::string& modelerDescription, const std::vector<OSArgument>& arguments,
                  const std::vector<OSOutput>& outputs);

    /// will be empty if information was successfully extracted from the script, otherwise includes error information
    boost::optional<std::string> error() const;

    /// type of measure if information was successfully extracted from the script, otherwise undefined
    MeasureType measureType() const;

    /// name of measure class if information was successfully extracted from the script, otherwise empty
    std::string className() const;

    /// result of measure's name method if information was successfully extracted from the script, otherwise empty
    std::string name() const;

    /// result of measure's description method if information was successfully extracted from the script, otherwise empty
    std::string description() const;

    /// result of measure's taxonomy method if information was successfully extracted from the script, otherwise empty
    std::string taxonomy() const;

    /// result of measure's modeler_description method if information was successfully extracted from the script, otherwise empty
    std::string modelerDescription() const;

    /// result of measure's arguments method called with empty model if information was successfully extracted from the script, otherwise empty
    std::vector<OSArgument> arguments() const;

    std::vector<BCLMeasureArgument> bclMeasureArguments() const;

    /// result of measure's outputs method called with empty model if information was successfully extracted from the script, otherwise empty
    std::vector<OSOutput> outputs() const;

    /// Compares the measure's information from the xml with information extracted from the ruby measure
    /// Updates the measure if needed, returns true if an update was performed
    bool update(BCLMeasure& measure) const;

   private:
    REGISTER_LOGGER("openstudio.measure.OSMeasureInfo");

    boost::optional<std::string> m_error;
    MeasureType m_measureType;
    std::string m_className;
    std::string m_name;
    std::string m_description;
    std::string m_taxonomy;
    std::string m_modelerDescription;
    std::vector<OSArgument> m_arguments;
    std::vector<OSOutput> m_outputs;
  };

  /** Interface class for using embedded Ruby to extract arguments from an OSMeasure.
 *  See src/measure/test/EmbeddedRuby_GTest.cpp for this use case. If your application is written in
 *  Ruby, this functionality is available through non-member helper function
 *  Opudio::Measure::infoExtractorRubyFunction(BCLMeasure), see
 *  ruby/openstudio/measure/test/OSMeasureInfoGetter_Test.rb for an example. */
  class MEASURE_API OSMeasureInfoGetter
  {
   public:
    virtual ~OSMeasureInfoGetter() = default;

    virtual OSMeasureInfo getInfo(const BCLMeasure& measure) = 0;

    /** Use this method to pass in a (EnergyPlus) Workspace or a (OpenStudio) Model. */
    virtual OSMeasureInfo getInfo(const BCLMeasure& measure, const Workspace& workspace) = 0;

    virtual OSMeasureInfo getInfo(const BCLMeasure& measure, const model::Model& model, const Workspace& workspace) = 0;

    OSMeasureInfo getInfo(const BCLMeasure& measure, const boost::optional<model::Model>& model, const boost::optional<Workspace>& workspace);
  };

  /** Non-member function that returns Ruby code text for retrieving info from a BCLMeasure. This
 *  function is used by both the embedded Ruby OSMeasureInfoGetter and native Ruby
 *  (OpenStudio::Measure::getInfo) implementations of the functionality, such that either usage
 *  pathway should produce identical results. \relates OSMeasureInfoGetter */
  MEASURE_API std::string infoExtractorRubyFunction();

  /** Interface class for rendering the ruby readme via ERB, parent class for the RubyMeasureInfoBinding
   *  declared in ruby/bindings/InitRubyBindings.cpp so we can use it from C++ */
  class MEASURE_API MeasureInfoBinding
  {
   public:
    virtual ~MeasureInfoBinding() = default;
    virtual void setMeasureInfo(OSMeasureInfo& info);
    virtual bool renderFile(const std::string& readmeInPath);
  };

}  // namespace measure
}  // namespace openstudio

#endif  // MEASURE_OSMEASUREINFOGETTER_HPP
