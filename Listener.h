#include <thread>
#include <httplib.h>
#include <unordered_map>
#include <nlohmann/json.hpp>
#include "Transaction.h"

using json = nlohmann::json;

void listener(int port);
