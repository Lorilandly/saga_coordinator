#include "Listener.h"

// hashmap


void listener(int port) {
  using namespace httplib;

  Server svr;
  std::unordered_map<std::string, int> transactions;

  svr.Post("/transactions", [&](const Request& req, Response& res) {
    // create a transaction, store to hashmap
    std::string transaction_id = "hello";
    int transaction = 1;
    transactions[transaction_id] = transaction;
    res.set_content("something", "application/json");
  });

  svr.Get("/transactions/:id", [&](const Request& req, Response& res) {
    std::string transaction_id  = req.path_params.at("id");
    if (auto transaction = transactions.find(transaction_id); transaction != transactions.end()) {
      // get transaction status
      res.set_content("{'status':'Pending'}", "application/json");
    } else {
      res.status = StatusCode::NotFound_404;
    }
  });

  svr.Get("/stop", [&](const Request& req, Response& res) {
    svr.stop();
  });

  svr.listen("localhost", port);
}
