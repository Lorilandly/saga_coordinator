#include "Listener.h"

// hashmap

void listener(int port)
{
  using namespace httplib;

  Server svr;
  std::unordered_map<std::string, Transaction> transactions;

  svr.Post("/transactions", [&](const Request &req, Response &res)
           {
    // create a transaction, store to hashmap
    std::string body = req.body;
    Transaction transaction(body);
    std::string id = transaction.get_id();
    transactions[id] = std::move(transaction);

    // Execute transaction in a separate thread
    thread t([&transactions, id]() { transactions[id].exec(); });
    t.detach();

    json j;
    j["transaction_id"] = id;
    res.set_content(to_string(j), "application/json"); });

  svr.Get("/transactions/:id", [&](const Request &req, Response &res)
          {
    std::string transaction_id  = req.path_params.at("id");
    if (auto transaction = transactions.find(transaction_id); transaction != transactions.end()) {
      // get transaction status
      json j;
      j["status"] = ToString(transaction->second.get_status());
      res.set_content(to_string(j), "application/json");
    } else {
      res.status = StatusCode::NotFound_404;
    } });

  svr.Get("/stop", [&](const Request &req, Response &res)
          { svr.stop(); });

  svr.listen("localhost", port);
}
