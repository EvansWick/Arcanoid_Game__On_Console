#include <iostream>
#include <conio.h> // для скану кнопок
#include <algorithm> // Для сортування вектора 
#include <windows.h> // для затримки та функцій консолі
#include <vector>
#include <sstream> 
#include <utility> // для std::pair
#include <cstdlib> // Для rand()
#include <ctime>   // Для time()

// Використовувати using namespace std; у заголовних файлах або великих проектах не рекомендується,
// але для невеликих прикладів це прийнятно.
using namespace std;

// Структура для передачі аргументів, її можна буде розширити
struct MoveArgs {
    int fieldX1;
    int fieldX2;
    int fieldY1;
    int fieldY2;

    // Ці поля поки не використовуються, але можуть знадобитися
    // vector<int> platformX;
    // vector<int> platformY;
    // int bollX;
    // int bollY;
};

class Ball {
public:
    Ball(char c) : ballSymbol(c), x(0), y(0) {}
    Ball() : ballSymbol('*'), x(0), y(0) {}
    Ball(char c, int X, int Y)
    {
        ballSymbol = c;
        this->x = X;
        this->y = Y;
        directBall = ballToDown; // Початковий напрямок м'яча
    }
    char ballSymbol;

    int x;
    int y;

    enum BallTo {
        ballToUp = 0,
        ballToLeftUp,
        ballToRightUp,
        ballToDown,
        ballToDownRight,
        ballToDownLeft
    };
    BallTo directBall;
};

class ReLoadImage {
public:
    // Конструктор
    ReLoadImage(MoveArgs& direct, Ball* ball) {
        this->HEIGHT = direct.fieldY2 - direct.fieldY1;
        this->WIDTH = direct.fieldX2 - direct.fieldX1;

        // Ініціалізуємо карту один раз у конструкторі
        map.assign(HEIGHT, vector<char>(WIDTH, ' '));

        // Символ платформи
        baseChar = '='; // Визначимо символ платформи

        // Ініціалізація координат платформи
        baseCord.assign(3, pair<int, int>(0, 0)); // Ініціалізуємо 3 пари нулями

        // Встановлюємо початкові координати платформи
        // Це центр знизу
        baseCord[1].first = WIDTH / 2;
        baseCord[1].second = HEIGHT - 3; // На 3 рядки вище нижнього краю
        baseCord[0].first = baseCord[1].first - 1; // Ліва частина
        baseCord[0].second = baseCord[1].second;
        baseCord[2].first = baseCord[1].first + 1; // Права частина
        baseCord[2].second = baseCord[1].second;

        circle = ball;

        // Заповнення масиву з перешкодами

        int numRows = 20;
        int obstaclesPerRow = 3;
        char obstacleChar = '#';
        int startY = 4;
        int startX = 2;
        int xSpacing = 2;
        int ySpacing = 1;

        for (int i = 0; i < numRows; ++i) {
            std::vector<std::pair<char, std::pair<int, int>>> currentRow;
            for (int j = 0; j < obstaclesPerRow; ++j) {
                int obstacleX = startX + j * xSpacing;
                int obstacleY = startY + i * ySpacing;
                currentRow.push_back({ obstacleChar, {obstacleX, obstacleY} });
            }
            obstacles.push_back(currentRow); // Додаємо заповнений рядок
        }
    }

    // Функція для переміщення курсору консолі
    void goToxy(int x, int y) {
        COORD pos = { (short)x, (short)y }; // Приведення до short
        HANDLE output = GetStdHandle(STD_OUTPUT_HANDLE);
        SetConsoleCursorPosition(output, pos);
    }

    // Функції для руху платформи
    void baseToLeft() {
        baseCord[0].first--;
        baseCord[1].first--;
        baseCord[2].first--;
    }

    void baseToRight() {
        baseCord[0].first++;
        baseCord[1].first++;
        baseCord[2].first++;

    }

    void baseToUp() {
        baseCord[0].second--;
        baseCord[1].second--;
        baseCord[2].second--;
    }

    void baseToDown() {
        baseCord[0].second++;
        baseCord[1].second++;
        baseCord[2].second++;
    }


    // Змінні класу
    int HEIGHT;
    int WIDTH;
    char baseChar; // Символ, яким відмальовується платформа
    vector<std::pair<int, int>> baseCord; // Координати кожного сегмента платформи
    Ball* circle;
    vector<vector<std::pair<char, pair<int, int>>>> obstacles; // Вектор з векторами перешкод

    vector <vector<char>> map; // Двовимірний вектор для зберігання карти поля

