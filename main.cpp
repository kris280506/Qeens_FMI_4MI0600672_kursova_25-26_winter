
/**
*
* Solution to course project # 6
* Introduction to programming course
* Faculty of Mathematics and Informatics of Sofia University
* Winter semester 2025/2026
*
* @author Kristian Varbanov
* @faculty number 4MI0600672
* @compiler VC
*
* Main file with game logic
*
*/
#include <iostream>
#include <cmath> // Za abs()

using namespace std;

// Maksimalen broi hodove za istoriyata
const int MAX_HISTORY = 1000;

// Struktura, koyato opisva sutoyanieto na igrata
struct Game {
	int** board;            // dvumeren masiv za duskata
	int rows;               // broi redove
	int cols;               // broi koloni
	int currentPlayer;      // tekusht igrach (1 ili 2)
	
	// masivi za istoriyata (populvat se pri hod, dori oshte da nyama Back komanda)
	int moveHistoryRow[MAX_HISTORY]; 
	int moveHistoryCol[MAX_HISTORY]; 
	int moveCount;          // broi hodove
	bool isRunning;         // buleva dali igrata teche
};

// Pomoshtna funkciya za sravnyavane na dva niza (zamestva strcmp ot cstring)
bool areEqual(const char* s1, const char* s2) {
	int i = 0;
	while (s1[i] != '\0' && s2[i] != '\0') {
		if (s1[i] != s2[i]) return false;
		i++;
	}
	return (s1[i] == '\0' && s2[i] == '\0');
}

// Funkcia za osvobojdavane na dinamichnata pamet
void clearMemory(Game& game) {
	if (game.board != nullptr) {
		for (int i = 0; i < game.rows; i++) {
			delete[] game.board[i]; // Iztrivane na vseki red
		}
		delete[] game.board; // Iztrivane na masiva ot ukazateli
		game.board = nullptr;
	}
}

// Inicializaciya na nova igra
void initGame(Game& game, int N, int M) {
	clearMemory(game); // Izchistvame starata pamet predi nova igra
	game.rows = N;
	game.cols = M;
	game.currentPlayer = 1;
	game.moveCount = 0;
	game.isRunning = true;
	
	// Zadelqne na pamet za novata dyska
	game.board = new int*[N];
	for (int i = 0; i < N; i++) {
		game.board[i] = new int[M];
		for (int j = 0; j < M; j++) {
			game.board[i][j] = 0; // 0 oznachava prazna kletka
		}
	}
}

// Proverka dali dadena kletka e pod ataka ot veche postaveni dami
bool isUnderAttack(Game& game, int r, int c) {
	for (int i = 0; i < game.rows; i++) {
		for (int j = 0; j < game.cols; j++) {
			if (game.board[i][j] != 0) {
				// Proverka za suvpdenie po red, kolona ili diagonal
				if (i == r || j == c || abs(i - r) == abs(j - c)) {
					return true;
				}
			}
		}
	}
	return false;
}

// Proverka dali hodyt e pozvolen spored pravilata
bool isValidMove(Game& game, int r, int c) {
	// Proverka dali koordinatite sa v ramkite na duskata
	if (r < 0 || r >= game.rows || c < 0 || c >= game.cols) return false;
	
	// Ne mojem da slagame dama vurhu druga dama
	if (game.board[r][c] != 0) return false;
	
	// Proverka dali kletkata e atakuvana ot druga dama 
    if (isUnderAttack(game, r, c)) return false;
	return true;
}

// Proverka dali imat ostanali validni hodove na duskata (za Game Over)
bool hasValidMoves(Game& game) {
	for (int i = 0; i < game.rows; i++) {
		for (int j = 0; j < game.cols; j++) {
			if (isValidMove(game, i, j)) {
				return true; 
			}
		}
	}
	return false; 
}

// Funkciya za izvejdane na duskata v konzolata
void showBoard(Game& game) {
	if (game.board == nullptr) {
		cout << "No active game.\n";
		return;
	}

	// Otpechatvane na nomerata na kolonite
	cout << "  ";
	for (int j = 0; j < game.cols; j++) cout << j << " ";
	cout << endl;

	for (int i = 0; i < game.rows; i++) {
		cout << i << " "; // Nomer na reda
		for (int j = 0; j < game.cols; j++) {
			if (game.board[i][j] == 1) cout << "1 ";      // Igrach 1
			else if (game.board[i][j] == 2) cout << "2 "; // Igrach 2
			else if (isUnderAttack(game, i, j)) cout << "* "; // Zastrashena kletka
			else cout << "_ "; // Svobodna kletka
		}
		cout << endl;
	}
}

