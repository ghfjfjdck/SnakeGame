#include <iostream>
#include <conio.h>
#include <windows.h>
#include <vector>
#include <cstdlib>
#include <locale>

// Структура для "вагончика" змейки
int speed = 150;
struct Snake {
    int x, y;        // Координаты вагончика (горизонталь и вертикаль)
    int cordEatX, cordEatY; // Координаты еды
    int Score = 0;   // Счет игры
    Snake* next;      // Указатель на следующий вагончик
    bool Eat = true; // Флаг "еда съедена"

    // Конструктор, для создания нового вагончика змейки
    Snake(int vx, int vy) : x(vx), y(vy), next(nullptr) {}

    // Метод для движения змейки
    void go(int goX, int goY) {
        int hX = x; // Сохраняем старые координаты
        int hY = y;

        x += goX;  // Двигаем голову
        y += goY;

        Snake* cur = next; // Начинаем с вагончика за головой
        while (cur != nullptr) {  // Пока есть вагончики
            int tempX = cur->x;   // Запоминаем координаты текущего вагончика
            int tempY = cur->y;

            cur->x = hX;          // Перемещаем текущий вагончик на место предыдущего
            cur->y = hY;

            hX = tempX;           // Обновляем "старые" координаты для следующего
            hY = tempY;

            cur = cur->next;      // Переходим к следующему вагончику
        }
    }

    // Метод для проверки, что змейка не врезалась
    bool CheckSnake(Snake* snake) {
        // Проверяем, не вышла ли голова за границы поля
        if (x < 0 || x >= 20 || y < 0 || y >= 20) {
            return true; // Если вышла, возвращаем "да, врезалась"
        }

        Snake* cur = next; // Проверяем, не врезалась ли голова в тело
        while (cur != nullptr) { // Пока есть вагончики
            if (cur->x == x && cur->y == y) {
                return true;    // Если врезалась, возвращаем "да, врезалась"
            }
            cur = cur->next;
        }

        // Если змейка съела еду
        if (cordEatX == x && cordEatY == y) {
            cur = snake; // Находим последний вагончик
            while (cur->next != nullptr) {
                cur = cur->next;
            }
            // Добавляем новый вагончик в конец
            cur->next = new Snake(cur->x, cur->y);
            Eat = true;  // Говорим, что еда съедена
            Score += 10; // Увеличиваем счет
            if(speed>30)
            speed -= 10;
        }
        return false;
    }

    // Метод для рисования игрового поля
    void drawField() {
        char field[20][20] = {}; // Создаем игровое поле

        // Если нужно создать новую еду
        if (Eat) {
            cordEatX = rand() % 19 + 1; // Случайная позиция еды
            cordEatY = rand() % 19 + 1;
            Eat = false; // Говорим, что еду больше не надо создавать
        }

        // Заполняем игровое поле ( '.' - пустота)
        for (int i = 0; i < 20; ++i) {
            for (int j = 0; j < 20; ++j) {
                if (j == cordEatX && i == cordEatY) {
                    field[i][j] = 'E';  // Рисуем еду
                }
                else {
                    field[i][j] = '_'; // Рисуем пустоту
                }
            }
        }

        Snake* cur = this;   // Начинаем с головы змейки
        while (cur != nullptr) {   // Пока есть вагончики
            // Рисуем змейку на поле ('O' - вагончик)
            if (cur->x >= 0 && cur->x < 20 && cur->y >= 0 && cur->y < 20) {
                field[cur->y][cur->x] = ' ';
            }
            cur = cur->next;       // Переходим к следующему вагончику
        }

        system("cls");       // Очищаем экран
        for (int i = 0; i < 20; ++i) { // Выводим поле
            for (int j = 0; j < 20; ++j) {
                std::cout << field[i][j] << ' ';
            }
            std::cout << std::endl;
        }
    }
};

Snake* snake; // Указатель на голову змейки
int main() {
    srand(time(0)); // Делаем случайные числа

    // Создаем начальную змейку из нескольких вагончиков
    snake = new Snake(5, 5);
    snake->next = new Snake(5, 4);


    int goX = 0;    // Направление движения по горизонтали
    int goY = 1;    // Направление движения по вертикали
    int lastGoX = goX; // Запоминаем прошлое направление
    int lastGoY = goY;

    while (true) { // Главный цикл игры
        // Проверяем нажатие клавиши
        if (_kbhit()) {
            int key = _getch(); // Получаем код нажатой клавиши
            int newGoX = goX;   // Новое направление по горизонтали
            int newGoY = goY;   // Новое направление по вертикали

            // Меняем направление на основе нажатой стрелки
            if (key == 72) {    // Вверх
                newGoX = 0;
                newGoY = -1;
            }
            else if (key == 80) { // Вниз
                newGoX = 0;
                newGoY = 1;
            }
            else if (key == 75) { // Влево
                newGoX = -1;
                newGoY = 0;
            }
            else if (key == 77) { // Вправо
                newGoX = 1;
                newGoY = 0;
            }
            // Если направление не обратное, запоминаем его
            if (newGoX != -lastGoX || newGoY != -lastGoY) {
                goX = newGoX;
                goY = newGoY;
            }
        }

        snake->go(goX, goY); // Двигаем змейку
        // Проверяем, не врезалась ли змейка
        if (snake->CheckSnake(snake)) {
            std::cout << "Game Over!" << std::endl;  // Если врезалась, выводим "Game Over!"
            std::cout << "Score:" << snake->Score << std::endl; // Выводим счет
            break;
        }

        snake->drawField(); // Рисуем игровое поле
        lastGoX = goX;
        lastGoY = goY;   // Запоминаем прошлое направление
        Sleep(speed);  
    }

    return 0; // Игра закончилась
}