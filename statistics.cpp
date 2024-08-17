#include <iostream>
#include <limits>
#include <vector>
#include <algorithm>

class IStatistics {
public:
	virtual ~IStatistics() {}

	virtual void update(double next) = 0;
	virtual double eval() const = 0;
	virtual const char *name() const = 0;

	void sort_numbers(std::vector<double> & arr) const {
		// size_t start = 0;
		// for (size_t i = start + 1; i < arr.size(); i++) {
		// 	for (size_t j = i; j > start && arr[j-1] > arr[j]; j--) {
		// 		double temp = arr[j - 1];
		// 		arr[j - 1] = arr[j];
		// 		arr[j] = temp;
		// 	}
		// }
		sort(arr.begin(), arr.end());
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
	Max() : m_max{std::numeric_limits<double>::lowest()} {
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

class Pct : public IStatistics {
public:
	Pct(int percent) : m_pct{0}, m_percent{percent} {}

	double pct(const std::vector<double> & arr, const int p) const {
		size_t index = ceil(p * arr.size() / 100.0) - 1;
		return arr[index];
	}

	void update(double next) override {
		numbers.push_back(next);
		sort_numbers(numbers);
		m_pct = pct(numbers, m_percent);
	}

	double eval() const override {
		return m_pct;
	}

	const char *name() const override {
		std::string str = "pct" + std::to_string(m_percent);
		int size_of_str = str.length();
		char *ch = new char[size_of_str];
		for (int i = 0; i < size_of_str; i++) {
			ch[i] = str[i];
		}
		return ch;
	}

private:
	double m_pct;
	int m_percent;
	std::vector<double> numbers;
};

int main() {

	const size_t statistics_count = 6;
	IStatistics *statistics[statistics_count] = { new Min(), new Max(), new Mean(),
												new Std(), new Pct(90), new Pct(95) };
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