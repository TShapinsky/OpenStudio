/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include <gtest/gtest.h>

#include "ModelFixture.hpp"

#include "../ExternalInterfaceFunctionalMockupUnitImportToActuator.hpp"
#include "../ExternalInterfaceFunctionalMockupUnitImportToActuator_Impl.hpp"
#include "../ExternalInterfaceFunctionalMockupUnitImport.hpp"
#include "../ExternalInterfaceFunctionalMockupUnitImport_Impl.hpp"

#include "../Building.hpp"
#include "../Building_Impl.hpp"
#include "../ThermalZone.hpp"
#include "../FanConstantVolume.hpp"
#include "../FanConstantVolume_Impl.hpp"
#include "../ElectricEquipment.hpp"
#include "../ElectricEquipment_Impl.hpp"
#include "../ElectricEquipmentDefinition.hpp"
#include "../ElectricEquipmentDefinition_Impl.hpp"
#include "../Schedule.hpp"
#include "../OutputVariable.hpp"
#include "../OutputVariable_Impl.hpp"
#include "../Model_Impl.hpp"

using namespace openstudio;
using namespace openstudio::model;
using std::string;

TEST_F(ModelFixture, ExternalInterfaceFunctionalMockupUnitImportToActuator) {
  Model model;

  Building building = model.getUniqueModelObject<Building>();

  ThermalZone zone1(model);
  ThermalZone zone2(model);

  // add fan
  Schedule s = model.alwaysOnDiscreteSchedule();
  FanConstantVolume fan(model, s);
  FanConstantVolume fan2(model, s);

  // add FMU
  ExternalInterfaceFunctionalMockupUnitImport eifmui(model, "test name");

  // add actuator
  std::string fanControlType = "Fan Pressure Rise";
  std::string ComponentType = "Fan";
  ExternalInterfaceFunctionalMockupUnitImportToActuator fanActuator(fan, ComponentType, fanControlType, eifmui, "FMU", "Fan FMU name", 10);
  EXPECT_EQ(fanControlType, fanActuator.actuatedComponentControlType());
  EXPECT_EQ(ComponentType, fanActuator.actuatedComponentType());
  EXPECT_EQ("FMU", fanActuator.fMUInstanceName());
  EXPECT_EQ("Fan FMU name", fanActuator.fMUVariableName());
  fanActuator.setFMUInstanceName("test 1");
  fanActuator.setFMUVariableName("test 2");
  EXPECT_EQ("test 1", fanActuator.fMUInstanceName());
  EXPECT_EQ("test 2", fanActuator.fMUVariableName());

  ExternalInterfaceFunctionalMockupUnitImport temp = fanActuator.fMUFile();
  EXPECT_EQ(eifmui, fanActuator.fMUFile());
  EXPECT_EQ(fan, fanActuator.actuatedComponentUnique());

  ExternalInterfaceFunctionalMockupUnitImport eifmui2(model, "test name 2");
  EXPECT_TRUE(fanActuator.setFMUFile(eifmui2));
  EXPECT_EQ(eifmui2, fanActuator.fMUFile());

  std::string fanName = fan.name().get() + "Press Actuator";
  fanActuator.setName(fanName);

  fanActuator.setActuatedComponentControlType(fanControlType);

  EXPECT_EQ(fan, fanActuator.actuatedComponentUnique());
  EXPECT_EQ(fanControlType, fanActuator.actuatedComponentControlType());

  fanActuator.setActuatedComponentType(ComponentType);
  EXPECT_EQ(ComponentType, fanActuator.actuatedComponentType());

  fanActuator.setActuatedComponentUnique(fan2);
  EXPECT_EQ(fan2, fanActuator.actuatedComponentUnique());

  EXPECT_EQ(10, fanActuator.initialValue());
  fanActuator.setInitialValue(1);
  EXPECT_EQ(1.0, fanActuator.initialValue());

  //add electric equipment actuator
  ElectricEquipmentDefinition definition(model);
  ElectricEquipment electricEquipment(definition);
  ComponentType = "ElectricEquipment";
  std::string equipControlType = "Electricity Rate";
  ExternalInterfaceFunctionalMockupUnitImportToActuator equipActuator(electricEquipment, ComponentType, equipControlType, eifmui, "FMU",
                                                                      "electric FMU", 1);
  EXPECT_EQ(equipControlType, equipActuator.actuatedComponentControlType());
  EXPECT_EQ(ComponentType, equipActuator.actuatedComponentType());

  std::string equipName = electricEquipment.name().get() + "power level Actuator";
  equipActuator.setName(equipName);

  equipActuator.setActuatedComponentControlType(equipControlType);

  EXPECT_EQ(electricEquipment, equipActuator.actuatedComponentUnique());
  EXPECT_EQ(equipControlType, equipActuator.actuatedComponentControlType());

  equipActuator.setActuatedComponentType(ComponentType);
  EXPECT_EQ(ComponentType, equipActuator.actuatedComponentType());
}
