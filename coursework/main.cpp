#include <iostream> // підключення бібліотеки для введення/виведення в консоль
#include <vector>   // підключення бібліотеки для використання векторів
#include <cstdlib>  // підключення бібліотеки для роботи з функціями для керування пам'яттю
#include <ctime>    // підключення бібліотеки для роботи з функціями для роботи з часом
#include <fstream>  // підключення бібліотеки для роботи з файлами
#include <map>      // підключення бібліотеки для використання асоціативних масивів
#include <sstream>  // підключення бібліотеки для роботи з рядками

using namespace std; // використання простору імен стандартної бібліотеки

const int BOARD_SIZE = 10; // константа для розміру ігрового поля

struct Ship // оголошення структури корабля
{
    int x; // координата x корабля
    int y; // координата y корабля
};

class Player // Клас, що представляє гравця
{
private:
    string name;                // Ім'я гравця
    vector<Ship> ships;         // Вектор кораблів гравця
    vector<vector<char>> board; // Двовимірний вектор для представлення ігрового поля

public:
    Player(string playerName) : name(playerName) // Конструктор класу
    {
        board.resize(BOARD_SIZE, vector<char>(BOARD_SIZE, ' ')); // Ініціалізація ігрового поля пустими клітинами
    }

    void placeShipsAutomatically() // Метод для автоматичного розміщення кораблів
    {
        srand(time(NULL));                 // Ініціалізація генератора випадкових чисел
        int numShips = 5;                  // Кількість кораблів
        int shipSizes[] = {5, 4, 3, 3, 2}; // Розміри кораблів

        for (int i = 0; i < numShips; ++i) // Цикл для розміщення кожного корабля
        {
            int shipSize = shipSizes[i]; // Поточний розмір корабля
            int x, y;                    // Координати корабля
            char direction;              // Напрямок корабля
            bool validPlacement = false; // Прапорець валідного розміщення корабля

            while (!validPlacement) // Повторення доти, доки не буде здійснено валідне розміщення
            {
                x = rand() % BOARD_SIZE;                 // Випадкова координата x
                y = rand() % BOARD_SIZE;                 // Випадкова координата y
                direction = rand() % 2 == 0 ? 'V' : 'H'; // Випадковий напрямок: вертикальний або горизонтальний

                if (direction == 'V') // Якщо напрямок - вертикальний
                {
                    validPlacement = isValidPlacement(x, y, shipSize, true); // Перевірка валідності розміщення
                }
                else
                {
                    validPlacement = isValidPlacement(x, y, shipSize, false); // Перевірка валідності розміщення
                }
            }

            // Розміщення корабля на полі
            Ship newShip;
            newShip.x = x;
            newShip.y = y;
            ships.push_back(newShip); // Додавання корабля до вектору кораблів гравця
            if (direction == 'V')     // Якщо напрямок - вертикальний
            {
                markShipOnBoard(x, y, shipSize, true); // Позначення корабля на полі
            }
            else
            {
                markShipOnBoard(x, y, shipSize, false); // Позначення корабля на полі
            }
        }
    }

