#pragma once

#include "input.h"
#include "FilenameSequence.h"

enum class Definition {
  none,
  begin,

  texture,
  width,
  height,
  max_width,
  max_height,
  power_of_two,
  allow_rotate,
  padding,
  deduplicate,

  path,
  sheet,
  colorkey,
  grid,
  offset,
  sprite,
  rect,
  skip,
  span,
  pivot,
  tag,
  trim,
  trim_margin,
};

struct State {
  Definition definition{ };
  int level;
  std::string indent;

  std::filesystem::path texture;
  int width{ };
  int height{};
  int max_width{ };
  int max_height{};
  bool power_of_two{ };
  bool allow_rotate{ };
  int border_padding{ };
  int shape_padding{ };
  bool deduplicate{ };

  std::filesystem::path path;
  FilenameSequence sheet;
  RGBA colorkey{ };
  std::map<std::string, std::string> tags;
  std::string sprite;
  Size grid{ };
  Size span{ 1, 1 };
  Pivot pivot{ PivotX::center, PivotY::middle };
  PointF pivot_point{ };
  Rect rect{ };
  Trim trim{ };
  int trim_margin{ };
};

class InputParser {
public:
  explicit InputParser(const Settings& settings);
  void parse(std::istream& input);
  const std::vector<Sprite>& sprites() const & { return m_sprites; }
  std::vector<Sprite> sprites() && { return std::move(m_sprites); }
  std::string autocomplete_output() const { return m_autocomplete_output.str(); }

private:
  [[noreturn]] void error(std::string message);
  void check(bool condition, std::string_view message);
  TexturePtr get_texture(const State& state);
  ImagePtr get_sheet(const State& state);
  ImagePtr get_sheet(const std::filesystem::path& full_path, RGBA colorkey);
  void sprite_ends(State& state);
  void deduce_sequence_sprites(State& state);
  void deduce_grid_sprites(State& state);
  void deduce_unaligned_sprites(State& state);
  void texture_ends(State& state);
  void sheet_ends(State& state);
  void apply_definition(State& state,
      Definition definition,
      std::vector<std::string_view>& arguments);
  bool has_implicit_scope(Definition definition);
  void scope_ends(State& state);

  const Settings& m_settings;
  std::stringstream m_autocomplete_output;
  int m_line_number{ };
  std::map<std::filesystem::path, TexturePtr> m_textures;
  std::map<std::filesystem::path, ImagePtr> m_sheets;
  std::vector<Sprite> m_sprites;
  int m_sprites_in_current_sheet{ };
  Point m_current_offset{ };
  int m_current_sequence_index{ };
};
