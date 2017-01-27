#include <set>
#include <algorithm>
#include <vector>
#include <iostream>

#include <QMap>
#include <QList>

class Link;

class State {

	char m_id; // pour test seulement
	double m_x, m_y;
	std::vector<State> m_states; 
	QMap<State, double> m_costs;
public:
	State() { } // pour QMap, putain de Qt.
	State(char id, double x, double y):
		m_id(id), m_x(x), m_y(y) { }
		
	double x() const { return m_x; }
	double y() const { return m_y; }
	
	char id() const { return m_id; }
	
	const std::vector<State>& states() const { return m_states; }
	double cost(const State& state) const { return m_costs[state]; }
	
	void addState(const State& state, double cost) { m_states.push_back(state); m_costs[state] = cost; }
	
	bool operator==(const State& b) const { return m_id == b.m_id; }
	bool operator<(const State& b) const { return m_id < b.m_id; }
	State& operator=(const State& b) {
		m_id=b.m_id;
		m_x=b.m_x;
		m_y=b.m_y;
		m_states=b.m_states;
		return *this;
	}
};

template <class S>
struct manhattan_distance {
	typedef const S& const_reference;

	double operator()(const_reference a, const_reference b) const {
		double x = a.x() - b.x();
		double y = a.y() - b.y();
		return sqrt(x * x + y * y);
	}

};

template <class S, class L>
L reconstruct_path(const QMap<S, S>& came_from, const S& end) {
	typedef L list_type;

	typedef S state_type;
	typedef S& state_ref;
	typedef const S& state_const_ref;

	list_type path;
	state_type current = end;
	typename QMap<state_type, state_type>::const_iterator it;

	while((it = came_from.find(current)) != came_from.end()) {
		path.push_front(current);
		current = it.value(); // ->second
	}
	path.push_front(current);
	return path;
}

template <class S,
	class L = QList<S>,
	class H = manhattan_distance<S>>
L a_star(const S& start,
	const S& end,
	H heuristique = H())
{
	typedef L list_type;
	
	typedef S state_type;
	typedef S& state_ref;
	typedef const S& state_const_ref;
	
	std::set<state_type> closed_set;
	std::set<state_type> open_set;
	open_set.insert(start);
	QMap<state_type, state_type> came_from;

	QMap<state_type, double> g_score;
	QMap<state_type, double> f_score;

	g_score[start] = 0.0;
	f_score[start] = heuristique(start, end);

	while(!open_set.empty()) {
		auto it_current = std::min_element(open_set.begin(), open_set.end(),
			[&f_score](state_const_ref a, state_const_ref b)->bool {
				return f_score[a] < f_score[b];
		});
		// std::cout << "current: " << (*it_current).id() << std::endl;
		state_type current = *it_current;

		if(current == end) {
			return reconstruct_path<state_type, list_type>(came_from, end);
		}
		
		open_set.erase(it_current);
		closed_set.insert(current);
		
		double current_g_score = g_score[current];
		for(state_const_ref neighbor: current.states()) {
			// std::cout << "neighbor: " << neighbor.id() << std::endl;
			double new_g_score = current_g_score + current.cost(neighbor);
			if(closed_set.find(neighbor) != closed_set.end() && new_g_score >= g_score[neighbor]) {
				continue;
			}

			bool neighbor_not_in_os = open_set.find(neighbor) == open_set.end();
			if(neighbor_not_in_os || new_g_score < g_score[neighbor]) {
				came_from[neighbor] = current;
				g_score[neighbor] = new_g_score;
				f_score[neighbor] = new_g_score + heuristique(neighbor, end);
				if(neighbor_not_in_os) {
					open_set.insert(neighbor);
				}
			}
		}
	}

	return list_type();
}


int main(int, char **) {
		
	State a('a', -2.0, 0.0), b('b', 2.0, 0.0), c('c', 0.0, 1.0), d('d', 2.0, 2.0);

	c.addState(d, 2.0);
	c.addState(b, 1.0);

	b.addState(d, 2.0);

	a.addState(b, 6.0);
	a.addState(c, 1.0);

	auto l = a_star<State>(a, d);
	
	int i = 0;
	for(auto s: l) {
		if(i) std::cout << " -> ";
		std::cout << s.id();
	}
	std::cout << std::endl;
	
	return 0;
}