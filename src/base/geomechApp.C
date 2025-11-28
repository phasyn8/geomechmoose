#include "geomechApp.h"
#include "Moose.h"
#include "AppFactory.h"
#include "ModulesApp.h"
#include "MooseSyntax.h"

InputParameters
geomechApp::validParams()
{
  InputParameters params = MooseApp::validParams();
  params.set<bool>("use_legacy_material_output") = false;
  params.set<bool>("use_legacy_initial_residual_evaluation_behavior") = false;
  return params;
}

geomechApp::geomechApp(const InputParameters & parameters) : MooseApp(parameters)
{
  geomechApp::registerAll(_factory, _action_factory, _syntax);
}

geomechApp::~geomechApp() {}

void
geomechApp::registerAll(Factory & f, ActionFactory & af, Syntax & syntax)
{
  ModulesApp::registerAllObjects<geomechApp>(f, af, syntax);
  Registry::registerObjectsTo(f, {"geomechApp"});
  Registry::registerActionsTo(af, {"geomechApp"});

  /* register custom execute flags, action syntax, etc. here */
}

void
geomechApp::registerApps()
{
  registerApp(geomechApp);
}

/***************************************************************************************************
 *********************** Dynamic Library Entry Points - DO NOT MODIFY ******************************
 **************************************************************************************************/
extern "C" void
geomechApp__registerAll(Factory & f, ActionFactory & af, Syntax & s)
{
  geomechApp::registerAll(f, af, s);
}
extern "C" void
geomechApp__registerApps()
{
  geomechApp::registerApps();
}
