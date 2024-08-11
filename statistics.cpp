#include <iostream>
#include <limits>
#include <vector>

class IStatistics {
public:
	virtual ~IStatistics() {}

	virtual void update(double next) = 0;
	virtual double eval() const = 0;
	virtual const char * name() const = 0;

	void sort(std::vector<double> & arr) const {
		size_t start = 0;
		for (size_t i = start + 1; i < arr.size(); i++) {
			for (size_t j = i; j > start && arr[j-1] > arr[j]; j--) {
				double temp = arr[j - 1];
				arr[j - 1] = arr[j];
				arr[j] = temp;
			}
		}
	}

	double pct(const std::vector<double> & arr, int p) const {
		size_t index = ceil(p * arr.size() / 100.0) - 1;
		return arr[index];
	}

};

class Min : public IStatistics {
public:
	Min() : m_min{std::numeric_limits<double>::max()} {
	}
	void update(double next) override {
		if (next < m_min) {
			m_min = next;
		}
	}
	double eval() const override {
		return m_min;
	}
	const char * name() const override {
		return "min";
	}
private:
	double m_min;
};

class Max : public IStatistics {
public:
	Max() : m_max{/*std::numeric_limits<double>::min()*/0} {
	}
	void update(double next) override {
		if (next > m_max) {
			m_max = next;
		}
	}
	double eval() const override {
		return m_max;
	}
	const char * name() const override {
		return "max";
	}
private:
	double m_max; 
};

class Mean : public IStatistics {
public:
	Mean() : m_mean{0}, m_count{0}, m_sum{0} {
	}
	void update(double next) override {
		m_count++;
		m_sum += next;
		m_mean = m_sum / m_count;
	}
	double eval() const override {
		return m_mean;
	}
	const char * name() const override {
		return "mean";
	}
private:
	double m_mean;
	int m_count;
	double m_sum;
};

class Std : public IStatistics {
public:
	Std() : m_std{0} {
	}
	void update(double next) override {
		numbers.push_back(next);

		double sum = 0;
		for (double d : numbers) {
			sum += d;
		}
		long mean = sum / numbers.size();

		double sumq = 0;
		for (double d : numbers) {
			sumq += pow(d - mean, 2);
		}
		m_std = pow(sumq / numbers.size(), 0.5);
	}
	double eval() const override {
		return m_std;
	}
	const char * name() const override {
		return "std";
	}
private:
	double m_std;
	std::vector<double> numbers;
};

class Pct90 : public IStatistics {
public:
	Pct90() : m_pct{0} {
	}
	void update(double next) override {
		numbers.push_back(next);
		sort(numbers);
		m_pct = pct(numbers, 90);
	}
	double eval() const override {
		return m_pct;
	}
	const char * name() const override {
		return "pct90";
	}
private:
	double m_pct;
	std::vector<double> numbers;
};

class Pct95 : public IStatistics {
public:
	Pct95() : m_pct{0} {
	}
	void update(double next) override {
		numbers.push_back(next);
		sort(numbers);
		m_pct = pct(numbers, 95);
	}
	double eval() const override {
		return m_pct;
	}
	const char * name() const override {
		return "pct95";
	}
private:
	double m_pct;
	std::vector<double> numbers;
};

int main() {

	const size_t statistics_count = 6;
	IStatistics *statistics[statistics_count];

	statistics[0] = new Min{};
	statistics[1] = new Max{};
	statistics[2] = new Mean{};
	statistics[3] = new Std{};
	statistics[4] = new Pct90{};
	statistics[5] = new Pct95{};

	double val = 0;
	while (std::cin >> val) {
		for (size_t i = 0; i < statistics_count; ++i) {
			statistics[i]->update(val);
		}
	}

	// Handle invalid input data
	if (!std::cin.eof() && !std::cin.good()) {
		std::cerr << "Invalid input data\n";
		return 1;
	}

	// Print results if any
	for (size_t i = 0; i < statistics_count; ++i) {
		std::cout << statistics[i]->name() << " = " << statistics[i]->eval() << std::endl;
	}

	// Clear memory - delete all objects created by new
	for (size_t i = 0; i < statistics_count; ++i) {
		delete statistics[i];
	}

	return 0;
}