    void placeShips() // Метод для ручного розміщення кораблів
    {
        int numShips = 5;                  // Кількість кораблів
        int shipSizes[] = {5, 4, 3, 3, 2}; // Розміри кораблів

        cout << name << ", розставте ваші кораблі." << endl; // Повідомлення для гравця

        printBoard(); // Виведення ігрового поля

        for (int i = 0; i < numShips; ++i) // Цикл для розміщення кожного корабля
        {
            int shipSize = shipSizes[i]; // Поточний розмір корабля
            int x, y;                    // Координати корабля
            char direction;              // Напрямок корабля
            bool validPlacement = false; // Прапорець валідного розміщення корабля

            while (!validPlacement) // Повторення доти, доки не буде здійснено валідне розміщення
            {
                cout << "Введіть координати (x, y) для верхньої лівої вершини корабля розміром " << shipSize << ": ";
                if (!(cin >> x >> y)) // Перевірка правильності введених координат
                {
                    cout << "Недійсні координати! Спробуйте ще раз." << endl;
                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    continue;
                }
                x--;
                y--;

                do
                {
                    cout << "Виберіть напрямок розташування корабля (V - вертикально, H - горизонтально): ";
                    cin >> direction; // Вибір напрямку корабля
                    if (direction != 'V' && direction != 'v' && direction != 'H' && direction != 'h')
                    {
                        cout << "Недопустимий напрямок! Спробуйте ще раз." << endl;
                        cin.clear();
                        cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    }
                } while (direction != 'V' && direction != 'v' && direction != 'H' && direction != 'h');

                if (direction == 'V' || direction == 'v') // Якщо напрямок - вертикальний
                {
                    validPlacement = isValidPlacement(x, y, shipSize, true); // Перевірка валідності розміщення
                }
                else if (direction == 'H' || direction == 'h') // Якщо напрямок - горизонтальний
                {
                    validPlacement = isValidPlacement(x, y, shipSize, false); // Перевірка валідності розміщення
                }

                if (!validPlacement) // Якщо розміщення не валідне
                {
                    cout << "Недопустиме розташування! Спробуйте ще раз." << endl; // Повідомлення про неправильне розміщення
                }
            }

            // Розміщення корабля на полі
            Ship newShip;
            newShip.x = x;
            newShip.y = y;
            ships.push_back(newShip);                 // Додавання корабля до вектору кораблів гравця
            if (direction == 'V' || direction == 'v') // Якщо напрямок - вертикальний
            {
                markShipOnBoard(x, y, shipSize, true); // Позначення корабля на полі
            }
            else
            {
                markShipOnBoard(x, y, shipSize, false); // Позначення корабля на полі
            }

            printBoard(); // Виведення ігрового поля
        }
    }

    bool isValidPlacement(int x, int y, int shipSize, bool isVertical) // Метод для перевірки валідності розміщення корабля
    {
        for (int i = 0; i < shipSize; ++i) // Перевірка кожної клітинки корабля
        {
            for (int dx = -1; dx <= 1; ++dx)
            {
                for (int dy = -1; dy <= 1; ++dy)
                {
                    int nx = x + (isVertical ? i : 0) + dx;                                               // Нова координата x
                    int ny = y + (isVertical ? 0 : i) + dy;                                               // Нова координата y
                    if (nx >= 0 && nx < BOARD_SIZE && ny >= 0 && ny < BOARD_SIZE && board[nx][ny] != ' ') // Перевірка в межах поля та наявності кораблів
                    {
                        return false; // Розміщення не валідне
                    }
                }
            }

            if (isVertical && x + shipSize >= BOARD_SIZE) // Якщо вертикальне розміщення та корабель виходить за межі поля
            {
                return false; // Розміщення не валідне
            }
            if (!isVertical && y + shipSize >= BOARD_SIZE) // Якщо горизонтальне розміщення та корабель виходить за межі поля
            {
                return false; // Розміщення не валідне
            }
        }

        return true; // Розміщення валідне
    }

    void markShipOnBoard(int x, int y, int shipSize, bool isVertical) // Метод для позначення корабля на полі
    {
        if (isVertical) // Якщо вертикальне розміщення
        {
            for (int i = 0; i < shipSize; ++i) // Проходження по кожній клітинці корабля
            {
                board[x + i][y] = 'O'; // Позначення клітинки корабля на полі
            }
        }
        else // Якщо горизонтальне розміщення
        {
            for (int i = 0; i < shipSize; ++i) // Проходження по кожній клітинці корабля
            {
                board[x][y + i] = 'O'; // Позначення клітинки корабля на полі
            }
        }
    }

    void printBoard(bool hideShips = false) // Метод для виведення ігрового поля
    {
        cout << "Поле гравця " << name << ":" << endl; // Повідомлення про виведення поля гравця
        cout << "   ";
        for (int i = 0; i < BOARD_SIZE; ++i)
        {
            cout << i + 1 << " ";
        }
        cout << endl;
        for (int i = 0; i < BOARD_SIZE; ++i)
        {
            cout << i + 1 << " |";
            for (int j = 0; j < BOARD_SIZE; ++j)
            {
                if (hideShips && board[i][j] == 'O') // Якщо потрібно приховати кораблі
                {
                    cout << ' ' << "|";
                }
                else
                {
                    cout << board[i][j] << "|";
                }
            }
            cout << endl;
        }
    }