int main() {
	Game game;
	game.board = nullptr; 
	game.rows = 0;
	game.cols = 0;
	
	char command[50]; // Bufer za chetene na komandi
	cout << "Queens Game. Write 'help' for commands.\n";

	while (true) {
		cout << "> ";
		cin >> command;

		// Izhod ot programata
		if (areEqual(command, "exit")) {
			break; 
		}
		// Syzdavane na nova igra
		else if (areEqual(command, "new")) {
			int n, m;
			cin >> n >> m;
			// Proverka za minimalen razmer
			if (n > 3 && m > 3) {
				initGame(game, n, m);
				cout << "New game: " << n << "x" << m << endl;
				showBoard(game);
			} else {
				cout << "Invalid size. Min 4x4.\n";
			}
		}
		// Vizualizaciya na duskata
		else if (areEqual(command, "show")) {
			showBoard(game);
		}
		// Postaviane na dama (Hod)
		else if (areEqual(command, "play")) {
			if (game.board == nullptr) {
				cout << "First start a new game.\n";
				continue;
			}
			int r, c;
			cin >> r >> c; // Chetene na koordinati
			
			if (isValidMove(game, r, c)) {
				// Postavya damata na tekushtiya igrach
				game.board[r][c] = game.currentPlayer;
				
				// Zapazvane v istoriyata (dori bez 'back' komanda, e dobre da se pazi)
				game.moveHistoryRow[game.moveCount] = r;
				game.moveHistoryCol[game.moveCount] = c;
				game.moveCount++;
				
				// Smyana na reda (ako e 1 stava 2, inache 1)
				game.currentPlayer = (game.currentPlayer == 1) ? 2 : 1;
				showBoard(game);
				
				// Proverka za kraya na igrata (dostupno ot Commit 3)
				if (!hasValidMoves(game)) {
					// Pobedityal e tozi, koito toku shto e igral
					int winner = (game.currentPlayer == 1) ? 2 : 1;
					cout << "Game over! Player " << winner << " wins!\n";
					clearMemory(game); 
				}

			} else {
				cout << "Invalid move.\n";
			}
		}
		// Informaciya za tova koi e na hod
		else if (areEqual(command, "turn")) {
			if (game.board != nullptr)
				cout << "Player " << game.currentPlayer << " is on turn.\n";
		}
		// Pokazvane na vsichki svobodni kletki
		else if (areEqual(command, "free")) {
			if (game.board == nullptr) continue;
			cout << "Free cells: ";
			// Obhojdame cyalata dyska i pechatame validnite
			for (int i = 0; i < game.rows; i++) {
				for (int j = 0; j < game.cols; j++) {
					if (isValidMove(game, i, j)) {
						cout << "(" << i << "," << j << ") ";
					}
				}
			}
			cout << endl;
		}else if (areEqual(command, "back")) {
			if (game.board == nullptr || game.moveCount == 0) {
				cout << "Nyama hodove za vrushtane.\n";
			} else {
				game.moveCount--;
				int lastR = game.moveHistoryRow[game.moveCount];
				int lastC = game.moveHistoryCol[game.moveCount];
				
				game.board[lastR][lastC] = 0;
				
				game.currentPlayer = (game.currentPlayer == 1) ? 2 : 1;
				cout << "Posledniyat hod e otmenen.\n";
				showBoard(game);
			}
		}
		// Pomoshtno menu
		else if (areEqual(command, "help")) {
			cout << "Commands:\n";
			cout << "new N M      - New game with size NxM\n";
			cout << "play x y     - Place queen at (x,y)\n";
			cout << "free         - Show free cells\n";
			cout << "show         - Show the board\n";
			cout << "turn         - Who is on turn\n";
			cout << "exit         - Exit\n";
		}
	}

	clearMemory(game); // Pochistvane na pametta pri izhod
	return 0;
}