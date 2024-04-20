#include <thread>
#include <mutex>
#include <iostream>
#include <httplib.h>
#include <nlohmann/json.hpp>
#include "Steps.h"
using namespace std;
using json = nlohmann::json;

enum class Status
{
	Pending,
	Processing,
	Success,
	Cancelled
};

inline const char *ToString(Status v)
{
	switch (v)
	{
	case Status::Pending:
		return "Pending";
	case Status::Processing:
		return "Processing";
	case Status::Success:
		return "Success";
	case Status::Cancelled:
		return "Cancelled";
	default:
		return "[Unknown Status]";
	}
}

class Transaction
{
public:
	string id;
	vector<Step> steps;
	int current_step;
	Status status;
	mutex mtx;

	Transaction() = default;
	Transaction(const string data);
	Transaction(Transaction &&other) noexcept;			  // Move constructor
	Transaction &operator=(Transaction &&other) noexcept; // Move assignment operator
	Status get_status();
	string get_id();
	void exec();

private:
	void comp();
	string gen_tid();
	string request(const Action &a);
};
