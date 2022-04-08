#include <fstream>
#include <iostream>
#include <vector>

class Animal {
protected:
	int direct;
	int constant;
	int x;
	int y;
	int moves;
	int age;
public:
	Animal(int direct, int constant, int x, int y) {
		this->age = 0;
		this->direct = direct;
		this->constant = constant;
		this->x = x;
		this->y = y;
		this->moves = 0;
	}
	virtual void changeDirect() {
		if (moves == constant) {
			direct++;
			moves = 0;
		}
		if (direct == 4) {
			direct = 0;
		}
	}
	void move() {}
	virtual void aging() {
		age++;
	}
	virtual int& getAge() {
		return this->age;
	}
	virtual int& getDirect() {
		return this->direct;
	}
	virtual int& getX() {
		return this->x;
	}
	virtual int& getY() {
		return this->y;
	}
	virtual int& getConstant() {
		return this->constant;
	}
	virtual int& getMoves() {
		return this->moves;
	}
	virtual void eat() = 0;
	virtual bool dying() = 0;
	virtual bool reproduction() = 0;
};

class Wolf : public Animal {
	int eaten;
	int born;
	int parent;
public:
	Wolf(int x, int y, int direct, int constant, int born) : Animal(direct, constant, x, y) {
		this->eaten = 0;
		this->born = 0;
		this->parent = 0;
	}
	void eat() {
		eaten++;
	}
	bool dying() {
		if (age == 15) {
			return true;
		}
		else return false;
	}
	void move(int n, int m) {
		switch (direct) {
		case 0:
			x += 2;
			if (x >= n) {
				x -= n;
			}
			break;
		case 1:
			y -= 2;
			if (y < 0) {
				y += m;
			}
			break;
		case 2:
			x -= 2;
			if (x < 0) {
				x += n;
			}
			break;
		case 3:
			y += 2;
			if (y >= m) {
				y -= m;
			}
			break;
		}
		moves++;
	}
	bool reproduction() {
		if (eaten >= 2) {
			eaten = eaten % 2;
			return true;
		}
		else return false;
	}
	bool olderWolf(Wolf secondWolf, int j, int i) {
		if (this->getAge() > secondWolf.getAge()) {
			return true;
		}
		if (this->getAge() == secondWolf.getAge()) {
			if (this->getBorn() == 0 && j > i) {
				return true;
			}
			if (this->getParent() > secondWolf.getParent()) {
				return true;
			}
		}
		return false;
	}
	int& getBorn() {
		return this->born;
	}
	int& getEaten() {
		return this->eaten;
	}
	int& getParent() {
		return this->parent;
	}
};

class Hyena : public Wolf {
public:
	Hyena(int x, int y, int direct, int constant, int born) : Wolf(x, y, direct, constant, born) {}
	bool reproduction() {
		if (this->getEaten() == 2) {
			this->getEaten() = 5;
			return true;
		}
		else return false;
	}
	void eat() {
		if (this->getEaten() < 2) {
			(this->getEaten())++;
		}
	}
	bool olderHyena(Hyena secondHyena, int j, int i) {
		if (this->getAge() > secondHyena.getAge()) {
			return true;
		}
		if (this->getAge() == secondHyena.getAge()) {
			if (this->getBorn() == 0 && j > i) {
				return true;
			}
			if (this->getParent() > secondHyena.getParent()) {
				return true;
			}
		}
		return false;
	}
};

class Rabbit : public Animal {
public:
	Rabbit(int x, int y, int direct, int constant) : Animal(direct, constant, x, y) {}

	void eat() {}

	bool dying() {
		if (age == 10) {
			return true;
		}
		else return false;
	}
	bool reproduction() {
		if (age == 5 || age == 10) {
			return true;
		}
		else return false;
	}
	void move(int n, int m) {
		switch (direct) {
		case 0:
			x++;
			if (x >= n) {
				x -= n;
			}
			break;
		case 1:
			y--;
			if (y < 0) {
				y += m;
			}
			break;
		case 2:
			x--;
			if (x < 0) {
				x += n;
			}
			break;
		case 3:
			y++;
			if (y >= m) {
				y -= m;
			}
			break;
		}
		moves++;
	}
};

