#include <iostream>

#include <vector>
#include <map>
#include <string>
#include <queue>

using namespace std;
/*
	Global variables
*/

char HORIZONTAL_SYMBOLS[] = {'a', 'b', 'c', 'd'};
char VERTICAL_SYMBOLS[] = {'x', 'y', 'z', 'w'};
char DEAN_SYMBOLS[] = {'o', 'o', 'o', 'o'};

/*
	Car object
*/

class Car {
	public:

		vector<int> position;
		bool isHorizontal;
		int carLength;

		Car(int xPos, int yPos, bool horizontal, int length);

		vector<int> getStartPoint();
		vector<int> getEndPoint();

		vector<int> getStartPositionAfterMove(int steps);
		vector<int> getEndPositionAfterMove(int steps);
		void move(int steps);
};


// Car constructor
Car::Car(int xPos, int yPos, bool horizontal, int length) {
	position.push_back(xPos);
	position.push_back(yPos);
	isHorizontal = horizontal;
	carLength = length;
}

// Chceck the car position after move of `steps` moves (Horizontally or diagonally depending on the position of the car)
vector<int> Car::getStartPositionAfterMove(int steps){

	vector<int> newPosition;

	if (isHorizontal) {
		newPosition.push_back(position[0] + steps);
		newPosition.push_back(position[1]);
	} else {
		newPosition.push_back(position[0]);
		newPosition.push_back(position[1] + steps);
	}

	return newPosition;
}

vector<int> Car::getEndPositionAfterMove(int steps){

	vector<int> newPosition;

	if (isHorizontal) {
		newPosition.push_back(position[0] + steps + carLength);
		newPosition.push_back(position[1]);
	} else {
		newPosition.push_back(position[0]);
		newPosition.push_back(position[1] + steps + carLength);
	}

	return newPosition;
}


// Move
void Car::move(int steps) {
	
	if (isHorizontal) {
		position[0] += steps;
	} else {
		position[1] += steps;
	}
}

// Start and end points getters
// Start point
vector<int> Car::getStartPoint(){
	return position;
}

// End point
vector<int> Car::getEndPoint(){

	vector<int> endPosition;	
	if (isHorizontal) {
		endPosition.push_back(position[0] + carLength);
		endPosition.push_back(position[1]);
	} else {
		endPosition.push_back(position[0]);
		endPosition.push_back(position[1] + carLength);
	}

	return endPosition;
}


/*
	Board object
*/

class Board {
	public:

		int width;
		int height;

		vector<vector<bool>> map2d;
		vector<Car> cars;
		vector<int> winField;
		int numCars = 0;
		
		int gameLength = 0; 
		string history = "";

		Board();
		Board(int W, int H);
		Board(int W, int H, vector<vector<bool>> newMap2d, vector<Car> newCars, vector<int> newWinField, int carsNumber, int newGameLenght, string newHistory);


		int getWidth();
		int getHeight();
		vector<vector<bool>> getMap2d();
		bool getMap2dOn(int xPos, int yPos) const;
		vector<Car> getCars();
		int getNumCars();
		vector<int> getWinField();
		int getGameLength();
		string getHistory();

		vector<int> getPossibleMoves(Car car);

		void placeBound(int x, int y);
		void placeWinField(int x, int y);

		void placeCar(Car& car);
		void deleteCar(Car& car);
		void addCar(Car& car);

		void moveCar(Car& car, int steps);
		void updateHistory(vector<int> oldPosition, bool isHorizontal, int steps);

		bool isEmpty(int xPos, int yPos);
		bool canMoveBy(Car& car, int steps);
		bool isSolved();

	
		void printMap2d();


		bool operator<(const Board& other) const {
            for (int i = 0; i < width; i++) {
                for (int j = 0; j < height; j++) {
                    if (map2d[j][i] != other.getMap2dOn(i, j)) {
                        return map2d[j][i] < other.getMap2dOn(i, j);
                    }
                }
            }
            return false;
        }
        bool operator==(const Board& other) const {
            for (int i = 0; i < width; i++) {
                for (int j = 0; j < height; j++)
                    if (map2d[j][i] != other.getMap2dOn(i, j))
                        return false;
            }
			return true;
        }
        bool operator!=(const Board& other) const {
            for (int i = 0; i < width; i++) {
                for (int j = 0; j < height; j++)
                    if (map2d[j][i] != other.getMap2dOn(i, j))
                        return true;
            }
			return false;
        }

};

Board::Board(){}

// Empty board constructor
Board::Board(int W, int H){
	width = W;
	height = H;

	for (int i = 0; i < H; i++){
		vector<bool> row(W, false); 
		map2d.push_back(row);
	}
}

// Full board constructor
Board::Board(int W, int H, vector<vector<bool>> newMap2d, vector<Car> newCars, vector<int> newWinField, int carsNumber, int newGameLength, string newHistory){
	width = W;
	height = H;
	map2d = newMap2d;
	cars = newCars;
	winField = newWinField;
	numCars = carsNumber;
	gameLength = newGameLength;
	history = newHistory;
}



