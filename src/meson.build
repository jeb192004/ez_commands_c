ez_commands_sources = [
  'main.c',
  'ez_commands-window.c',
  'my_clipboard.c',
  'json_parser.c',
  'get_commands_file.c',
  'build_command_list_item.c',
  'replace.c',
  'dialogs.c',
]

ez_commands_deps = [
  dependency('gtk4'),
  dependency('libadwaita-1'),
  dependency('json-glib-1.0'),
  ]

 gnome.compile_resources('ez_commands-resources',
  'ez_commands.gresource.xml',
  c_name: 'ez_commands'
)

executable('ez_commands', ez_commands_sources,
  dependencies: ez_commands_deps,
       install: true,
)