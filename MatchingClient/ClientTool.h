// Dear emacs, this is -*- c++ -*-
#ifndef CLIENTTOOL_H
#define CLIENTTOOL_H

// Framework include(s):
#include "AsgTools/AsgTool.h"
#include "AsgTools/ToolHandle.h"

#include "MatchingClient/IClientTool.h"

class ClientTool : public asg::AsgTool, 
                   virtual public IClientTool {
  ASG_TOOL_CLASS(ClientTool,IClientTool)

public:
  ClientTool(const std::string& name);
  StatusCode initialize();
  bool match(IExpertMatchingTool& imt);
};

#endif
