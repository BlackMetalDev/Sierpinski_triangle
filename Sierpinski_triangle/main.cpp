/*
 Построение треугольника Серпинского из точек методом хаоса.
 Вершины треугольника располагаются случайным образом.
 */

#include <SFML/Graphics.hpp>
#include <cmath>
#include <ctime>

using namespace sf;

const int numberOfPoints = 50000; // Число точек, из которых будет состоять изображение.

class InnerPoint
{
protected:
    int x = 0;
    int y = 0;

public:
    static int previousX;
    static int previousY;

    InnerPoint()
    {
    }

    InnerPoint(int randomBasePointX, int randomBasePointY)
    {
        // Расчет координаты X следующей точки.
        if (previousX < randomBasePointX)
            x = abs((previousX - randomBasePointX) / 2) + previousX;
        else
            x = abs((previousX - randomBasePointX) / 2) + randomBasePointX;

        // Расчет координаты Y следующей точки.
        if (previousY < randomBasePointY)
            y = abs((previousY - randomBasePointY) / 2) + previousY;
        else
            y = abs((previousY - randomBasePointY) / 2) + randomBasePointY;
    }

    int GetX()
    {
        previousX = x;
        return x;
    }

    int GetY()
    {
        previousY = y;
        return y;
    }
};

int InnerPoint::previousX = 0;
int InnerPoint::previousY = 0;

class BasePoint : protected InnerPoint
{
public:
    BasePoint()
    {
        x = (rand() % 750 + 100);
        y = (rand() % 750 + 100);
    }

    int GetX()
    {
        return x;
    }

    int GetY()
    {
        return y;
    }
};

void DrawPoints(RenderWindow& window, VertexArray& basePoints,
    VertexArray& innerPoints)
{
    /* Создание трех базовых точек (вершин главного треугольника) со
    случайными координатами. */
    BasePoint basePointsArray[3] = {};

    for (int i = 0; i < 3; i++)
    {
        basePoints[i].position = Vector2f(basePointsArray[i].GetX(),
            basePointsArray[i].GetY());
    }

    // Создание массивов координат X и Y внутренних точек.
    InnerPoint *pointsArray = new InnerPoint[numberOfPoints];
    int *arrX = new int[numberOfPoints];
    int *arrY = new int[numberOfPoints];

    for (int i = 0; i < numberOfPoints; i++)
    {
        int randomIndex = rand() % 3;
        pointsArray[i] = InnerPoint(basePointsArray[randomIndex].GetX(),
            basePointsArray[randomIndex].GetY());
        arrX[i] = pointsArray[i].GetX();
        arrY[i] = pointsArray[i].GetY();
    }

    /* Исправление выхода первых внутренних точек
    за пределы базового треугольника. */
    for (int i = 0; i < 10; i++)
    {
        arrX[i] = basePointsArray[0].GetX();
        arrY[i] = basePointsArray[0].GetY();
    }

    // Присвоение полученных координат внутренним точкам.
    for (int i = 0; i < numberOfPoints; i++)
    {
        innerPoints[i].position = Vector2f(arrX[i], arrY[i]);
    }

    // Очистка графического окна.
    window.clear();

    // Обновление базовых точек.
    window.draw(basePoints);

    // Обновление внутренних точек.
    window.draw(innerPoints);
    void PrintText(RenderWindow & window);
    PrintText(window);

    // Отрисовка обновленных точек.
    window.display();

    // Освобождение динамической памяти
    delete[] pointsArray;
    delete[] arrX;
    delete[] arrY;
}

void PrintText(RenderWindow& window)
{
    // Отрисовка текста в графическом окне.
    Font font;
    font.loadFromFile("font/arial.ttf");
    Text text(L"Нажмите ПРОБЕЛ чтобы обновить изображение", font, 30);
    text.setPosition(155, 10);
    window.draw(text);
}

void MainLoop(RenderWindow& window, VertexArray& basePoints,
    VertexArray& innerPoints)
{
    // Основной цикл.
    while (window.isOpen())
    {
        Event event;

        // Обработка событий в цикле.
        while (window.pollEvent(event))
        {
            // Проверка типа события.

                // Закрытие окна или нажатие клавиши "Escape".
            if (event.type == Event::Closed ||
                (event.type == Event::KeyPressed && event.key.code == Keyboard::Escape))
                window.close();

            // Нажатие клавиши "Пробел".
            if (event.type == Event::KeyPressed && event.key.code == Keyboard::Space)
            {
                DrawPoints(window, basePoints, innerPoints);
            }
        }
    }
}

int main()
{
    srand(time(NULL));

    // Создание графического окна.
    RenderWindow window(VideoMode(1000, 900), "Fractal");
    window.setFramerateLimit(30);

    /* Создание массива объектов - базовых точек (вершин
    главного треугольника). */
    VertexArray basePoints(Points, 3);

    // Создание массива объектов - внутренних точек.
    VertexArray innerPoints(Points, numberOfPoints);

    /* Функция DrawPoints передает случайные координаты базовым
    точкам. Опираясь на базовые точки, рассчитывает координаты
    внутренних точек и отрисовывает их в графическом окне. */
    DrawPoints(window, basePoints, innerPoints/*, numberOfPoints*/);

    // Отрисовка текста.
    PrintText(window);

    // Запуск основного цикла.
    MainLoop(window, basePoints, innerPoints/*, numberOfPoints */);

    return 0;
}
