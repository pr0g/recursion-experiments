#include <SDL3/SDL.h>
#define SDL_MAIN_USE_CALLBACKS
#include <SDL3/SDL_main.h>

#include <as-ops.h>

#include <format>
#include <iostream>
#include <random>
#include <ranges>
#include <stack>

static SDL_Window* g_window = nullptr;
static SDL_Renderer* g_renderer = nullptr;
static int64_t g_prev_ns = 0;

const int g_window_width = 800;
const int g_window_height = 600;

struct line_t {
  as_point2f begin;
  as_point2f end;
};

struct box_t {
  as_point2f xy;
  as_vec2f wh;
};

enum class mode_e {
  spiral,
  sierpinski_triangle,
  sierpinski_carpet,
  fractal_tree
};

struct fractals_t {
  std::vector<line_t> spiral;
  std::vector<line_t> triangles;
  std::vector<line_t> branches;
  std::vector<box_t> boxes;
  mode_e mode = mode_e::spiral;
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

// todo - move to as-c-math library...s
as_vec2f as_mat22f_mul_vec2f_v(as_mat22f mat, const as_vec2f vec) {
  return as_mat22f_mul_vec2f(&mat, vec);
}

as_point2f midpoint(const as_point2f& begin, const as_point2f& end) {
  return as_point2f_add_vec2f(
    begin, as_vec2f_mul_float(as_point2f_sub_point2f(end, begin), 0.5f));
}

bool triangle_too_small(const as_point2f& p1, const as_point2f& p2) {
  return as_vec2f_length(as_point2f_sub_point2f(p2, p1)) < 4.0f;
}

void draw_triangle(
  fractals_t& fractals, const as_point2f& p1, const as_point2f& p2,
  const as_point2f& p3) {
  if (triangle_too_small(p1, p2)) {
    return;
  }
  fractals.triangles.push_back(line_t{.begin = p1, .end = p2});
  fractals.triangles.push_back(line_t{.begin = p2, .end = p3});
  fractals.triangles.push_back(line_t{.begin = p3, .end = p1});

  const auto midpoint_p1p2 = midpoint(p1, p2);
  const auto midpoint_p2p3 = midpoint(p2, p3);
  const auto midpoint_p3p1 = midpoint(p3, p1);

  draw_triangle(fractals, p1, midpoint_p1p2, midpoint_p3p1);
  draw_triangle(fractals, midpoint_p1p2, p2, midpoint_p2p3);
  draw_triangle(fractals, midpoint_p3p1, midpoint_p2p3, p3);
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

bool box_too_small(const box_t& box) {
  return box.wh.y < 2.0f;
}

void add_box(fractals_t& fractals, const box_t& box) {
  fractals.boxes.push_back(box);
}

void draw_inner_rectangle(fractals_t& fractals, const box_t& box) {
  if (box_too_small(box)) {
    return;
  }
  const float one_third_width = box.wh.x / 3.0f;
  const float one_third_height = box.wh.y / 3.0f;
  const float two_thirds_width = one_third_width * 2.0f;
  const float two_thirds_height = one_third_height * 2.0f;

  fractals.boxes.push_back(
    {.xy = as_point2f(box.xy.x + one_third_width, box.xy.y + one_third_height),
     .wh = as_vec2f(one_third_width, one_third_height)});

  // top
  draw_inner_rectangle(
    fractals, {.xy = as_point2f(box.xy.x, box.xy.y),
               as_vec2f(one_third_width, one_third_height)});
  draw_inner_rectangle(
    fractals, {.xy = as_point2f(box.xy.x + one_third_width, box.xy.y),
               as_vec2f(one_third_width, one_third_height)});
  draw_inner_rectangle(
    fractals, {.xy = as_point2f(box.xy.x + two_thirds_width, box.xy.y),
               as_vec2f(one_third_width, one_third_height)});

  // middle
  draw_inner_rectangle(
    fractals, {.xy = as_point2f(box.xy.x, box.xy.y + one_third_height),
               as_vec2f(one_third_width, one_third_height)});
  draw_inner_rectangle(
    fractals,
    {.xy = as_point2f(box.xy.x + two_thirds_width, box.xy.y + one_third_height),
     as_vec2f(one_third_width, one_third_height)});

  // bottom
  draw_inner_rectangle(
    fractals, {.xy = as_point2f(box.xy.x, box.xy.y + two_thirds_height),
               as_vec2f(one_third_width, one_third_height)});
  draw_inner_rectangle(
    fractals,
    {.xy = as_point2f(box.xy.x + one_third_width, box.xy.y + two_thirds_height),
     as_vec2f(one_third_width, one_third_height)});
  draw_inner_rectangle(
    fractals, {.xy = as_point2f(
                 box.xy.x + two_thirds_width, box.xy.y + two_thirds_height),
               as_vec2f(one_third_width, one_third_height)});
}

void draw_carpet(fractals_t& fractals, const box_t& box) {
  fractals.boxes.push_back(box);
  draw_inner_rectangle(fractals, box);
}

void draw_branch(
  fractals_t& fractals, const as_point2f& start, const float angle,
  const float length) {
  if (length < 5) {
    return;
  }
  const as_vec2f heading = as_mat22f_mul_vec2f_v(
    as_mat22f_rotation(as_radians_from_degrees(angle)), as_vec2f{.y = -1.0f});
  const as_point2f finish =
    as_point2f_add_vec2f(start, as_vec2f_mul_float(heading, length));

  fractals.branches.push_back(line_t{.begin = start, .end = finish});

  std::random_device rd;
  std::mt19937 gen(rd());

  std::uniform_real_distribution<> angle_distribution(10.0f, 30.0f);
  std::uniform_real_distribution<> decrease_distribution(8.0f, 15.0f);

  draw_branch(
    fractals, finish, angle - angle_distribution(gen),
    length - decrease_distribution(gen));
  draw_branch(
    fractals, finish, angle + angle_distribution(gen),
    length - decrease_distribution(gen));
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
  // create spiral
  for (int i = 0; i < 360; i++) {
    const auto turtle_last_position = turtle.position;
    turtle_forward(turtle, i);
    turtle_left(turtle, 59.0f);
    fractals->spiral.push_back(
      line_t{.begin = turtle_last_position, .end = turtle.position});
  }

  // Sierpinski triangle
  draw_triangle(
    *fractals, as_point2f{-300, 250}, as_point2f{0, -250},
    as_point2f{300, 250});

  // Sierpinski carpet
  draw_carpet(
    *fractals,
    box_t{.xy = as_point2f(100.0f, 100.0f), .wh = as_vec2f(600.0f, 400.0f)});

  // fractal tree
  draw_branch(*fractals, as_point2f{.x = 0, .y = 200}, 0.0f, 90.0f);

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

  const auto draw_lines = [](const std::vector<line_t>& lines) {
    for (const auto line : lines) {
      const auto offset =
        as_vec2f{.x = g_window_width / 2, .y = g_window_height / 2};
      auto begin = as_point2f_add_vec2f(line.begin, offset);
      auto end = as_point2f_add_vec2f(line.end, offset);
      SDL_RenderLine(g_renderer, begin.x, begin.y, end.x, end.y);
    }
  };

  switch (fractals->mode) {
    case mode_e::spiral: {
      draw_lines(fractals->spiral);
    } break;
    case mode_e::sierpinski_triangle: {
      draw_lines(fractals->triangles);
    } break;
    case mode_e::sierpinski_carpet: {
      if (!fractals->boxes.empty()) {
        SDL_SetRenderDrawColor(g_renderer, 0, 0, 0, 255);
        const auto box = fractals->boxes[0];
        SDL_FRect rect =
          SDL_FRect{.x = box.xy.x, .y = box.xy.y, .w = box.wh.x, .h = box.wh.y};
        SDL_RenderFillRect(g_renderer, &rect);
        SDL_SetRenderDrawColor(g_renderer, 255, 255, 255, 255);
        for (const auto box : fractals->boxes | std::views::drop(1)) {
          SDL_FRect rect = SDL_FRect{
            .x = box.xy.x, .y = box.xy.y, .w = box.wh.x, .h = box.wh.y};
          SDL_RenderFillRect(g_renderer, &rect);
        }
      }
    } break;
    case mode_e::fractal_tree: {
      draw_lines(fractals->branches);
    } break;
  }

  SDL_RenderPresent(g_renderer);

  return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppEvent(void* appstate, SDL_Event* event) {
  fractals_t* fractals = (fractals_t*)(appstate);
  if (event->type == SDL_EVENT_KEY_DOWN) {
    if (event->key.scancode == SDL_SCANCODE_1) {
      fractals->mode = mode_e::spiral;
    } else if (event->key.scancode == SDL_SCANCODE_2) {
      fractals->mode = mode_e::sierpinski_triangle;
    } else if (event->key.scancode == SDL_SCANCODE_3) {
      fractals->mode = mode_e::sierpinski_carpet;
    } else if (event->key.scancode == SDL_SCANCODE_4) {
      fractals->mode = mode_e::fractal_tree;
    }
  }
  if (event->type == SDL_EVENT_QUIT) {
    return SDL_APP_SUCCESS;
  }
  return SDL_APP_CONTINUE;
}

void SDL_AppQuit(void* appstate, SDL_AppResult result) {
  fractals_t* fractals = (fractals_t*)(appstate);
  delete fractals;
}