// Getters

// Get size
int Board::getWidth(){ return width; }
int Board::getHeight(){ return height; }



// Get whole map
vector<vector<bool>> Board::getMap2d(){
	return map2d;
}

// Get field of teh map
bool Board::getMap2dOn(int xPos, int yPos) const {
	return map2d[yPos][xPos];
}

// Get the list of the cars
vector<Car> Board::getCars(){
	return cars;
}

// Get the number of cars
int Board::getNumCars(){
	return numCars;
}

vector<int> Board::getWinField(){
	return winField;
}

int Board::getGameLength() {
	return gameLength;
}

string Board::getHistory(){
	return to_string(gameLength) + history;
}


// Get possible numbers of steps that the car can move
vector<int> Board::getPossibleMoves(Car car) {
	int forward_step = 1;
	int backward_step = -1;

	vector<int> steps;

	// forward
	while (canMoveBy(car, forward_step)){
		steps.push_back(forward_step);
		forward_step += 1;
	}

	// backward
	while (canMoveBy(car, backward_step)){
		steps.push_back(backward_step);
		backward_step -= 1;
	}

	return steps;
}



// Operations
// Add a bound
void Board::placeBound(int x, int y) {
	map2d[y][x] = true;
}

// Mark a win field
void Board::placeWinField(int x, int y) {
	winField.push_back(x);
	winField.push_back(y);
}

// Place a car on map2d
void Board::placeCar(Car& car){

	vector<int> startPos = car.getStartPoint();
	vector<int> endPos = car.getEndPoint();

	int xVect = endPos[0] - startPos[0];
	int yVect = endPos[1] - startPos[1];

	for (int i = 0; i <= xVect; i++) {
		for (int j = 0; j <= yVect; j++) {
			map2d[startPos[1] + j][startPos[0] + i] = true;
		}
	}
}

// Delete car forom map 2d
void Board::deleteCar(Car& car) {

	vector<int> startPos = car.getStartPoint();
	vector<int> endPos = car.getEndPoint();

	int xVect = endPos[0] - startPos[0];
	int yVect = endPos[1] - startPos[1];

	for (int i = 0; i <= xVect; i++) {
		for (int j = 0; j <= yVect; j++) {
			map2d[startPos[1] + j][startPos[0] + i] = false;
		}
	}
}

// Add new car to the board
void Board::addCar(Car& car){
	cars.push_back(car);
	placeCar(car);
	numCars += 1;
}


// Move car
void Board::moveCar(Car& car, int steps) {

	if (canMoveBy(car, steps)) {
		// Information for history purposes
		vector<int> oldPosition = car.getStartPoint();
		bool isHorizontal = car.isHorizontal;

		// Move the car
		deleteCar(car);
		car.move(steps);
		placeCar(car);

		// Update history
		updateHistory(oldPosition, isHorizontal, steps);
	}
}

// Update the history for future display
void Board::updateHistory(vector<int> oldPosition, bool isHorizontal, int steps) {
	history += "\n";

	history += to_string(oldPosition[0]) + " " + to_string(oldPosition[1]) + " ";

	if (isHorizontal){
		if (steps < 0) {
			history += "L " + to_string(-steps);
		} else {
			history += "R " + to_string(steps);
		}
	} else {
		if (steps < 0) {
			history += "U " + to_string(-steps);
		} else {
			history += "D " + to_string(steps);
		}
	}
}




// Checkers
// Is empty
bool Board::isEmpty(int xPos, int yPos) {
	if (0 <= xPos && xPos < width && 0 <= yPos && yPos < height) return !map2d[yPos][xPos];
	return false;	
}

// Chceck if car can move by `steps` steps
bool Board::canMoveBy(Car& car, int steps){
	int xPos, yPos;
	if (steps < 0){
		vector<int> newPosition = car.getStartPositionAfterMove(steps);
		xPos = newPosition[0];
		yPos =  newPosition[1];
	} else {
		vector<int> newPosition = car.getEndPositionAfterMove(steps);
		xPos = newPosition[0];
		yPos =  newPosition[1];
	}

	return isEmpty(xPos, yPos);
}

// Check if the board is solved
bool Board::isSolved(){
	int xPos = winField[0];
	int yPos = winField[1];
	return getMap2dOn(xPos, yPos);
}


// Additional
// Display map
void Board::printMap2d() {
	for (int j = 0; j < height; j++) {
		for (int i = 0; i < width; i++) {
			cout << map2d[j][i];
		}
	cout << endl;
	}
}

/*
	BFS Solver
*/

class Solver {
	public:
		queue<Board> Q;
		map<Board, Board> visited;

		Solver();

		vector<Board> getPossibleBoards(Board board);

		string run(Board board, int maxDepth);

};

// constructor (??)
Solver::Solver() {}