    string getName() const // Метод для отримання імені гравця
    {
        return name; // Повернення імені гравця
    }

    vector<vector<char>> &getBoard() // Метод для отримання ігрового поля гравця
    {
        return board; // Повернення ігрового поля гравця
    }

    vector<Ship> &getShips() // Метод для отримання кораблів гравця
    {
        return ships; // Повернення кораблів гравця
    }
};

class Game // Клас, що представляє гру
{
private:
    Player player;                 // Об'єкт класу Player для гравця
    Player computer;               // Об'єкт класу Player для комп'ютера
    ofstream file;                 // Файл для запису результатів гри
    vector<vector<bool>> attacked; // Матриця, що відстежує попередні атаки
    vector<pair<int, int>> hits;   // Вектор успішних попадань
public:
    Game(string playerName) : player(playerName), computer("Компʼютер") // Конструктор класу
    {
        attacked.resize(BOARD_SIZE, vector<bool>(BOARD_SIZE, false)); // Ініціалізація матриці attacked
    }

    void start() // Метод для початку гри
    {
        player.placeShips();                // Розстановка кораблів гравця
        computer.placeShipsAutomatically(); // Автоматична розстановка кораблів комп'ютера

        int difficulty; // Рівень складності
        cout << "Виберіть рівень складності (1 - легкий, 2 - складний): ";
        cin >> difficulty; // Введення рівня складності

        while (true) // Нескінченний цикл гри
        {
            player.printBoard();        // Виведення ігрового поля гравця
            playerAttack();             // Хід гравця
            string winner = checkWin(); // Перевірка переможця
            if (!winner.empty())        // Якщо є переможець
            {
                cout << "Вітаю, " << winner << ", ви перемогли!" << endl; // Повідомлення про перемогу
                recordResult(winner, difficulty);                         // Запис результату гри
                break;                                                    // Завершення гри
            }

            computer.printBoard(true); // Виведення ігрового поля комп'ютера з прихованими кораблями
            if (difficulty == 1)       // Якщо рівень складності легкий
            {
                computerAttack(); // Хід комп'ютера
            }
            else // Якщо рівень складності складний
            {
                computerAttackAdvanced(); // Хід комп'ютера з покращеною стратегією
            }
            winner = checkWin(); // Перевірка переможця
            if (!winner.empty()) // Якщо є переможець
            {
                cout << winner << " переміг!" << endl; // Повідомлення про перемогу
                recordResult(winner, difficulty);      // Запис результату гри
                break;                                 // Завершення гри
            }
        }

        displayRating(); // Виведення рейтингу
    }

    void playerAttack(){ // Метод для ходу гравця
        int x, y;                // Координати атаки
        bool validInput = false; // Прапорець валідного введення

        while (!validInput) // Повторення доти, доки введення не буде валідним
        {
            cout << "Ваш хід. Введіть координати для атаки: ";
            if (!(cin >> x >> y) || x < 1 || x > BOARD_SIZE || y < 1 || y > BOARD_SIZE) // Перевірка правильності введених координат
            {
                cout << "Неправильні координати! Спробуйте ще раз." << endl; // Повідомлення про неправильні координати
                cin.clear();                                                 // Очищення флагів стану потоку вводу
                cin.ignore(numeric_limits<streamsize>::max(), '\n');         // Ігнорування введених символів до кінця рядка
            }
            else
            {
                validInput = true; // Введення валідне
            }
        }

        x--;
        y--; // Корекція координат

        vector<vector<char>> &compBoard = computer.getBoard(); // Отримання ігрового поля комп'ютера
        if (compBoard[x][y] == 'O')                            // Якщо гравець попав
        {
            cout << "Ви попали!" << endl; // Повідомлення про попадання
            compBoard[x][y] = 'X';        // Позначення попадання на полі комп'ютера
        }
        else
        {
            cout << "Ви промахнулись." << endl; // Повідомлення про промах
            if (compBoard[x][y] != 'X')
            {
                compBoard[x][y] = '-'; // Позначення промаху на полі комп'ютера
            }
        }
    }

