type session;

type editor;
[@bs.send] external setTheme: (editor, string) => unit = "setTheme";

type ace;
[@bs.send] external edit: (ace, string) => editor = "edit";

[@bs.module "ace-builds/src-noconflict/ace.js"] external ace: ace = "default";
