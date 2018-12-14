#include <iostream>
#include <cmath>
#include <iomanip>
using namespace std;

struct Vector
{
    Vector(double x, double y, double z): x(x), y(y), z(z) {}

    double x;
    double y;
    double z;
};

// скалярное произведение двух векторов
double ScalarProduct(Vector u, Vector v) {
    return (u.x * v.x) + (u.y * v.y) + (u.z * v.z);
};

// векторное произведение двух векторов
Vector VectorProduct(Vector u, Vector v) {
    double x = u.y * v.z - u.z * v.y;
    double y = u.x * v.z - u.z * v.x;
    double z = u.x * v.y - u.y * v.x;

    Vector ans(x, y, z);
    return ans;
};

// коллинеарны (ЛЗ) два вектора или нет
bool Collinear(Vector u, Vector v) {
    Vector vec_prod = VectorProduct(u, v);

    return ScalarProduct(vec_prod, vec_prod) == 0;
};

// расстояние между прямой p0 + s*u и прямой q0 + t*v
// возвращаем пару {расстояние, лежат ли точки на отрезках из задачи}
pair<double, bool> DistBetweenLines(Vector p0, Vector u, Vector q0, Vector v) {
    Vector w0(p0.x - q0.x, p0.y - q0.y, p0.z - q0.z);

    double u_u = ScalarProduct(u, u);
    double u_v = ScalarProduct(u, v);
    double v_v = ScalarProduct(v, v);
    double u_w0 = ScalarProduct(u, w0);
    double v_w0 = ScalarProduct(v, w0);

    if (Collinear(u, v)) {
        // если прямые параллельны, то все нужные действия и так будут проделаны потом, если вернем false
        return {-1, false};
    }

    double s_param = (u_v*v_w0 - v_v*u_w0) / (u_u*v_v - u_v*u_v);
    double t_param = (u_u*v_w0 - u_v*u_w0) / (u_u*v_v - u_v*u_v);

    w0.x += s_param * u.x - t_param * v.x;
    w0.y += s_param * u.y - t_param * v.y;
    w0.z += s_param * u.z - t_param * v.z;

    if (s_param >= 0 && s_param <= 1 && t_param >= 0 && t_param <= 1) {
        return {sqrt(ScalarProduct(w0, w0)), true};
    } else {
        return {sqrt(ScalarProduct(w0, w0)), false};
    }
}