    void computerAttack() // Метод для ходу комп'ютера
    {
        srand(time(NULL));           // Ініціалізація генератора випадкових чисел
        int x = rand() % BOARD_SIZE; // Випадкова координата x
        int y = rand() % BOARD_SIZE; // Випадкова координата y

        // cout << "Хід комп'ютера. Координати для атаки: " << x + 1 << ", " << y + 1 << endl; // Display computer's attack coordinates

        vector<vector<char>> &playerBoard = player.getBoard(); // Отримання ігрового поля гравця
        if (playerBoard[x][y] == 'O')                          // Якщо комп'ютер попав
        {
            cout << "Компʼютер попав у вас!" << endl; // Повідомлення про попадання
            playerBoard[x][y] = 'X';                  // Позначення попадання на полі гравця
        }
        else
        {
            cout << "Компʼютер промахнувся." << endl; // Повідомлення про промах
            if (playerBoard[x][y] != 'X')
            {
                playerBoard[x][y] = '-'; // Позначення промаху на полі гравця
            }
        }
    }

    void computerAttackAdvanced() // Метод для ходу комп'ютера з покращеною стратегією
    {
        srand(time(NULL));        // Ініціалізація генератора випадкових чисел
        int x, y;                 // Координати атаки
        bool validAttack = false; // Флаг, що позначає правильність атаки

        while (!validAttack) // Поки атака не буде виконана правильно
        {
            if (!hits.empty()) // Якщо є успішні попадання
            {
                // Якщо були успішні попадання, атакувати сусідні клітинки
                tie(x, y) = hits.back();                                                // Отримати координати останнього успішного попадання
                vector<pair<int, int>> directions = {{-1, 0}, {1, 0}, {0, -1}, {0, 1}}; // Список можливих напрямків атаки
                for (auto [dx, dy] : directions)                                        // Проходження по кожному напрямку
                {
                    int nx = x + dx, ny = y + dy;                                                      // Координати нової клітинки для атаки
                    if (nx >= 0 && nx < BOARD_SIZE && ny >= 0 && ny < BOARD_SIZE && !attacked[nx][ny]) // Перевірка на виход за межі поля та на те, чи не була атакована ця клітинка раніше
                    {
                        x = nx;
                        y = ny;             // Оновлення координат атаки
                        validAttack = true; // Позначення атаки як правильної
                        break;              // Виход з циклу
                    }
                }
                if (!validAttack)
                {
                    // Якщо всі сусідні клітинки були атаковані, видалити останнє успішне попадання
                    hits.pop_back(); // Видалення останнього успішного попадання
                }
            }
            else
            {
                // Якщо немає успішних попадань, вибрати випадкову клітинку
                x = rand() % BOARD_SIZE; // Випадкова координата x
                y = rand() % BOARD_SIZE; // Випадкова координата y
                if (!attacked[x][y])     // Перевірка на те, чи не була атакована ця клітинка раніше
                {
                    validAttack = true; // Позначення атаки як правильної
                }
            }
        }

        // Позначення цієї клітинки як атакованої
        attacked[x][y] = true;

        // cout << "Хід комп'ютера. Координати для атаки: " << x + 1 << ", " << y + 1 << endl; // Виведення координат атаки комп'ютера

        vector<vector<char>> &playerBoard = player.getBoard(); // Отримання ігрового поля гравця
        if (playerBoard[x][y] == 'O')                          // Якщо комп'ютер попав
        {
            cout << "Комп'ютер попав!" << endl; // Повідомлення про попадання
            playerBoard[x][y] = 'X';            // Позначення попадання на полі гравця
            hits.push_back({x, y});             // Додавання успішного попадання
        }
        else
        {
            cout << "Комп'ютер промахнувся." << endl; // Повідомлення про промах
            if (playerBoard[x][y] != 'X')
            {
                playerBoard[x][y] = '-'; // Позначення промаху на полі гравця
            }
        }
    }

