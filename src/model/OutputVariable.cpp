/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "OutputVariable.hpp"
#include "OutputVariable_Impl.hpp"
#include "Schedule.hpp"
#include "Schedule_Impl.hpp"
#include "EnergyManagementSystemSensor.hpp"
#include "EnergyManagementSystemSensor_Impl.hpp"

#include "Model.hpp"

#include <utilities/idd/IddFactory.hxx>

#include <utilities/idd/OS_Output_Variable_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>

#include "../utilities/sql/SqlFileEnums.hpp"
#include "../utilities/sql/SqlFile.hpp"

#include "../utilities/core/Assert.hpp"

using openstudio::Handle;
using openstudio::OptionalHandle;
using openstudio::HandleVector;
using openstudio::IdfObject;
using openstudio::WorkspaceObject;
using openstudio::OptionalWorkspaceObject;
using openstudio::WorkspaceObjectVector;
using openstudio::Workspace;

using std::string;

namespace openstudio {
namespace model {

  namespace detail {

    OutputVariable_Impl::OutputVariable_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle)
      : ModelObject_Impl(idfObject, model, keepHandle) {
      OS_ASSERT(idfObject.iddObject().type() == OutputVariable::iddObjectType());
    }

    OutputVariable_Impl::OutputVariable_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle)
      : ModelObject_Impl(other, model, keepHandle) {
      OS_ASSERT(other.iddObject().type() == OutputVariable::iddObjectType());
    }

    OutputVariable_Impl::OutputVariable_Impl(const OutputVariable_Impl& other, Model_Impl* model, bool keepHandle)
      : ModelObject_Impl(other, model, keepHandle) {}

    const std::vector<std::string>& OutputVariable_Impl::outputVariableNames() const {
      static const std::vector<std::string> result;
      return result;
    }

    IddObjectType OutputVariable_Impl::iddObjectType() const {
      return OutputVariable::iddObjectType();
    }

    std::vector<openstudio::IdfObject> OutputVariable_Impl::remove() {
      //Note: Cant do /object-list implementation for EMS Sensor since Auto Naming of Objects causes issues.
      //      Instead, doing an /alpha getString implementation so we need to manually remove any referring Sensors
      const Model m = this->model();

      std::vector<EnergyManagementSystemSensor> objects = m.getConcreteModelObjects<EnergyManagementSystemSensor>();
      for (auto& sensor : objects) {
        if (sensor.outputVariable()) {
          //if (sensor.outputVariable().get().variableName() == this->variableName()) {
          //  sensor.remove();
          //}
          // check handle instead of name
          if (sensor.outputVariable().get().handle() == this->handle()) {
            sensor.remove();
          }
        }
      }

      return ModelObject_Impl::remove();
    }

    std::vector<ScheduleTypeKey> OutputVariable_Impl::getScheduleTypeKeys(const Schedule& schedule) const {
      std::vector<ScheduleTypeKey> result;
      UnsignedVector fieldIndices = getSourceIndices(schedule.handle());
      UnsignedVector::const_iterator b(fieldIndices.begin());
      UnsignedVector::const_iterator e(fieldIndices.end());
      if (std::find(b, e, OS_Output_VariableFields::ScheduleName) != e) {
        result.push_back(ScheduleTypeKey("OutputVariable", "Active Hours"));
      }
      return result;
    }

    std::string OutputVariable_Impl::keyValue() const {
      OptionalString result = getString(OS_Output_VariableFields::KeyValue, true);
      OS_ASSERT(result);
      return result.get();
    }

    bool OutputVariable_Impl::isKeyValueDefaulted() const {
      return isEmpty(OS_Output_VariableFields::KeyValue);
    }

    std::string OutputVariable_Impl::variableName() const {
      OptionalString result = getString(OS_Output_VariableFields::VariableName, true);
      OS_ASSERT(result);
      return result.get();
    }

    std::string OutputVariable_Impl::reportingFrequency() const {
      OptionalString result = getString(OS_Output_VariableFields::ReportingFrequency, true);
      OS_ASSERT(result);
      return result.get();
    }

    bool OutputVariable_Impl::isReportingFrequencyDefaulted() const {
      return isEmpty(OS_Output_VariableFields::ReportingFrequency);
    }

    bool OutputVariable_Impl::setKeyValue(const std::string& keyValue) {
      return setString(OS_Output_VariableFields::KeyValue, keyValue);
    }

    void OutputVariable_Impl::resetKeyValue() {
      bool test = setString(OS_Output_VariableFields::KeyValue, "");
      OS_ASSERT(test);
    }

    bool OutputVariable_Impl::setVariableName(const std::string& variableName) {
      return setString(OS_Output_VariableFields::VariableName, variableName);
    }

    bool OutputVariable_Impl::setReportingFrequency(const std::string& reportingFrequency) {
      return setString(OS_Output_VariableFields::ReportingFrequency, reportingFrequency);
    }

    void OutputVariable_Impl::resetReportingFrequency() {
      bool test = setString(OS_Output_VariableFields::ReportingFrequency, "");
      OS_ASSERT(test);
    }

    boost::optional<Schedule> OutputVariable_Impl::schedule() const {
      return getObject<ModelObject>().getModelObjectTarget<Schedule>(OS_Output_VariableFields::ScheduleName);
    }

    bool OutputVariable_Impl::setSchedule(Schedule& schedule) {
      bool result = ModelObject_Impl::setSchedule(OS_Output_VariableFields::ScheduleName, "OutputVariable", "Active Hours", schedule);
      return result;
    }

    void OutputVariable_Impl::resetSchedule() {
      this->setString(OS_Output_VariableFields::ScheduleName, "");
    }

