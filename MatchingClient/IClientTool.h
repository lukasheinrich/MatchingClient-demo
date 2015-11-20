// Dear emacs, this is -*- c++ -*-
#ifndef ICLIENTTOOL_H
#define ICLIENTTOOL_H


// Framework include(s):
#include "AsgTools/IAsgTool.h"

#include "TriggerMatchingTool/IExpertMatchingTool.h"

class IClientTool : virtual public asg::IAsgTool {
  ASG_TOOL_INTERFACE(IClientTool)
public:
  virtual bool match(IExpertMatchingTool& matchtool) = 0;
};

#endif
