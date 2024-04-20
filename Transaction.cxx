#include "Transaction.h"

// Why does it not work unless I put this here?
void from_json(const json &j, Action &a)
{
	string address = j.at("address");
	size_t pos = address.find_first_of('/');
	a.host = address.substr(0, pos);
	a.path = address.substr(pos);
	j.at("method").get_to(a.method);
	a.data = to_string(j.at("data"));
}

void from_json(const json &j, Step &s)
{
	j.at("name").get_to(s.name);
	if (!j.at("execute").is_null())
		j.at("execute").get_to(s.execute);
	if (!j.at("compensate").is_null())
		j.at("compensate").get_to(s.compensate);
}

void from_json(const json &j, Steps &s)
{
	for (const json &elem : j)
	{
		s.push_back(elem.get<Step>());
	}
}

Transaction::Transaction(const string data)
{
	id = gen_tid();
	json j = json::parse(data);
	j.at("steps").get_to(steps);
	current_step = 0;
	status = Status::Pending;
}

void Transaction::exec()
{
	mtx.lock();
	status = Status::Processing;
	mtx.unlock();

	for (auto &step : steps)
	{
		// Execute step
		cout << "Executing step: " << step.name << endl;
		cout << "\tHost: " << step.execute.host << endl;
		cout << "\tPath: " << step.execute.path << endl;
		cout << "\tMethod: " << step.execute.method << endl;
		cout << "\tData: " << step.execute.data << endl;

		try
		{
			auto resp = request(step.execute);
		}
		catch (const httplib::Error &e)
		{
			cerr << "Error occurred: " << e << endl;
			current_step -= 1;
			comp();
			return;
		}

		current_step++;
	}

	mtx.lock();
	status = Status::Success;
	mtx.unlock();
}

void Transaction::comp()
{
	for (int i = current_step - 1; i >= 0; i--)
	{
		auto &step = steps[i];
		// Compensate step
		cout << "Compensating step: " << step.name << endl;
		cout << "\tHost: " << step.compensate.host << endl;
		cout << "\tPath: " << step.compensate.path << endl;
		cout << "\tMethod: " << step.compensate.method << endl;
		cout << "\tData: " << step.compensate.data << endl;

		try
		{
			request(step.compensate);
		}
		catch (const httplib::Error &e)
		{
			cerr << "Error occurred during compensation: " << e << endl;
		}
	}

	mtx.lock();
	status = Status::Cancelled;
	mtx.unlock();
}

Status Transaction::get_status()
{
	lock_guard<mutex> lock(mtx);
	return status;
}

string Transaction::get_id()
{
	return id;
}

string Transaction::gen_tid()
{
	return to_string(chrono::system_clock::now().time_since_epoch().count());
}

string Transaction::request(const Action &a)
{
	httplib::Client cli(a.host.c_str());
	httplib::Result res;
	if (a.method == "GET")
	{
		if (!(res = cli.Put(a.path.c_str(), a.data.c_str(), "application/json")))
		{
			throw res.error();
		}
	}
	else if (a.method == "POST")
	{
		if (!(res = cli.Post(a.path.c_str(), a.data.c_str(), "application/json")))
		{
			throw res.error();
		}
	}

	if (res->status != 200)
	{
		throw res->body;
	}

	return res->body;
};

Transaction::Transaction(Transaction &&other) noexcept
{
	id = std::move(other.id);
	steps = std::move(other.steps);
	current_step = other.current_step;
	status = other.status;
	// No need to move mutex, as it is non-copyable
}

Transaction &Transaction::operator=(Transaction &&other) noexcept
{
	if (this != &other)
	{
		id = std::move(other.id);
		steps = std::move(other.steps);
		current_step = other.current_step;
		status = other.status;
		// No need to move mutex, as it is non-copyable
	}
	return *this;
};