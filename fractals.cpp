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

using lines_t = std::vector<line_t>;

struct box_t {
  as_point2f xy;
  as_vec2f wh;
};

enum class mode_e {
  spiral,
  sierpinski_triangle,
  sierpinski_carpet,
  tree,
  snowflake,
  hilbert,
  chequered_box,
  peano,
};

struct fractals_t {
  std::vector<line_t> spiral;
  std::vector<line_t> triangles;
  std::vector<line_t> branches;
  std::vector<box_t> carpet;
  std::vector<line_t> snowflake;
  std::vector<line_t> hilbert;
  std::vector<box_t> chequered_box;
  std::vector<line_t> peano;
  mode_e mode = mode_e::peano;
};

struct turtle_t {
  as_vec2f heading = {1.0f, 0.0f};
  as_point2f position = {0.0f, 0.0f};
  float scale = 0.8f;
};

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

void turtle_forward(turtle_t& turtle, const int distance, lines_t& lines) {
  const auto previous_position = turtle.position;
  turtle_forward(turtle, distance);
  const auto next_position = turtle.position;
  lines.push_back(line_t{.begin = previous_position, .end = next_position});
}

void turtle_left(turtle_t& turtle, const float degrees) {
  turtle.heading = as_mat22f_mul_vec2f_v(
    as_mat22f_rotation(as_radians_from_degrees(-degrees)), turtle.heading);
}

void turtle_right(turtle_t& turtle, const float degrees) {
  turtle.heading = as_mat22f_mul_vec2f_v(
    as_mat22f_rotation(as_radians_from_degrees(degrees)), turtle.heading);
}

bool box_too_small(const box_t& box) {
  return box.wh.y < 2.0f;
}