class Simulation {
	int n, m;
	int numOfRabbits;
	int numOfWolfs;
	int numOfHyens;
	std::vector<Wolf> wolfs;
	std::vector<Rabbit> rabbits;
	std::vector<Hyena> hyens;
public:
	Simulation(int n, int m) {
		numOfHyens = 0;
		numOfRabbits = 0;
		numOfWolfs = 0;
		this->n = n;
		this->m = m;
	}
	void addWolf(Wolf newWolf) {
		wolfs.push_back(newWolf);
		numOfWolfs++;
	}
	void addRabbit(Rabbit newRabbit) {
		rabbits.push_back(newRabbit);
		numOfRabbits++;
	}
	void addHyena(Hyena newHyena) {
		hyens.push_back(newHyena);
		numOfHyens++;
	}

	void sortWolf() {
		for (int i = 0; i < numOfWolfs; i++) {
			Wolf oldest = wolfs[i];
			int indexOfOldest = i;
			for (int j = i + 1; j < numOfWolfs; j++) {
				if (wolfs[j].olderWolf(oldest, j, i)) {
					oldest = wolfs[j];
					indexOfOldest = j;
				}

			}
			wolfs[indexOfOldest] = wolfs[i];
			wolfs[i] = oldest;

		}
	}
	void sortRabbit() {
		for (int i = 0; i < numOfWolfs; i++) {
			Rabbit oldest = rabbits[i];
			int indexOfOldest = i;
			for (int j = i + 1; j < numOfWolfs; j++) {
				if (rabbits[j].getAge() > oldest.getAge()) {
					oldest = rabbits[j];
					indexOfOldest = j;
				}
			}
			rabbits[indexOfOldest] = rabbits[i];
			rabbits[i] = oldest;
		}
	}
	void sortHyena() {
		for (int i = 0; i < numOfWolfs; i++) {
			Hyena oldest = hyens[i];
			int indexOfOldest = i;
			for (int j = i + 1; j < numOfWolfs; j++) {
				if (hyens[j].olderHyena(oldest, j, i)) {
					oldest = hyens[j];
					indexOfOldest = j;
				}
			}
			hyens[indexOfOldest] = hyens[i];
			hyens[i] = oldest;
		}
	}
	void play(int time) {
		for (int i = 0; i < time; i++) {
			//moves
			for (int j = 0; j < numOfRabbits; j++) {
				rabbits[j].move(n, m);
			}
			for (int j = 0; j < numOfWolfs; j++) {
				wolfs[j].move(n, m);
			}
			for (int j = 0; j < numOfHyens; j++) {
				hyens[j].move(n, m);
			}
			for (int j = 0; j < numOfRabbits; j++) {
				rabbits[j].changeDirect();
			}
			for (int j = 0; j < numOfWolfs; j++) {
				wolfs[j].changeDirect();
			}
			for (int j = 0; j < numOfHyens; j++) {
				hyens[j].changeDirect();
			}
			//hyena`s food
			for (int j = 0; j < numOfHyens; j++) {
				for (int k = j + 1; k < numOfHyens; k++) {
					if (hyens[j].getX() == hyens[k].getX() && hyens[j].getY() == hyens[k].getY()) {
						hyens[j].eat();
						for (int r = k; r < numOfHyens - 1; r++) {
							hyens[r] = hyens[r + 1];
						}
						hyens.pop_back();
						numOfHyens--;
						k--;
					}
				}
			}

			for (int j = 0; j < numOfHyens; j++) {
				for (int k = 0; k < numOfWolfs; k++) {
					if (hyens[j].getX() == wolfs[k].getX() && hyens[j].getY() == wolfs[k].getY()) {
						hyens[j].eat();
						for (int r = k; r < numOfWolfs - 1; r++) {
							wolfs[r] = wolfs[r + 1];
						}
						wolfs.pop_back();
						numOfWolfs--;
						k--;
					}
				}
			}

			for (int j = 0; j < numOfHyens; j++) {
				for (int k = 0; k < numOfRabbits; k++) {
					if (hyens[j].getX() == rabbits[k].getX() && hyens[j].getY() == rabbits[k].getY()) {
						hyens[j].eat();
						for (int r = k; r < numOfRabbits - 1; r++) {
							rabbits[r] = rabbits[r + 1];
						}
						rabbits.pop_back();
						numOfRabbits--;
						k--;
					}
				}
			}
			//wolf`s food
			for (int j = 0; j < numOfWolfs; j++) {
				for (int k = 0; k < numOfRabbits; k++) {
					if (wolfs[j].getX() == rabbits[k].getX() && wolfs[j].getY() == rabbits[k].getY()) {
						wolfs[j].eat();
						for (int r = k; r < numOfRabbits - 1; r++)
							rabbits[r] = rabbits[r + 1];
						rabbits.pop_back();
						numOfRabbits--;
						k--;
					}
				}
			}
			//aging
			for (int j = 0; j < numOfRabbits; j++) {
				rabbits[j].aging();
			}
			for (int j = 0; j < numOfWolfs; j++) {
				wolfs[j].aging();
			}
			for (int j = 0; j < numOfHyens; j++) {
				hyens[j].aging();
			}
			//reproduction
			for (int j = 0; j < numOfRabbits; j++) {
				if (rabbits[j].reproduction()) {
					Rabbit newRabbit(rabbits[j].getX(), rabbits[j].getY(), rabbits[j].getDirect(), rabbits[j].getConstant());
					addRabbit(newRabbit);//добавляем
				}
			}
			for (int j = 0; j < numOfWolfs; j++) {
				if (wolfs[j].reproduction()) {
					Wolf newWolf(wolfs[j].getX(), wolfs[j].getY(), wolfs[j].getDirect(), wolfs[j].getConstant(), i + 1);
					newWolf.getParent() = wolfs[j].getAge();
					addWolf(newWolf);
				}
			}
			for (int j = 0; j < numOfHyens; j++) {
				if (hyens[j].reproduction()) {
					Hyena newHyena(wolfs[j].getX(), wolfs[j].getY(), wolfs[j].getDirect(), wolfs[j].getConstant(), i + 1);
					newHyena.getParent() = wolfs[j].getAge();
					addHyena(newHyena);
				}
			}
			//dying
			for (int j = 0; j < numOfRabbits; j++) {
				if (rabbits[j].dying()) {
					for (int k = j; k < numOfRabbits - 1; k++) {
						rabbits[k] = rabbits[k + 1];
					}
					rabbits.pop_back();
					numOfRabbits--;
					j--;
				}
			}
			for (int j = 0; j < numOfWolfs; j++) {
				if (wolfs[j].dying()) {
					for (int k = j; k < numOfWolfs - 1; k++)
						wolfs[k] = wolfs[k + 1];
					wolfs.pop_back();
					numOfWolfs--;
					j--;
				}
			}
			for (int j = 0; j < numOfHyens; j++) {
				if (hyens[j].dying()) {
					for (int k = j; k < numOfHyens - 1; k++)
						hyens[k] = hyens[k + 1];
					hyens.pop_back();
					numOfHyens--;
					j--;
				}
			}
		}
	}
	void print() {
		int** field = new int* [n];
		for (int i = 0; i < n; i++)
			field[i] = new int[m];
		for (int i = 0; i < n; i++)
			for (int j = 0; j < m; j++)
				field[i][j] = 0;
		for (int i = 0; i < numOfWolfs; i++)
			field[wolfs[i].getX()][wolfs[i].getY()]--;
		for (int i = 0; i < numOfRabbits; i++)
			field[rabbits[i].getX()][rabbits[i].getY()]++;
		std::ofstream out("output.txt");
		for (int i = 0; i < n; i++)
		{
			for (int j = 0; j < m; j++)
				if (field[i][j] == 0)
					out << "#";
				else out << field[i][j];
			out << std::endl;
		}
		out.close();
	}
};


int main() {
	std::ifstream input("input.txt");
	int n;
	int m;
	int time;
	int numWolfs;
	int numRabbits;
	input >> n >> m >> time >> numRabbits >> numWolfs;
	Simulation simulation(n, m);
	int x;
	int y;
	int direct;
	int constant;
	for (int i = 0; i < numRabbits; i++) {
		input >> x >> y >> direct >> constant;
		Rabbit rabbit(x, y, direct, constant);
		simulation.addRabbit(rabbit);
	}
	for (int i = 0; i < numWolfs; i++) {
		input >> x >> y >> direct >> constant;
		Wolf wolf(x, y, direct, constant, 0);
		simulation.addWolf(wolf);
	}
	input.close();
	simulation.play(time);
	simulation.print();
	return 0;
}