    string checkWin() // Перевірка переможця гри
    {

        vector<Ship> &playerShips = player.getShips(); // Отримання кораблів гравця
        vector<Ship> &compShips = computer.getShips(); // Отримання кораблів комп'ютера

        bool playerHasShips = false;   // Прапорець для перевірки, чи залишилися кораблі у гравця
        bool computerHasShips = false; // Прапорець для перевірки, чи залишилися кораблі у комп'ютера

        for (const auto &ship : playerShips) // Проходження по кораблях гравця
        {
            if (player.getBoard()[ship.x][ship.y] == 'O') // Якщо на полі гравця є незнищений корабель
            {
                playerHasShips = true; // Встановлення прапорця
                break;                 // Вихід з циклу
            }
        }

        for (const auto &ship : compShips) // Проходження по кораблях комп'ютера
        {
            if (computer.getBoard()[ship.x][ship.y] == 'O') // Якщо на полі комп'ютера є незнищений корабель
            {
                computerHasShips = true; // Встановлення прапорця
                break;                   // Вихід з циклу
            }
        }

        if (!playerHasShips) // Якщо у гравця немає кораблів
        {
            return computer.getName(); // Повернути ім'я комп'ютера як переможця
        }
        else if (!computerHasShips) // Якщо у комп'ютера немає кораблів
        {
            return player.getName(); // Повернути ім'я гравця як переможця
        }
        else // Якщо немає переможця (гра продовжується)
        {
            return ""; // Повернути порожній рядок
        }
    }

    void displayRating() // Відображення рейтингу гравців
    {
        ifstream file("scores.txt"); // Відкриття файлу з результатами
        if (!file)                   // Перевірка, чи файл відкрито успішно
        {
            cerr << "Помилка під час відкриття файлу" << endl; // Виведення повідомлення про помилку
            return;                                            // Вихід з методу
        }

        cout << "Рейтинг:" << endl; // Виведення заголовку рейтингу
        string line;                // Рядок для зчитування кожного рядка з файлу
        while (getline(file, line)) // Зчитування кожного рядка з файлу
        {
            cout << line << endl; // Виведення рядка
        }
        cout << endl; // Виведення порожнього рядка (для відділення від інших виведених даних)
        file.close(); // Закриття файлу
    }

    void recordResult(const string &winner, const int &difficulty) // Запис результату гри до файлу
    {
        ofstream file("scores.txt", ios::app); // Відкриття файлу для дозапису
        ifstream Readfile("scores.txt");       // Відкриття файлу для зчитування
        int round = 1;                         // Номер раунду, за замовчуванням 1
        if (!file)                             // Перевірка, чи файл відкрито успішно
        {
            cerr << "Помилка під час відкриття файлу" << endl; // Виведення повідомлення про помилку
            return;                                            // Вихід з методу
        }

        string line;                    // Рядок для зчитування кожного рядка з файлу
        while (getline(Readfile, line)) // Зчитування кожного рядка з файлу
        {
            if (line.find("Гра: ") != string::npos) // Перевірка, чи знайдено запис про раунд
            {
                round++; // Збільшення номеру раунду
            }
        }

        file << "Гра: " << round << endl; // Запис номеру раунду до файлу
        string STRdifficulty;             // Рядок для представлення рівня складності
        if (difficulty == 1)
        {
            STRdifficulty = "Легкий рівень"; // Якщо рівень складності 1, то "Легкий рівень"
        }
        else
        {
            STRdifficulty = "Складний рівень"; // Якщо рівень складності не 1, то "Складний рівень"
        }

        file << "Рівень складності: " << STRdifficulty << endl; // Запис рівня складності до файлу

        if (winner == player.getName()) // Якщо переможець гравець
        {
            file << player.getName() << ": Перемога" << endl; // Запис перемоги гравця до файлу
        }
        else if (winner == computer.getName()) // Якщо переможець комп'ютер
        {
            file << player.getName() << ": Поразка" << endl; // Запис поразки гравця до файлу
        }

        file.close(); // Закриття файлу
    }
};

int main()
{
    string playerName;             // Змінна для збереження імені гравця
    cout << "Введіть ваше ім'я: "; // Виведення повідомлення для введення імені
    cin >> playerName;             // Зчитування імені з консолі

    Game game(playerName); // Створення об'єкту гри з введеним іменем гравця
    game.start();          // Запуск гри

    return 0; // Повернення нульового значення для позначення успішного завершення програми
}
