#include <SDL3/SDL.h>
#define SDL_MAIN_USE_CALLBACKS
#include <SDL3/SDL_main.h>

#include <as-ops.h>

#include <format>
#include <iostream>
#include <stack>

static SDL_Window* g_window = nullptr;
static SDL_Renderer* g_renderer = nullptr;
static int64_t g_prev_ns = 0;

const int g_window_width = 800;
const int g_window_height = 600;

struct line_t {
  as_point2i begin;
  as_point2i end;
};

struct fractals_t {
  as_mat44f mvp;
  std::vector<line_t> lines;
};

struct turtle_t {
  as_vec2f heading = {1.0f, 0.0f};
  as_point2f position = {0.0f, 0.0f};
  float scale = 0.8f;
};

// todo - move to as-c-math library...
as_vec2f as_mat22f_mul_vec2f(const as_mat22f* const mat, const as_vec2f vec) {
  return (as_vec2f){.x = mat->elem[0] * vec.x + mat->elem[1] * vec.y,
                    .y = mat->elem[2] * vec.x + mat->elem[3] * vec.y};
}

void turtle_forward(turtle_t& turtle, const int distance) {
  turtle.position = as_point2f_add_vec2f(
    turtle.position,
    as_vec2f_mul_float(turtle.heading, turtle.scale * distance));
}

void turtle_left(turtle_t& turtle, const float degrees) {
  as_mat22f rotation = as_mat22f_rotation(as_radians_from_degrees(-degrees));
  turtle.heading = as_mat22f_mul_vec2f(&rotation, turtle.heading);
}

void turtle_right(turtle_t& turtle, const float degrees) {
  as_mat22f rotation = as_mat22f_rotation(as_radians_from_degrees(degrees));
  turtle.heading = as_mat22f_mul_vec2f(&rotation, turtle.heading);
}

SDL_AppResult SDL_AppInit(void** appstate, int argc, char** argv) {
  SDL_SetAppMetadata("Fractals", "1.0", "com.tomhultonharrop.fractals");

  if (!SDL_Init(SDL_INIT_VIDEO)) {
    SDL_Log("Couldn't initialize SDL: %s", SDL_GetError());
    return SDL_APP_FAILURE;
  }

  if (!SDL_CreateWindowAndRenderer(
        "Fractals", g_window_width, g_window_height, 0, &g_window,
        &g_renderer)) {
    SDL_Log("Couldn't create window/renderer: %s", SDL_GetError());
    return SDL_APP_FAILURE;
  }

  SDL_SetRenderVSync(g_renderer, 1); // enable vsync

  fractals_t* fractals = new fractals_t();

  turtle_t turtle;
  for (int i = 0; i < 360; i++) {
    const auto turtle_last_position = turtle.position;
    turtle_forward(turtle, i);
    turtle_left(turtle, 59.0f);
    fractals->lines.push_back(
      line_t{
        .begin = as_point2i_from_point2f(turtle_last_position),
        .end = as_point2i_from_point2f(turtle.position)});
  }

  *appstate = fractals;

  return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppIterate(void* appstate) {
  fractals_t* fractals = (fractals_t*)(appstate);

  const uint64_t now_ns = SDL_GetTicksNS();
  const uint64_t delta_ticks_ns = now_ns - g_prev_ns;
  g_prev_ns = now_ns;

  const double delta_time = delta_ticks_ns * 1.0e-9;

  SDL_SetRenderDrawColor(g_renderer, 242, 242, 242, SDL_ALPHA_OPAQUE);
  SDL_RenderClear(g_renderer);

  SDL_SetRenderDrawColor(g_renderer, 39, 61, 113, 255);
  for (const auto line : fractals->lines) {
    const auto offset =
      as_vec2i{.x = g_window_width / 2, .y = g_window_height / 2};
    auto begin = as_point2i_add_vec2i(line.begin, offset);
    auto end = as_point2i_add_vec2i(line.end, offset);
    SDL_RenderLine(g_renderer, begin.x, begin.y, end.x, end.y);
  }

  SDL_RenderPresent(g_renderer);

  return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppEvent(void* appstate, SDL_Event* event) {
  if (event->type == SDL_EVENT_QUIT) {
    return SDL_APP_SUCCESS;
  }
  return SDL_APP_CONTINUE;
}

void SDL_AppQuit(void* appstate, SDL_AppResult result) {
  fractals_t* fractals = (fractals_t*)(appstate);
  delete fractals;
}