    boost::optional<ModelObject> OutputVariable_Impl::scheduleAsModelObject() const {
      OptionalModelObject result;
      OptionalSchedule intermediate = schedule();
      if (intermediate) {
        result = *intermediate;
      }
      return result;
    }

    bool OutputVariable_Impl::setScheduleAsModelObject(const boost::optional<ModelObject>& modelObject) {
      if (modelObject) {
        OptionalSchedule intermediate = modelObject->optionalCast<Schedule>();
        if (intermediate) {
          Schedule schedule(*intermediate);
          return setSchedule(schedule);
        } else {
          return false;
        }
      } else {
        resetSchedule();
      }
      return true;
    }

    std::vector<std::string> OutputVariable_Impl::reportingFrequencyValues() const {
      return OutputVariable::reportingFrequencyValues();
    }

    bool OutputVariable_Impl::exportToBCVTB() const {
      boost::optional<std::string> value = getString(OS_Output_VariableFields::ExportToBCVTB, true);
      OS_ASSERT(value);
      return openstudio::istringEqual(value.get(), "True");
    }

    bool OutputVariable_Impl::isExportToBCVTBDefaulted() const {
      return isEmpty(OS_Output_VariableFields::ExportToBCVTB);
    }

    bool OutputVariable_Impl::setExportToBCVTB(bool exportToBCVTB) {
      bool result = false;
      if (exportToBCVTB) {
        result = setString(OS_Output_VariableFields::ExportToBCVTB, "True");
      } else {
        result = setString(OS_Output_VariableFields::ExportToBCVTB, "False");
      }
      OS_ASSERT(result);
      return result;
    }

    void OutputVariable_Impl::resetExportToBCVTB() {
      bool result = setString(OS_Output_VariableFields::ExportToBCVTB, "");
      OS_ASSERT(result);
    }

  }  // namespace detail

  OutputVariable::OutputVariable(const std::string& variableName, const Model& model) : ModelObject(OutputVariable::iddObjectType(), model) {
    OS_ASSERT(getImpl<detail::OutputVariable_Impl>());
    bool test = getImpl<detail::OutputVariable_Impl>()->setVariableName(variableName);
    OS_ASSERT(test);
  }

  OutputVariable::OutputVariable(std::shared_ptr<detail::OutputVariable_Impl> impl) : ModelObject(std::move(impl)) {}

  IddObjectType OutputVariable::iddObjectType() {
    IddObjectType result(IddObjectType::OS_Output_Variable);
    return result;
  }

  std::vector<std::string> OutputVariable::reportingFrequencyValues() {
    std::vector<std::string> retvals;

    std::map<int, std::string> names = openstudio::ReportingFrequency::getNames();

    for (auto itr = names.begin(); itr != names.end(); ++itr) {
      retvals.push_back(itr->second);
    }

    return retvals;
  }

  std::string OutputVariable::keyValue() const {
    return getImpl<detail::OutputVariable_Impl>()->keyValue();
  }

  bool OutputVariable::isKeyValueDefaulted() const {
    return getImpl<detail::OutputVariable_Impl>()->isKeyValueDefaulted();
  }

  std::string OutputVariable::variableName() const {
    return getImpl<detail::OutputVariable_Impl>()->variableName();
  }

  std::string OutputVariable::reportingFrequency() const {
    return getImpl<detail::OutputVariable_Impl>()->reportingFrequency();
  }

  bool OutputVariable::isReportingFrequencyDefaulted() const {
    return getImpl<detail::OutputVariable_Impl>()->isReportingFrequencyDefaulted();
  }

  bool OutputVariable::setKeyValue(const std::string& keyValue) {
    return getImpl<detail::OutputVariable_Impl>()->setKeyValue(keyValue);
  }

  void OutputVariable::resetKeyValue() {
    getImpl<detail::OutputVariable_Impl>()->resetKeyValue();
  }

  bool OutputVariable::setVariableName(const std::string& variableName) {
    return getImpl<detail::OutputVariable_Impl>()->setVariableName(variableName);
  }

  bool OutputVariable::setReportingFrequency(const std::string& reportingFrequency) {
    return getImpl<detail::OutputVariable_Impl>()->setReportingFrequency(reportingFrequency);
  }

  void OutputVariable::resetReportingFrequency() {
    getImpl<detail::OutputVariable_Impl>()->resetReportingFrequency();
  }

  boost::optional<Schedule> OutputVariable::schedule() const {
    return getImpl<detail::OutputVariable_Impl>()->schedule();
  }

  bool OutputVariable::setSchedule(Schedule& schedule) {
    return getImpl<detail::OutputVariable_Impl>()->setSchedule(schedule);
  }

  void OutputVariable::resetSchedule() {
    getImpl<detail::OutputVariable_Impl>()->resetSchedule();
  }

  std::vector<IdfObject> OutputVariable::remove() {
    return getImpl<detail::OutputVariable_Impl>()->remove();
  }

  bool OutputVariable::exportToBCVTB() const {
    return getImpl<detail::OutputVariable_Impl>()->exportToBCVTB();
  }

  bool OutputVariable::isExportToBCVTBDefaulted() const {
    return getImpl<detail::OutputVariable_Impl>()->isExportToBCVTBDefaulted();
  }

  bool OutputVariable::setExportToBCVTB(bool exportToBCVTB) {
    return getImpl<detail::OutputVariable_Impl>()->setExportToBCVTB(exportToBCVTB);
  }

  void OutputVariable::resetExportToBCVTB() {
    getImpl<detail::OutputVariable_Impl>()->resetExportToBCVTB();
  }

}  // namespace model
}  // namespace openstudio