void draw_inner_rectangle(fractals_t& fractals, const box_t& box) {
  if (box_too_small(box)) {
    return;
  }
  const float one_third_width = box.wh.x / 3.0f;
  const float one_third_height = box.wh.y / 3.0f;
  const float two_thirds_width = one_third_width * 2.0f;
  const float two_thirds_height = one_third_height * 2.0f;

  fractals.carpet.push_back(
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
  fractals.carpet.push_back(box);
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

// doesn't handle removing previous line
void draw_koch_curve(
  fractals_t& fractals, const as_point2f& position, const as_vec2f& heading,
  const float length) {
  if (length < 1) {
    // base case
    return;
  } else {
    fractals.snowflake.push_back(
      line_t{
        .begin = position,
        .end =
          as_point2f_add_vec2f(position, as_vec2f_mul_float(heading, length))});
    const as_point2f left_position = as_point2f_add_vec2f(
      position, as_vec2f_mul_float(heading, length / 3.0f));
    const as_vec2f left_heading = as_mat22f_mul_vec2f_v(
      as_mat22f_rotation(as_radians_from_degrees(60.0f)), heading);
    draw_koch_curve(fractals, position, heading, length / 3.0f);
    draw_koch_curve(fractals, left_position, left_heading, length / 3.0f);
    const as_point2f right_position = as_point2f_add_vec2f(
      left_position, as_vec2f_mul_float(left_heading, length / 3.0f));
    const as_vec2f right_heading = as_mat22f_mul_vec2f_v(
      as_mat22f_rotation(as_radians_from_degrees(-120.0f)), left_heading);
    draw_koch_curve(
      fractals,
      as_point2f_add_vec2f(
        position, as_vec2f_mul_float(heading, 2.0f * (length / 3.0f))),
      heading, length / 3.0f);
    draw_koch_curve(fractals, right_position, right_heading, length / 3.0f);
  }
}

void draw_koch_snowflake(
  fractals_t& fractals, const as_point2f& position, const float length) {
  as_vec2f heading = {.x = 1.0f};
  as_point2f curve_start = position;
  float heading_degrees = -120.0f;
  for (int i = 0; i < 3; i++) {
    draw_koch_curve(fractals, curve_start, heading, length);
    curve_start =
      as_point2f_add_vec2f(curve_start, as_vec2f_mul_float(heading, length));
    heading = as_mat22f_mul_vec2f_v(
      as_mat22f_rotation(as_radians_from_degrees(heading_degrees)), heading);
  }
}

void hilbert_curve_quadrant(
  const int level, const float angle, const float line_length, turtle_t& turtle,
  fractals_t& fractals) {
  if (level == 0) {
    return;
  }
  turtle_right(turtle, angle);
  hilbert_curve_quadrant(level - 1, -angle, line_length, turtle, fractals);
  turtle_forward(turtle, line_length, fractals.hilbert);
  turtle_left(turtle, angle);
  hilbert_curve_quadrant(level - 1, angle, line_length, turtle, fractals);
  turtle_forward(turtle, line_length, fractals.hilbert);
  hilbert_curve_quadrant(level - 1, angle, line_length, turtle, fractals);
  turtle_left(turtle, angle);
  turtle_forward(turtle, line_length, fractals.hilbert);
  hilbert_curve_quadrant(level - 1, -angle, line_length, turtle, fractals);
  turtle_right(turtle, angle);
}

void hilbert_curve(
  fractals_t& fractals, turtle_t& turtle, const float line_length,
  const int levels) {
  const float angle = 90.0f;
  hilbert_curve_quadrant(levels, angle, line_length, turtle, fractals);
  turtle_forward(turtle, line_length, fractals.hilbert);
  hilbert_curve_quadrant(levels, angle, line_length, turtle, fractals);
  turtle_left(turtle, angle);
  turtle_forward(turtle, line_length, fractals.hilbert);
  turtle_left(turtle, angle);
  hilbert_curve_quadrant(levels, angle, line_length, turtle, fractals);
  turtle_forward(turtle, line_length, fractals.hilbert);
  hilbert_curve_quadrant(levels, angle, line_length, turtle, fractals);
}

void draw_inner_box(fractals_t& fractals, const box_t& box, int level) {
  if (level == 0) {
    return;
  }
  const float one_third_width = box.wh.x / 3.0f;
  const float one_third_height = box.wh.y / 3.0f;
  const float two_thirds_width = one_third_width * 2.0f;
  const float two_thirds_height = one_third_height * 2.0f;

  fractals.chequered_box.push_back(
    {.xy = as_point2f(box.xy.x + one_third_width, box.xy.y),
     .wh = as_vec2f(one_third_width, one_third_height)});
  fractals.chequered_box.push_back(
    {.xy = as_point2f(box.xy.x, box.xy.y + one_third_height),
     .wh = as_vec2f(one_third_width, one_third_height)});
  fractals.chequered_box.push_back(
    {.xy = as_point2f(box.xy.x + two_thirds_width, box.xy.y + one_third_height),
     .wh = as_vec2f(one_third_width, one_third_height)});
  fractals.chequered_box.push_back(
    {.xy = as_point2f(box.xy.x + one_third_width, box.xy.y + two_thirds_height),
     .wh = as_vec2f(one_third_width, one_third_height)});

  draw_inner_box(
    fractals,
    {.xy = as_point2f(box.xy.x, box.xy.y),
     as_vec2f(one_third_width, one_third_height)},
    level - 1);
  draw_inner_box(
    fractals,
    {.xy = as_point2f(box.xy.x + two_thirds_width, box.xy.y),
     as_vec2f(one_third_width, one_third_height)},
    level - 1);
  draw_inner_box(
    fractals,
    {.xy = as_point2f(box.xy.x + one_third_width, box.xy.y + one_third_height),
     as_vec2f(one_third_width, one_third_height)},
    level - 1);
  draw_inner_box(
    fractals,
    {.xy = as_point2f(box.xy.x, box.xy.y + two_thirds_height),
     as_vec2f(one_third_width, one_third_height)},
    level - 1);
  draw_inner_box(
    fractals,
    {.xy =
       as_point2f(box.xy.x + two_thirds_width, box.xy.y + two_thirds_height),
     as_vec2f(one_third_width, one_third_height)},
    level - 1);
}

void draw_chequered_box(
  fractals_t& fractals, const box_t& box, const int levels) {
  fractals.chequered_box.push_back(box);
  draw_inner_box(fractals, box, levels);
}

void peano_curve_quadrant(
  const int level, const float angle, const float line_length, turtle_t& turtle,
  fractals_t& fractals) {
  if (level == 0) {
    return;
  }
  peano_curve_quadrant(level - 1, angle, line_length, turtle, fractals);
  turtle_forward(turtle, line_length, fractals.peano);
  peano_curve_quadrant(level - 1, -angle, line_length, turtle, fractals);
  turtle_forward(turtle, line_length, fractals.peano);
  peano_curve_quadrant(level - 1, angle, line_length, turtle, fractals);
  turtle_right(turtle, angle);
  turtle_forward(turtle, line_length, fractals.peano);
  turtle_right(turtle, angle);
  peano_curve_quadrant(level - 1, -angle, line_length, turtle, fractals);
  turtle_forward(turtle, line_length, fractals.peano);
  peano_curve_quadrant(level - 1, angle, line_length, turtle, fractals);
  turtle_forward(turtle, line_length, fractals.peano);
  peano_curve_quadrant(level - 1, -angle, line_length, turtle, fractals);
  turtle_left(turtle, angle);
  turtle_forward(turtle, line_length, fractals.peano);
  turtle_left(turtle, angle);
  peano_curve_quadrant(level - 1, angle, line_length, turtle, fractals);
  turtle_forward(turtle, line_length, fractals.peano);
  peano_curve_quadrant(level - 1, -angle, line_length, turtle, fractals);
  turtle_forward(turtle, line_length, fractals.peano);
  peano_curve_quadrant(level - 1, angle, line_length, turtle, fractals);
}

void peano_curve(
  fractals_t& fractals, turtle_t& turtle, const float line_length,
  const int levels) {
  const float angle = 90.0f;
  peano_curve_quadrant(levels, angle, line_length, turtle, fractals);
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

  {
    turtle_t turtle;
    // create spiral
    for (int i = 0; i < 360; i++) {
      const auto turtle_last_position = turtle.position;
      turtle_forward(turtle, i);
      turtle_left(turtle, 59.0f);
      fractals->spiral.push_back(
        line_t{.begin = turtle_last_position, .end = turtle.position});
    }
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

  // Koch snowflake
  draw_koch_snowflake(*fractals, as_point2f{-200.0f, 125.0f}, 400.0f);

  // Hilbert curve
  {
    turtle_t turtle = {.position = as_point2f{.x = -250.0f, .y = 0.0f}};
    hilbert_curve(*fractals, turtle, 5.0f, 6);
  }

  // Peano curve
  {
    turtle_t turtle = {
      .position = as_point2f{.x = -250.0f, .y = 250.0f},
      .heading = as_vec2f{.y = -1.0f}};
    peano_curve(*fractals, turtle, 8.0f, 4);
  }

  // recursive chequered box pattern
  draw_chequered_box(
    *fractals,
    box_t{.xy = as_point2f(200.0f, 100.0f), .wh = as_vec2f(400.0f, 400.0f)}, 4);

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

  const auto draw_boxes = [](const std::vector<box_t>& boxes) {
    if (boxes.empty()) {
      return;
    }
    SDL_SetRenderDrawColor(g_renderer, 0, 0, 0, 255);
    const auto box = boxes[0];
    SDL_FRect rect =
      SDL_FRect{.x = box.xy.x, .y = box.xy.y, .w = box.wh.x, .h = box.wh.y};
    SDL_RenderFillRect(g_renderer, &rect);
    SDL_SetRenderDrawColor(g_renderer, 255, 255, 255, 255);
    for (const auto box : boxes | std::views::drop(1)) {
      SDL_FRect rect =
        SDL_FRect{.x = box.xy.x, .y = box.xy.y, .w = box.wh.x, .h = box.wh.y};
      SDL_RenderFillRect(g_renderer, &rect);
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
      draw_boxes(fractals->carpet);
    } break;
    case mode_e::tree: {
      draw_lines(fractals->branches);
    } break;
    case mode_e::snowflake: {
      draw_lines(fractals->snowflake);
    } break;
    case mode_e::hilbert: {
      draw_lines(fractals->hilbert);
    } break;
    case mode_e::chequered_box: {
      draw_boxes(fractals->chequered_box);
    } break;
    case mode_e::peano: {
      draw_lines(fractals->peano);
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
      fractals->mode = mode_e::tree;
    } else if (event->key.scancode == SDL_SCANCODE_5) {
      fractals->mode = mode_e::snowflake;
    } else if (event->key.scancode == SDL_SCANCODE_6) {
      fractals->mode = mode_e::hilbert;
    } else if (event->key.scancode == SDL_SCANCODE_7) {
      fractals->mode = mode_e::chequered_box;
    } else if (event->key.scancode == SDL_SCANCODE_8) {
      fractals->mode = mode_e::peano;
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
