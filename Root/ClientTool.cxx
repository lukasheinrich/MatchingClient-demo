#include "MatchingClient/ClientTool.h"

#include "TriggerMatchingTool/MakerFuncs.h"

ClientTool::ClientTool(const std::string& name) : asg::AsgTool(name){
}

StatusCode ClientTool::initialize(){
        ATH_MSG_INFO("initialize client");

return StatusCode::SUCCESS;
}

bool ClientTool::match(IExpertMatchingTool& imt){
  ATH_MSG_INFO("Matching with imt");
  
  const xAOD::ElectronContainer* recElectrons = 0;
  evtStore()->retrieve(recElectrons, "Electrons");

  const xAOD::MuonContainer* recMuons = 0;
  evtStore()->retrieve(recMuons, "Muons");
  
  if(!recElectrons || !recMuons) return false;


  ATH_MSG_INFO("Event has " << recElectrons->size() << " reco Electrons and " << recMuons->size() <<  " reco Muons");

  
  
  if(recElectrons->size() > 0){
    ATH_MSG_INFO("matching electrons" );

    bool result = imt.match(recElectrons->at(0),"HLT_e20_vloose");
    ATH_MSG_INFO("match electron chain: " << result);
  }
  
  if(recMuons->size() > 2){
    ATH_MSG_INFO("matching muons" );

    auto lead_mu = recMuons->at(0);
    auto sub_mu = recMuons->at(1);
    auto subsub_mu = recMuons->at(2);
    
    bool result1 = imt.match({lead_mu,sub_mu},"HLT_2mu10");
    ATH_MSG_INFO("match di-muon chain (leading): "           <<  result1);
    
    bool result2 = imt.match({sub_mu,lead_mu},"HLT_2mu10"); 
    ATH_MSG_INFO("match di-muon chain (leading, inverted): " << result2);
    
    bool result3 = imt.match({sub_mu,subsub_mu},"HLT_2mu10");
    ATH_MSG_INFO("match di-muon chain (sub, subsub): "       <<  result3);

  }
  
  if((recMuons->size() > 0) && (recElectrons->size() > 0) ){
    ATH_MSG_INFO("matching muons and electrons" );
    
    auto lead_el = recElectrons->at(0);
    auto lead_mu = recMuons->at(0);
    
    bool matchresult = imt.match(make_reco({lead_el},{lead_mu}),"HLT_e17_loose_mu14");
    
    ATH_MSG_INFO("match mixed chain: " << matchresult );
    
    auto custom_el = [](const xAOD::Electron& lhs, const xAOD::TrigElectron& rhs){
      std::cout << "=================> custom el metric" << std::endl;
      return lhs.p4().DeltaR(rhs.p4());
    };
    
    auto custom_mu = [](const xAOD::Muon& lhs, const xAOD::L2CombinedMuon& rhs){
      std::cout << "=================> custom mu metric" << std::endl;
      return lhs.p4().DeltaR(rhs.p4());
    };
    

    //note that in these calls, the objects in the brace expression {...} are of *different* type and it still works.

    //we can override with previously defined functions    
    bool matchresult_pop = imt.match({lead_el,lead_mu},"HLT_e17_loose_mu14",{{custom_el,0.3},{custom_mu,0.2}});
    ATH_MSG_INFO("match mixed chain (both metrices): " << matchresult_pop );
    
    
    //order in {...} does not matter
    bool matchresult_only_one = imt.match({lead_mu,lead_el},"HLT_e17_loose_mu14",{{custom_el,0.3}});
    ATH_MSG_INFO("match mixed chain (one metric): " << matchresult_only_one );
    
    //define everything in one call
    bool matchresult_incall= imt.match({lead_el,lead_mu},"HLT_e17_loose_mu14",
				       {{[](const xAOD::Muon& lhs, const xAOD::L2CombinedMuon& rhs){
					     auto reladeltapt = fabs(lhs.pt()-rhs.pt())/rhs.pt();
					     auto deltar = lhs.p4().DeltaR(rhs.p4());
					     auto combined = sqrt(pow(deltar,2)+pow(reladeltapt,2));
					     return combined;
					   },0.3}});
    
    ATH_MSG_INFO("match mixed chain (one metric): " << matchresult_incall );
  }
  return true;
}