    // Функція для оновлення вмісту карти (має викликатися перед кожним виведенням)
    void setMap() {
        // Заповнюємо карту пробілами для очищення попереднього кадру
        for (int i = 0; i < HEIGHT; ++i) {
            for (int j = 0; j < WIDTH; ++j) {
                map[i][j] = ' ';
            }
        }

        // Відрисовка рамок поля
        char fieldSymbolX = '='; // Горизонтальні рамки
        char fieldSymbolY = 'I'; // Вертикальні рамки

        for (int i = 0; i < HEIGHT; i++) {
            for (int j = 0; j < WIDTH; j++) {
                if (i == 0 || i == HEIGHT - 1) { // Верхня та нижня рамки
                    map[i][j] = fieldSymbolX;
                }
            }
            map[i][0] = fieldSymbolY; // Ліва рамка
            map[i][WIDTH - 1] = fieldSymbolY; // Права рамка
        }

        // Відмальовуємо м'яч, якщо він в межах поля
        if (circle->x > 0 && circle->x < WIDTH - 1 && circle->y > 0 && circle->y < HEIGHT - 1) {
            map[circle->y][circle->x] = circle->ballSymbol;
        }


        // Відрисовка платформи
        for (const auto& point : baseCord) {
            // Перевіряємо, чи координати платформи знаходяться в межах поля
            if (point.first >= 0 && point.first < WIDTH &&
                point.second >= 0 && point.second < HEIGHT) {
                map[point.second][point.first] = baseChar; // Відмальовуємо платформу
            }
        }

        // Відрисовка перешкод
        for (int i = 0; i < obstacles.size(); i++) {
            for (int j = 0; j < obstacles[i].size(); j++) {
                map[obstacles[i][j].second.first][obstacles[i][j].second.second] = obstacles[i][j].first;
            }
        }
    }

    void ballMoveTo() {
        // Поточні координати м'яча
        int nextX = circle->x;
        int nextY = circle->y;

        // Зміна координат відповідно до поточного напрямку
        switch (circle->directBall) {
        case Ball::ballToUp:
            nextY--;
            break;
        case Ball::ballToLeftUp:
            nextX--;
            nextY--;
            break;
        case Ball::ballToRightUp:
            nextX++;
            nextY--;
            break;
        case Ball::ballToDown:
            nextY++;
            break;
        case Ball::ballToDownRight:
            nextX++;
            nextY++;
            break;
        case Ball::ballToDownLeft:
            nextX--;
            nextY++;
            break;
        }

        // --- Обробка колізій ---

        // Колізія з верхньою стіною (Y=0 або Y=1, залежно від того, як малюється межа)
        if (nextY <= 0) { // М'яч намагається зайти за верхню межу або на неї
            if (circle->directBall == Ball::ballToUp) {
                circle->directBall = Ball::ballToDown;
            }
            else if (circle->directBall == Ball::ballToLeftUp) {
                circle->directBall = Ball::ballToDownLeft;
            }
            else if (circle->directBall == Ball::ballToRightUp) {
                circle->directBall = Ball::ballToDownRight;
            }
            nextY = 1; // Залишаємо м'яч на 1 одиниці від краю
        }

        // Колізія з лівою стіною (X=0 або X=1)
        if (nextX <= 0) { // М'яч намагається зайти за ліву межу
            if (circle->directBall == Ball::ballToLeftUp) {
                circle->directBall = Ball::ballToRightUp;
            }
            else if (circle->directBall == Ball::ballToDownLeft) {
                circle->directBall = Ball::ballToDownRight;
            }
            nextX = 1; // Залишаємо м'яч на 1 одиниці від краю
        }

        // Колізія з правою стіною (X=WIDTH-1 або X=WIDTH-2)
        if (nextX >= WIDTH - 1) { // М'яч намагається зайти за праву межу
            if (circle->directBall == Ball::ballToRightUp) {
                circle->directBall = Ball::ballToLeftUp;
            }
            else if (circle->directBall == Ball::ballToDownRight) {
                circle->directBall = Ball::ballToDownLeft;
            }
            nextX = WIDTH - 2; // Залишаємо м'яч на 1 одиниці від краю
        }

        // Колізія з платформою
        // Перевіряємо, чи наступна позиція м'яча по Y збігається з Y-координатою платформи
        // та чи є збіг по X-координаті з будь-яким сегментом платформи
        if (nextY == baseCord[1].second) {
            bool hitPlatform = false;
            for (int i = 0; i < baseCord.size(); ++i) {
                if (nextX == baseCord[i].first) {
                    // М'яч зіткнувся з платформою
                    hitPlatform = true;
                    // Змінюємо напрямок м'яча залежно від сектора платформи
                    if (i == 0) { // Лівий сегмент платформи
                        circle->directBall = Ball::ballToLeftUp;
                    }
                    else if (i == 1) { // Центральний сегмент
                        circle->directBall = Ball::ballToUp;
                    }
                    else if (i == 2) { // Правий сегмент
                        circle->directBall = Ball::ballToRightUp;
                    }
                    break;
                }
            }
            // Важливо: якщо м'яч потрапив на рівень платформи, але не над нею,
            // він продовжує рух вниз. Це вирішується перевіркою `hitPlatform`.
            // Якщо м'яч просто перетинає рівень платформи, але не в зоні зіткнення,
            // він має продовжувати падати.
            // Якщо він зіткнувся з платформою, то вже змінили напрямок вище, 
            // і він не повинен "пролітати" далі вниз, тому повертаємося.
            if (hitPlatform) {
                circle->x = nextX;
                circle->y = nextY;
                return; // Завершуємо функцію, оскільки напрямок вже змінено
            }
        }


        // Колізія з нижньою межею (програш або втрата життя)
        if (nextY >= HEIGHT - 1) {
            // М'яч вилетів за межі поля знизу - програш
            circle->y = HEIGHT / 2; // Повертаємо на середину
            circle->x = WIDTH / 2;
            circle->directBall = (rand() % 2 == 0) ? Ball::ballToLeftUp : Ball::ballToRightUp; // Випадковий напрямок вгору
            return; // Завершуємо функцію, м'яч перезапущено
        }

        // Оновлення координат м'яча після всіх перевірок
        circle->x = nextX;
        circle->y = nextY;
    }