// Generates the list of boards achivable from current board in one moc=ve
vector<Board> Solver::getPossibleBoards(Board board){

	vector<Board> possibleBoards;

	int numCars = board.getNumCars();
	vector<Car> cars = board.getCars();

	for (int i = 0; i < numCars; i++) {

		Car car = cars[i];
		vector<int> steps = board.getPossibleMoves(car);
		int numOptions = steps.size();
		

		for (int j = 0; j < numOptions; j++) {

			Board newBoard = Board(
				board.getWidth(),
				board.getHeight(),
				board.getMap2d(),
				board.getCars(),
				board.getWinField(),
				board.getNumCars(),
				board.getGameLength(),
				board.getHistory()
			);

			Car newCar = newBoard.cars[i];
			newBoard.moveCar(newCar, steps[j]);

			possibleBoards.push_back(newBoard);
		}

	}

	return possibleBoards;
}

// Main solver using BFS algorithm
string Solver::run(Board initialBoard, int maxDepth){
	
	visited[initialBoard] = initialBoard;
	Q.push(initialBoard);

	while (!Q.empty()) {
		Board currentBoard = Q.front();
		Q.pop();

		if (currentBoard.isSolved()){
			return currentBoard.getHistory();
		}

		// Get all the neighbours
		vector<Board> neighbours = getPossibleBoards(currentBoard);

		int neighboursSize = neighbours.size();

		for (int i = 0; i < neighboursSize; i++) {
			Board neighbour = neighbours[i];

			// check if the neighbour was already calculated
			if (!visited.count(neighbour)) {
				visited[neighbour] = neighbour;

				Q.push(neighbour);
			}
		}
	}
	

	return "No solution";
}


/*
	Help functions
*/

// Horizontal car
Car createHorizontalCar(vector<string> rows, int xPos, int yPos, char symbols[], int W){
	int length = 0;

	for (int i = 1; i < 4; i++) {
		if (xPos + i < W){
			char nextSymbol = rows[yPos][xPos + i];
			if (nextSymbol == symbols[i]) {
				length++;
			} else break;
			
		} else break;
	}
	
	Car car = Car(xPos, yPos, true, length);
	return car;
}

// Vertical car
Car createVerticalCar(vector<string> rows, int xPos, int yPos, char symbols[], int H){
	int length = 0;
	
	for (int j = 1; j < 4; j++) {
		if (yPos + j < H) {
			char nextSymbol = rows[yPos + j][xPos];
			if (nextSymbol == symbols[j]) {
				length++;
			} else break;

		} else break;
	}
	
	Car car = Car(xPos, yPos, false, length);
	return car;
}

// Create dean car
Car createDeanCar(vector<string> rows, int xPos, int yPos, int W, int H){

	if (xPos + 1 < W && rows[yPos][xPos + 1] == 'o') {
		Car car = createHorizontalCar(rows, xPos, yPos, DEAN_SYMBOLS, W);
		return car;
	} else {
		Car car = createVerticalCar(rows, xPos, yPos, DEAN_SYMBOLS, H);
		return car;
	}
}


// create_board
Board parse_args(int W, int H, vector<string> rows) {

	
	Board board = Board(W, H);

	bool dean_car_not_found = true;

	for (int j = 0; j < H; j++) {
		for (int i = 0; i < W; i++) {
			char symbol = rows.at(j).at(i);
			if (symbol == '#') {
				board.placeBound(i, j);

			} else if (symbol == 'a') {
				Car car = createHorizontalCar(rows, i, j, HORIZONTAL_SYMBOLS, W);		
				board.addCar(car);

			} else if (symbol == 'x') {
				Car car = createVerticalCar(rows, i, j, VERTICAL_SYMBOLS, H);
				board.addCar(car);

			} else if (symbol == 'o' && dean_car_not_found) {
				dean_car_not_found = false;
				Car car = createDeanCar(rows, i, j, W, H);
				board.addCar(car);
			} else if (symbol == '.' && (j == 0 || i == W - 1)){
				board.placeWinField(i, j);
			}
			
		}
	}

	return board;
}

/*
	Main Code
*/

int main(int argc, char *argv[]){

	vector<string> rows;
	
	int W = stoi(argv[1]);
	int H = stoi(argv[2]);
	int N = stoi(argv[3]);

	for (int i = 4; i < H + 4; i++) {
		rows.push_back(argv[i]);
	}

	Board board = parse_args(W, H, rows);
	Solver solver = Solver();

	string solution = solver.run(board, N);

	cout << solution << endl;


	// vector<Board> boards = solver.getPossibleBoards(board);
	// int numBoards = boards.size();

	// cout << numBoards << endl;
	// for (int i = 0; i < numBoards; i++){
	// 	cout << boards[i].getHistory() << endl;
	// }

	int i = 4;

	while (i < argc){
		cout << " " << argv[i] << endl;
		i++;
		}
	return 	0;
}