// расстояние между отрезком p0 + s*u и отрезком q0 + t*v, 0<=s<= 1, 0<=t<=1
double DistBetweenSegments(Vector p0, Vector u, Vector q0, Vector v) {
    // находим расстояние между прямыми, содержащими отрезки
    pair <double, bool> dist_lines = DistBetweenLines(p0, u, q0, v);
    if (dist_lines.second) {  // если минимум достигается на точках отрезков, то сразу возвращаем ответ
        return dist_lines.first;
    }

    // хотим минимизировать:
    // |w|^2 = |p0 + s*u - q0 - t*v|^2 =
    // = |w0|^2 + s*s*|u|^2 + t*t*|v|^2 + 2*s*(u, w0) - 2*t*(v, w0) - 2*s*t(u, v)
    // это параболоид => минимум будет достигаться на границе => s == 0 || s == 1 || t == 0 || t == 1

    // w0 = p0 - q0
    Vector w0(p0.x - q0.x, p0.y - q0.y, p0.z - q0.z);

    // ищем минимум на границе параболоида для (s == 0 || s == 1) && (t == 0 || t == 1)
    // просто перебираем все возможные паре из 0 и 1 для s и t

    // s = 1, t = 1
    Vector w1(w0.x + u.x - v.x, w0.y + u.y - v.y, w0.z + u.z - v.z);
    double minn = sqrt(ScalarProduct(w1, w1));

    // s = 1, t = 0
    Vector w2(w0.x + u.x, w0.y + u.y, w0.z + u.z);
    minn = min(minn, sqrt(ScalarProduct(w2, w2)));

    // s = 0, t = 0
    Vector w3(w0.x, w0.y, w0.z);
    minn = min(minn, sqrt(ScalarProduct(w3, w3)));

    // s = 0, t = 1
    Vector w4(w0.x - v.x, w0.y - v.y, w0.z - v.z);
    minn = min(minn, sqrt(ScalarProduct(w4, w4)));

    // теперь ищем минимум для (s == 0 || s == 1 || t == 0 || t == 1)
    // будем по очереди присваивать параметрам s и t зн-ия 0 и 1, вычисляя значение второго параметра,
    // как вершины параболы

    // s = 0 => хотим минимизировать t*t*(v, v) - 2*t*(v, w0)
    // тогда min t = (v, w0) / (v, v) как вершина параболы
    double s_param = 0;
    double t_param = ScalarProduct(v, w0) / ScalarProduct(v, v);
    Vector w5(w0.x + s_param*u.x - t_param*v.x, w0.y + s_param*u.y - t_param*v.y, w0.z + s_param*u.z - t_param*v.z);
    if (t_param >= 0 && t_param <= 1) {
        minn = min(minn, sqrt(ScalarProduct(w5, w5)));
    }

    // s = 1 => хотим минимизировать t*t*(v, v) - 2*t*(v, w0) - 2*t*(v, u)
    // тогда min t = ((v, w0) + (v, u)) / (v, v) как вершина параболы
    s_param = 1;
    t_param = (ScalarProduct(v, w0) + ScalarProduct(v, u)) / ScalarProduct(v, v);
    Vector w6(w0.x + s_param*u.x - t_param*v.x, w0.y + s_param*u.y - t_param*v.y, w0.z + s_param*u.z - t_param*v.z);
    if (t_param >= 0 && t_param <= 1) {
        minn = min(minn, sqrt(ScalarProduct(w6, w6)));
    }

    // t = 0 => хотим минимизировать s*s*(u, u) + 2*s*(u, w0)
    // тогда min s = - (u, w0) / (u, u) как вершина параболы
    s_param = - ScalarProduct(u, w0) / ScalarProduct(u, u);
    t_param = 0;
    Vector w7(w0.x + s_param*u.x - t_param*v.x, w0.y + s_param*u.y - t_param*v.y, w0.z + s_param*u.z - t_param*v.z);
    if (s_param >= 0 && s_param <= 1) {
        minn = min(minn, sqrt(ScalarProduct(w7, w7)));
    }

    // t = 1 => хотим минимизировать s*s*(u, u) + 2*s*(u, w0) - 2*s*(u,v)
    // тогда min s = ((u,v) - (u, w0)) / (u, u) как вершина параболы
    s_param = (ScalarProduct(u, v) - ScalarProduct(u, w0)) / ScalarProduct(u, u);
    t_param = 1;
    Vector w8(w0.x + s_param*u.x - t_param*v.x, w0.y + s_param*u.y - t_param*v.y, w0.z + s_param*u.z - t_param*v.z);
    if (s_param >= 0 && s_param <= 1) {
        minn = min(minn, sqrt(ScalarProduct(w8, w8)));
    }

    return minn;
};

int main() {
    double x, y, z;

    cin >> x >> y >> z;  // первая точка первого вектора
    Vector p0(x, y, z);

    cin >> x >> y >> z;  // вторая точка первого вектора
    Vector p1(x, y, z);

    cin >> x >> y >> z;  // первая точка второго вектора
    Vector q0(x, y, z);

    cin >> x >> y >> z;  // вторая точка второго вектора
    Vector q1(x, y, z);

    Vector u(p1.x - p0.x, p1.y - p0.y, p1.z - p0.z);
    Vector v(q1.x - q0.x, q1.y - q0.y, q1.z - q0.z);

    cout << fixed << setprecision(10) << DistBetweenSegments(p0, u, q0, v);
}