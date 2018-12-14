// Даны два выпуклых многоугольника на плоскости. В первом n точек, во втором m.
// Определите, пересекаются ли они за O(n + m).
// Указание. Используйте сумму Минковского.
// Формат ввода.
// Первая строка содержит число n точек первого многоугольника. Затем идут n строчек с координатами
// точек первого многоугольника по часовой стрелке (координаты – действительные числа, double).
// Второй прямоугольник задается аналогично. n, m ≤ 80000.
// Формат вывода: YES/NO

#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

const double PI = 3.14159265;

struct Point
{
    double x;
    double y;

    Point(double x, double y): x(x), y(y) {};
    Point() {};
};

// чтение точки
istream& operator>>(istream& in, Point& p) {
    in >> p.x >> p.y;
    return in;
}

// p1 < p2, если она ниже (в сл. рав-ва - левее)
const bool operator<(const Point& p1, const Point& p2) {
    if (p1.y < p2.y) {
        return true;
    }
    if (p1.y == p2.y) {
        return p1.x < p2.x;
    }
    return false;
}

// сумма координат двух точек
Point operator+(const Point& p1, const Point& p2) {
    return Point(p1.x + p2.x, p1.y + p2.y);
}

// полярный угол вектора {p1, p2}
double PolarAngle(const Point& p1, const Point& p2) {
    double alpha = atan2(p2.y - p1.y, p2.x - p1.x);
    if (alpha < 0) {
        alpha += 2 * PI;
    }
    return alpha;
}

// преобразование многоугольника для дальнейшего вычисления суммы Минковского
void PolygonTransform(vector <Point>& polygon) {
    // поменяем обход на против часовой стрелки
    reverse(polygon.begin(), polygon.end());

    // найдем индекс самой нижней левой вершины
    vector <Point>::iterator min_point = min_element(polygon.begin(), polygon.end());
    int down_left_ind = distance(polygon.begin(), min_point);

    // делаем циклический двиг, чтобы лев. нижн. т-а была на позиции 0
    rotate(polygon.begin(), polygon.begin() + down_left_ind, polygon.end());

    // дублируем первые две вершины для дальнейшего вычисления суммы Минковского
    polygon.push_back(polygon[0]);
    polygon.push_back(polygon[1]);
}

// сумма Минковского двух многоугольников
vector <Point> MinkowskiAmount(const vector <Point>& polygon_p,
                               const vector <Point>& polygon_q) {
    // размеры без учета продублированных вершин
    int p_size = polygon_p.size() - 2;
    int q_size = polygon_q.size() - 2;

    vector <Point> answer;

    int ind_p = 0;
    int ind_q = 0;
    while (ind_p < p_size && ind_q < q_size) {
        answer.push_back(polygon_p[ind_p] + polygon_q[ind_q]);
        // полярные углы текущих ребер в многоугольниках p и q
        double angle_p = PolarAngle(polygon_p[ind_p], polygon_p[ind_p + 1]);
        double angle_q = PolarAngle(polygon_q[ind_q], polygon_q[ind_q + 1]);
        if (angle_p < angle_q) {
            ind_p++;
        } else {
            if (angle_p > angle_q) {
                ind_q++;
            } else {
                ind_p++;
                ind_q++;
            }
        }
    }
    answer.push_back(polygon_p[ind_p] + polygon_q[ind_q]);
    answer.push_back(answer[0]);

    return answer;
}

// лежит ли точка p в многоугольнике polygon
bool PointInPolygon(const Point& p, const vector <Point>& polygon) {
    for (int i = 0; i < polygon.size() - 1; i++) {
        Point a = polygon[i];
        Point b = polygon[i + 1];
        // если при движении по ребрам многоугольника пр. часовой стрелки
        // оказалось, что т-а p находится справа от ребра, то false
        if ((b.x - a.x) * (p.y - b.y) - (p.x - b.x) * (b.y - a.y) < 0) {
            return false;
        }
    }

    return true;
}

int main() {
    int n, m;  // кол-во вершин в многоугольниках p и q
    vector <Point> polygon_p;  // первый многоугольник p
    vector <Point> polygon_q;  // второй многоугольник q

    // чтение первого многоугольника
    cin >> n;
    for (int i = 0; i < n; i++) {
        Point p;
        cin >> p;
        polygon_p.push_back(p);
    }

    // чтение второго многоугольника
    cin >> m;
    for (int i = 0; i < m; i++) {
        Point p;
        cin >> p;
        polygon_q.push_back(p);
    }
    for (int i = 0; i < polygon_q.size(); i++) {
        polygon_q[i].x *= -1;
        polygon_q[i].y *= -1;
    }

    // подготовка многоугольников
    PolygonTransform(polygon_p);
    PolygonTransform(polygon_q);

    // вычисление суммы Минковского
    vector <Point> minkowski_amount = MinkowskiAmount(polygon_p, polygon_q);

    Point p(0.0, 0.0);
    string answer = PointInPolygon(p, minkowski_amount) ? "YES" : "NO";
    cout << answer;
}