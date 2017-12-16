#ifndef SEARCH_METHODS_HH
#define SEARCH_METHODS_HH

#include "readConfigurationFile.hh"
#include "batch.hh"
#include "optjrParameters.hh"
#include "candidates.hh"
#include  <mysql.h>

/**
  "Search_methods" class provides methods to find a solution minimizing
   the objective function.

*/

class Search_methods{

public:
  void checkTotalNodes(int N, Batch &App_manager);
  sCandidates approximated_loop( Batch &App_manager, int &iteration, optJrParameters &par);
  void exact_loop(sCandidates &app_pairs, sConfiguration &configuration,  MYSQL *conn,  Batch &App_manager, optJrParameters &par, int &index_pair);



};

#endif
