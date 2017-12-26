#include "search_alterning.hh"
#include "statistics.hh"
#include "objectiveFunction.hh"


void Search_alterning::local_search(Batch &app_manager, Configuration &configuration, MYSQL *conn,  OPT_JR_parameters &par)
{
  std::string debugMsg;
  Candidates sCandidateApproximated ;
  double TotalFO;
  int how_many;
  Statistics statistics;
  Objective_fun OF;

  debugMsg =  "\n     ***** Estimate the candidates for the predictor ******\n"; par.debug_message(debugMsg);

  //START THE ITERATION LOOP
  for (int iteration = 1; iteration <= par.get_max_iterations(); iteration++)
  {
    check_total_nodes(par.get_number(), app_manager);
    debugMsg= "ITERATION " + std::to_string(iteration); par.debug_message(debugMsg);


    /*
    *   Estimate the candidates for the predictor
    */
    sCandidateApproximated = approximated_loop( app_manager, par );
    debugMsg= "\n\n\n\n       finished approximatedLoop   \n\n\n\n"; par.debug_message(debugMsg);

    if (sCandidateApproximated.get_empty())
    {
      // The Candidate Application is empty. No further solution enhancements possible
      debugMsg = "LocalSearch: empty Candidate Application "; par.debug_message(debugMsg);
      break;
    }

    debugMsg = " \n\n\n\n\n*****  Ex-iteration loop ******\n\n\n\n\n";par.debug_message(debugMsg);


    debugMsg = "\n\n*******************************************************************\n";
    debugMsg +="*** Consider the first MAX_PROMISING_CONFIGURATIONS of the Application ****\n";
    debugMsg +="*******************************************************************\n"; par.debug_message(debugMsg);

    how_many=sCandidateApproximated.get_size();
    debugMsg = " There are " + std::to_string(how_many) +" promising configurations in iteration " + std::to_string(iteration) + "\n\n"; par.debug_message(debugMsg);



    //tmp=app_manager;

    int index_pair;
    exact_loop(sCandidateApproximated, configuration, conn, app_manager, par, index_pair);


    check_total_nodes(par.get_number(), app_manager);


    if (par.get_global_FO_calculation())
    {
      TotalFO = OF.global(configuration, conn, app_manager, par);
      std::cout << "\n At iteration: "<< iteration << " GLOBAL OBJECTIVE FUNCTION = "<< TotalFO <<"\n"<<std::endl;
      statistics.add_statistics( iteration, how_many, TotalFO); // Update Statistics
    }

    if (index_pair==-1)
    {
      break; //It means that even if there are promising configuration they are not convenient
    }

  }



  if (par.get_global_FO_calculation())
  {
    statistics.read_statistics( par);
  }

}