    // Функція для виведення карти в консоль
    void viewMap() {
        for (int i = 0; i < HEIGHT; i++) {
            cout << '\t'; // Можливо, це не потрібно, якщо гра розрахована на весь екран
            for (int j = 0; j < this->WIDTH; j++) {
                std::cout << map[i][j];
            }
            std::cout << '\n'; // Додаємо перехід на новий рядок після кожного рядка карти
        }
    }

    void checkObstacles() {  // Перевірка зіткнення з перешкодами
        bool ballIn = false;
        //if (circle->y < HEIGHT / 2) {
            for (int i = 0; i < obstacles.size(); i++) {
                for (int j = 0; j < obstacles[i].size(); j++) {
                    if (obstacles[i][j].second.first == circle->y && obstacles[i][j].second.second == circle->x && obstacles[i][j].first != ' ') {
                        obstacles[i][j].first = ' ';
                        switch (circle->directBall)
                        {
                        case Ball::ballToUp:
                            circle->directBall = Ball::ballToDown;
                            
                            break;
                        case Ball::ballToRightUp:
                            circle->directBall = Ball::ballToDownRight;
                            
                            break;
                        case Ball::ballToLeftUp:
                            circle->directBall = Ball::ballToDownLeft;
                            break;
                        case Ball::ballToDownRight:
                            circle->directBall = Ball::ballToRightUp;
                            break;
                        case Ball::ballToDownLeft:
                            circle->directBall = Ball::ballToLeftUp;
                            break;
                        default:
                            break;
                        }
                    }
                }
            }
        //}
    }
};

enum To {
    Left = 0,
    Right,
    Stop,
    Up,
    Down,
};

class ProxyReload {
public:

    ProxyReload(ReLoadImage& mainImage) : realImage(mainImage) {}

    void baseTo(To direct) {
        if (direct == To::Left)
            if (realImage.baseCord[0].first > 1) realImage.baseToLeft();
        if (direct == To::Right)
            if (realImage.baseCord[2].first < realImage.WIDTH - 2) realImage.baseToRight();
        if (direct == To::Up)
            if (realImage.baseCord[1].second > 1) realImage.baseToUp();
        if (direct == To::Down)
            if (realImage.baseCord[2].second < realImage.HEIGHT - 2) realImage.baseToDown();
    }
    ReLoadImage& realImage;

};

int main() {
    srand(static_cast<unsigned int>(time(0))); // Ініціалізація генератора випадкових чисел

    MoveArgs arg;
    arg.fieldX1 = 0;
    arg.fieldX2 = 29; // Ширина
    arg.fieldY1 = 0;
    arg.fieldY2 = 22; // Висота


    To direction = To::Stop;
    // Початкова позиція м'яча: по центру над платформою, рухається вгору
    Ball circle('O', arg.fieldX2 / 2, arg.fieldY2 - 4);
    circle.directBall = (rand() % 2 == 0) ? Ball::ballToLeftUp : Ball::ballToRightUp; // Випадковий старт вгору

    ReLoadImage image(arg, &circle);
    ProxyReload proxy(image);

    bool Game = true;
    while (Game) {
        // Очищаємо екран (переміщуємо курсор на початок)
        image.goToxy(0, 0);

        // Обробка вводу з клавіатури
        if (_kbhit()) {
            char ch = _getch();
            switch (tolower(ch)) {
            case 'a':
                direction = To::Left;
                break;
            case 'd':
                direction = To::Right;
                break;
            case 'q':
                Game = false;
                break;
            default: // Якщо натиснута інша клавіша, зупиняємо рух платформи
                direction = To::Stop;
                break;
            }
        }

        proxy.baseTo(direction); // Рух платформи
        image.ballMoveTo();      // Рух м'яча
        image.checkObstacles();        // Перевірка зіткнень

        // Оновлюємо карту перед кожним виведенням!
        image.setMap();
        image.viewMap();

        Sleep(80); // Затримка 80 мс для кращої візуалізації
    }


    system("cls"); // Очистити консоль в кінці
    cout << "Гра завершена!" << endl;
    return 0;
}