#include <iostream>
#include <SDL2/SDL.h>

enum Shape {
    Rectangle = 0,
    Triangle = 1,
    Circle = 2
};

struct Rect {
    int x, y;
    int w, h;
};

struct Circ {
    int x, y;
    int r;
};

struct Rotation {
    bool is_running;
    double theta;

    SDL_Window *window;
    SDL_Renderer *renderer;

    Rect rect = { 400, 300, 100, 100 };
    Circ circ = { 400, 300, 50 };

    SDL_FPoint *points;
    SDL_FPoint *trans_points;

    void get_points(const Shape shape, const Circ circle, const Rect rect) {
        int count;
        double angle;

        points = new SDL_FPoint[rect.w * rect.h];
        trans_points = new SDL_FPoint[rect.w * rect.h];

        SDL_FPoint point;

        switch (shape) {
        case Rectangle:
            for (int i = 0; i < rect.w; ++i) {
                for (int j = 0; j < rect.h; ++j) {
                    point.x = i + rect.x;
                    point.y = j + rect.y;

                    points[i + (j * rect.w)] = point;
                }
            }

            break;

        case Triangle:
            for (int i = 0; i < rect.w; ++i) {
                for (int j = i; j < rect.h; ++j) {
                    point.x = i + rect.x;
                    point.y = j + rect.y;

                    points[i + (j * rect.w)] = point;
                }
            }

            break;

        case Circle:
            count = int(2 * 3.1415 * circ.r + 1);
            angle = 2 * 3.1415 / count;

            for (int i = 0; i < count; ++i) {
                double current_angle = i * angle;

                point.x = circ.x + circ.r * SDL_cos(current_angle);
                point.y = circ.y + circ.r * SDL_sin(current_angle);

                points[i] = point;
            }

            break;
        default:
            break;
        }
    }

    void rotate(const double theta, SDL_FPoint *points, const int count) {
        for (int i = 0; i < count; ++i) {
            SDL_FPoint point = points[i];

            double x = point.x - 400 - rect.w / 2;
            double y = point.y - 300 - rect.h / 2;

            point.x = 400 + (x * SDL_cos(theta) - y * SDL_sin(theta));
            point.y = 300 + (x * SDL_sin(theta) + y * SDL_cos(theta));

            trans_points[i] = point;
        }
    }

    void init(const Shape shape, const char *title, const int x, const int y, const int w, const int h, const bool fullscreen) {
        SDL_Init(SDL_INIT_EVERYTHING);

        window = SDL_CreateWindow(title, x, y, w, h, (fullscreen) ? SDL_WINDOW_FULLSCREEN : 0);
        renderer = SDL_CreateRenderer(window, -1, 0);

        get_points(shape, circ, rect);

        is_running = true;
    }

    void handle_events() {
        SDL_Event e;
        SDL_PollEvent(&e);

        switch (e.type) {
        case SDL_QUIT:
            is_running = false;
            break;
        default:
            break;
        }
    }

    void update(const Shape shape) {
        int count = (shape == Circle) ? int(2 * 3.141593 * circ.r) : rect.w * rect.h;

        rotate(theta, points, count);
        theta += 0.05;

        SDL_Delay(16);
    }

    void render(const Shape shape) {
        int count = (shape == Circle) ? int(2 * 3.141593 * circ.r) : rect.w * rect.h;

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderDrawPointsF(renderer, trans_points, count);

        SDL_RenderPresent(renderer);
    }

    void clear() {
        SDL_DestroyWindow(window);
        SDL_DestroyRenderer(renderer);
        SDL_Quit();

        delete[] points;
        delete[] trans_points;
    }
};

int main(int argc, char const *argv[]) {
    Rotation *r = new Rotation();
    Shape shape = Circle;

    r->init(shape, "title", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, false);
    while (r->is_running) {
        r->handle_events();
        r->update(shape);
        r->render(shape);
    }

    r->clear();
    delete r;

    return 0;
}
